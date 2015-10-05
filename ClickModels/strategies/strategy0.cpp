#include "strategy0.h"
#include "counters.h"
#include <algorithm>

vector<int> Strategy0::calculate(const Query& query)
{
    for (size_t i = 0; i < 10; ++i)
    {
        const vector<double>& found = counters.user_url.watch(query.person, query.urls[i]);
        if (found.size() > 0 && found[0] > 1 - 1e-5)
        {
            return {};
        }
    }

    const vector<double>& countOnRank0 = counters.query_rank.watch(query.id, 0);
    const vector<double>& countOnRank1 = counters.query_rank.watch(query.id, 1);

    if (countOnRank0.size() > 0 && countOnRank1.size() == 0)
        return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    if (countOnRank0.size() > 0 && countOnRank1.size() > 0 && countOnRank0[0] >= 2 * countOnRank1[0])
        return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    size_t num_users = counters.query_user.watch(query.id).size();
    if (num_users < 10)
    {
        return {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    }

    vector<double> prob(10);
    double currentProb = 1;
    for (size_t i = 0; i < 10; ++i)
    {
        const vector<double>& pars = counters.query_url.watch(query.id, query.urls[i]);
        if (pars.size() > 0 && pars[1] > 5) {
            prob[i] = currentProb * pars[1];
            currentProb *= 1.0 - pars[3] / pars[1];
        } else {
            prob[i] = prob[i-1]*0.9;
        }
    }
    vector<int> ranging = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::sort(ranging.begin(), ranging.end(), [&](int i, int j) { return prob[i] > prob[j]; } );
    return ranging;
}
