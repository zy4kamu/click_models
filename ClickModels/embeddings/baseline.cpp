#include "baseline.h"
#include "counters.h"
#include <set>
#include "filters.h"

static const std::string OUT_DIRECTORY =  "/Users/irtalk/Desktop/UVA/first/";
static const std::string DAY_DATA_FOLDER = OUT_DIRECTORY + "data_by_days/";
static const std::string DAY_DATA_FOLDER_VW = OUT_DIRECTORY + "data_by_days_VW/";
static const std::string DATA_FILE = OUT_DIRECTORY + "my_data/train_bin";
static const int SERP_SIZE = 10;
static Counters counters1;

void DeleteElementsFromUUMAP(uumap& m, const std::set<size_t>& s)
{
    for (auto it = m.data.begin(); it != m.data.end();)
    {
        if(s.find(it->first) == s.end()) {
            it = m.data.erase(it);
        }
        else
            it++;
    }
}

void DeleteSomeCounters(const std::set<size_t>& users,
                        const std::set<size_t>& queries)
{
    DeleteElementsFromUUMAP(counters1.query_rank, queries);
    DeleteElementsFromUUMAP(counters1.query_url, queries);
    DeleteElementsFromUUMAP(counters1.query_user, queries);
    DeleteElementsFromUUMAP(counters1.user_rank, users);
    DeleteElementsFromUUMAP(counters1.user_url, users);
    DeleteElementsFromUUMAP(counters1.user_query, users);
}

Url_type UserData::Get(int person, int url)
{
    if (data.find(person) != data.end())
    {
        if (data[person].find(url) != data[person].end())
        {
            return data[person][url];
        }
    }
    return Url_type();
}

Url_type Get(int url, const std::unordered_map<int, Url_type>& local)
{
   if (local.find(url) != local.end())
   {
       return local.at(url);
   }
   return Url_type();
}

void BaseLine::Write_Example(int enumerator, int rank,
                             const std::unordered_map<int, Url_type>& local,
                             const Query& serp, ofstream* file)
{
    //std::cout << "Enumerator " << enumerator << " " <<serp.type[rank] << "\n";
    int label = serp.type[rank] >= 1;
    label = 2 * (label - 0.5);
    int weight = 1.;
    if (serp.type[rank] == 2)
    {
        weight = 2.;
    }
    int filter1 = Filter1(counters1.query_user, counters1.user_url, counters1.query_rank, serp);
    int filter2 = Filter2(counters1.query_user, counters1.user_url, counters1.query_rank, serp);
    int filter3 = Filter3(counters1.query_user, counters1.user_url, counters1.query_rank, serp);
    int filter4 = Filter4(counters1.query_user, counters1.user_url, counters1.query_rank, serp);
    int user_history = 0;
    if (counters1.user_query.watch(serp.person, -1).size() > 0)
    {
        user_history = counters1.user_query.watch(serp.person, -1)[0];
    }
    (*file) << label << " " << weight << " '" <<enumerator << "_" << filter1 <<  "_" << filter2 << "_" <<
            counters1.query_user.watch(serp.id).size() << "_" << filter4 << "_" << user_history << "_" << serp.type[rank]
            <<" | ";
    (*file) << "r" << rank << " ";
    (*file) << "q" << serp.id << " ";
    for (int i = 0; i < serp.terms.size(); ++i)
    {
        (*file) << "q_t" << i << "_" << serp.terms[i] << " ";
    }
    (*file) << "u" << serp.urls[rank] << " ";
    (*file) << "d" << serp.domains[rank] << " ";
    (*file)<< "u_q" << serp.urls[rank] << "_" << serp.id << " ";
    (*file) << "u_r" << serp.urls[rank] << "_" << rank << " ";
    (*file) << "d_p" << serp.domains[rank] << "_" << rank << " ";
    for (int i = 0; i < serp.terms.size(); ++i)
    {
        (*file) << "u_t" << i << "_" << serp.urls[rank] << "_" << serp.terms[i] << " ";
        (*file) << "d_t" << i << "_" << serp.domains[rank] << "_" << serp.terms[i] << " ";
    }

    Url_type u = users.Get(serp.person, serp.urls[rank]);
    (*file) << "sH:" << u.skipped << " ";
    (*file) << "mH:" << u.missed << " ";
    (*file) << "cH:" << u.clicked << " ";
    (*file) << "c1H:" << u.clicked_1 << " ";
    (*file) << "c2H:" << u.clicked_2 << " ";

    Url_type u1 = Get(serp.urls[rank], local);
    (*file) << "sL:" << u1.skipped << " ";
    (*file) << "mL:" << u1.missed << " ";
    (*file) << "cL:" << u1.clicked << " ";
    (*file) << "c1L:" << u1.clicked_1 << " ";
    (*file) << "c2L:" << u1.clicked_2;
    for (int i = 0; i < SERP_SIZE; ++i)
    {
        for (int j = i+1; j < SERP_SIZE; ++j)
        {
            (*file) << " u_u" << serp.urls[i] << "_" << serp.urls[j];
        }
    }
    (*file) << std::endl;

}

void Get_counters_at_test_set()
{
    std::set<size_t> users;
    std::set<size_t> queries;
    {
        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(21) + ".txt");
        for (const auto &personData : data)
        {
            const unordered_map<size_t, Query> &sessionData = personData.second;
            for (const auto &session : sessionData)
            {
                const Query &serp = session.second;
                queries.insert(serp.id);
                users.insert(serp.person);

            }
        }
    }
    for (size_t i =  1; i <=  20; ++i)
    {
        DayData dayData = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        calculate_counters(dayData, counters1);
        DeleteSomeCounters(users, queries);
    }
}

BaseLine::BaseLine()
{
    Get_counters_at_test_set();

    vector<ofstream*> outs;
    for (size_t i = 0; i < 30; ++i)
    {
        string toOpen = DAY_DATA_FOLDER_VW + std::to_string(i) + ".txt";
        outs.emplace_back(new ofstream(toOpen));
    }
    int last_person = -1;
    int last_session = -1;
    int last_day = -1;
    std::unordered_map<int, Url_type> local;

    int enumerator = 0;
    {
        std::cout << "run reading file " << DATA_FILE << std::endl;
        vector<ofstream*> outs;
        for (size_t i = 0; i < 30; ++i)
        {
            string toOpen = DAY_DATA_FOLDER_VW + std::to_string(i) + ".txt";
            outs.emplace_back(new ofstream(toOpen, std::ios::binary | std::ios::out));
        }
        ifstream input(DATA_FILE, std::ios::binary | std::ios::in);
        size_t person, session, query, day, url, rank, domain;
        vector<size_t> terms(5,0);
        int type;
        size_t enumerator = 0;
        Query addedQuery;
        Query lastQuery;
        while(!input.eof())
        {
    //        std::cout << enumerator << "\n";
            input.read((char*)&person, sizeof(size_t));
            input.read((char*)&session, sizeof(size_t));
            input.read((char*)&query, sizeof(size_t));
            input.read((char*)&day, sizeof(size_t));
            input.read((char*)&url, sizeof(size_t));
            input.read((char*)&type, sizeof(int));
            input.read((char*)&rank, sizeof(size_t));
            input.read((char*)&domain, sizeof(size_t));
            /*std::cout << person << " " << session << " " << query << " " << day <<  " " << url << " " <<
                      type << " " <<  rank << " " << domain << std::endl;*/
            for (int i = 0; i < 5; ++i)
            {
                input.read((char*)&terms[i], sizeof(size_t));
            }

            addedQuery.id = query;
            addedQuery.session = session;
            addedQuery.person = person;
            addedQuery.urls[rank] = url;
            addedQuery.type[rank] = type;
            addedQuery.domains[rank] = domain;
            for (int i = 0; i < 5; ++i)
            {
                addedQuery.terms[i] = terms[i];
            }
            if ((enumerator) % 10 == 0 && enumerator > 0) {
                const Query &serp = lastQuery;
                if (enumerator % 10000 == 0)
                {
                    std::cout << enumerator << "\n";
                }
                if (true) {
                    /*std::cout << "QUERY " << serp.person << " " << serp.id << " " << serp.session << " ";
                    for (int i = 0 ; i < SERP_SIZE; ++i)
                    {
                        std::cout << serp.type[i] << " " << serp.urls[i] << " " << serp.domains[i] << " ";
                    }
                    std::cout << std::endl;*/
                    for (int i = 0; i < SERP_SIZE; ++i)
                    {
                        if (last_day == 21 && Filter4(counters1.query_user, counters1.user_url, counters1.query_rank, serp))
                        {
                            if (last_session != session) {
                                Write_Example(enumerator, i, local, serp, outs[last_day]);
                            }
                        }
                        if (last_day < 21 && Filter6(counters1.query_user, counters1.user_url, counters1.query_rank, serp))
                        {

                            Write_Example(enumerator, i, local, serp, outs[last_day]);
                        }
                    }
                    if (last_person != person)
                    {
                        users.data.clear();
                    }
                    if (last_session != session)
                    {

                        local.clear();
                    }
                }
                int last_click = 0;
                for (int i = 0; i < SERP_SIZE; ++i) {
                    if (serp.type[i] >= 1) {
                       last_click = i;
                    }
                }
                for (int i = 0; i < SERP_SIZE; ++i) {
                    users.data[serp.person];
                    users.data[serp.person][serp.urls[i]];
                    local[serp.urls[i]];
                    if (serp.type[i] < 1 && i > last_click) {
                        users.data[serp.person][serp.urls[i]].missed += 1;
                        local[serp.urls[i]].missed += 1;
                    }
                    if (serp.type[i] < 1 && i < last_click) {
                        users.data[serp.person][serp.urls[i]].skipped += 1;
                        local[serp.urls[i]].skipped += 1;
                    }
                    if (serp.type[i] < 0) {
                        users.data[serp.person][serp.urls[i]].clicked += 1;
                        local[serp.urls[i]].clicked += 1;
                    }
                    if (serp.type[i] == 1) {
                        users.data[serp.person][serp.urls[i]].clicked_1 += 1;
                        local[serp.urls[i]].clicked_1 += 1;
                    }
                    if (serp.type[i] == 2) {
                        users.data[serp.person][serp.urls[i]].clicked_2 += 1;
                        local[serp.urls[i]].clicked_2 += 1;
                    }
                }
                if (last_person != person)
                {
                    users.data.clear();
                }
                if (last_session != session)
                {

                    local.clear();
                }

            }
            enumerator += 1;
            lastQuery = addedQuery;
            last_person = person;
            last_session = session;
            last_day = day;
            //std::cout << lastQuery.urls[0] << "\n";


        }
    }
}

