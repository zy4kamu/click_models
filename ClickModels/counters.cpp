#include "counters.h"

Counters counters;

static string out_directory = "/Users/annasepliaraskaia/Desktop/work/";
//static string out_directory = "/home/anna/Рабочий стол/work/data/";

Counters::Counters()
    : query_url("/tmp/query_url")
    , user_url("/tmp/user_url")
    , user_rank("/tmp/user_rank")
    , query_user("/tmp/query_user")
    , query_rank("/tmp/query_rank")
{
}

void Counters::Set_query_url(uumap& query_url_)
{
    query_url = query_url_;
}
void Counters::Set_user_url(uumap& user_url_)
{
    user_url = user_url_;
}
void Counters::Set_user_rank(uumap& user_rank_)
{
    user_rank = user_rank_;
}
void Counters::Set_query_user(uumap& query_user_)
{
    query_user = query_user_;
}
void Counters::Set_query_rank(uumap& query_rank_)
{
    query_rank = query_rank_;
}

void Counters::clear()
{
    query_url.clear();
    user_url.clear();
    user_rank.clear();
    query_user.clear();
    query_rank.clear();
}

void Counters::save(const string& out_directory, size_t start_day, size_t finish_day)
{
    string postfix = "_" + std::to_string(start_day) + "_" + std::to_string(finish_day);

    query_url.save(out_directory + "query_url" + postfix);
    user_url.save(out_directory + "user_url" + postfix);
    user_rank.save(out_directory + "user_rank" + postfix);
    query_user.save(out_directory + "query_user" + postfix);
    query_rank.save(out_directory + "query_rank" + postfix);
}

void calculate_counters(const DayData& data, Counters& counters1)
{
    for(const auto& item0 : data)
    {
        for (const auto& item1 : item0.second)
        {
            const Query& query = item1.second;
            counters1.query_user.get(query.id, query.person, 1)[0] += 1;
            counters1.user_query.get(query.person, query.id, 1)[0] += 1;

            for (size_t i = 0; i <= query.type.size(); ++i)
            {
                if (query.type[i] == 2)
                {
                    counters1.user_url.get(query.person, query.urls[i], 1)[0] += 1;
                    counters1.query_rank.get(query.id, i, 1)[0] += 1;
                }
            }

            size_t maxIndex = 0;
            int maxElem = query.type[0];
            for (size_t i = 0; i < 10; ++i)
            {
                if (query.type[i] >= maxElem) {
                    maxIndex = i;
                    maxElem = query.type[i];
                }
            }
            if (maxElem != 2) continue;
            for (size_t i = 0; i <= maxIndex; ++i)
            {
                vector<double>& vec = counters1.query_url.get(query.id, query.urls[i], 4);
                vec[0] += 1;
                if (query.type[i] == 2) {
                    vec[1] += 1;
                } else {
                    vec[2] += 1;
                }
                if (i == maxIndex)
                {
                    vec[3] += 1;
                }
            }

            for (size_t i = 0; i <= maxIndex; ++i)
            {
                vector<double>& vec = counters1.user_rank.get(query.person, i, 4);
                vec[0] += 1;
                if (query.type[i] == 2) {
                    vec[1] += 1;
                } else {
                    vec[2] += 1;
                }
                if (i == maxIndex)
                {
                    vec[3] += 1;
                }
            }
        }
    }
}

void calculate_counters(const string& folder, size_t start_day, size_t finish_day)
{
    counters.clear();
    clock_t start, end;
    for(size_t i = start_day; i <= finish_day; ++i)
    {
        std::cout << "evaluating day " << i << std::endl;
        start = clock();
        string file = folder + std::to_string(i) + ".txt";
        DayData data = read_day(file);
        end = clock();
        std::cout << i << " reading: " << double(end - start) /  CLOCKS_PER_SEC << std::endl;
        start = clock();
        calculate_counters(data, counters);
        end = clock();
        std::cout << i << " calculations: " << double(end - start) /  CLOCKS_PER_SEC << std::endl;
    }
    counters.save(out_directory, start_day, finish_day);
}

