#include "baseline.h"

static const std::string OUT_DIRECTORY =  "/Users/annasepliaraskaia/Desktop/work/";
static const std::string DAY_DATA_FOLDER = OUT_DIRECTORY + "data_by_days/";
static const std::string DAY_DATA_FOLDER_VW = OUT_DIRECTORY + "data_by_days_VW/";
static const int SERP_SIZE = 10;

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
    (*file) << label << " " << 1. << " '" <<enumerator << " | ";
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

BaseLine::BaseLine()
{
    vector<ofstream*> outs;
    for (size_t i = 0; i < 30; ++i)
    {
        string toOpen = DAY_DATA_FOLDER_VW + std::to_string(i) + ".txt";
        outs.emplace_back(new ofstream(toOpen));
    }
    int last_person = -1;
    int last_session = -1;
    std::unordered_map<int, Url_type> local;
    int enumerator = 0;
    for (size_t day =  1; day <=  2; ++day)
    {
        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(day) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                if (enumerator % 1000000 == 0)
                {
                    std::cout << enumerator << "\n";
                }
                if (last_person != serp.person || last_session != serp.session)
                {
                    local.clear();
                }
                for (int i = 0; i < SERP_SIZE; ++i)
                {
                    ++enumerator;
                    Write_Example(enumerator, i,local,serp, outs[day]);
                }
                for (int i = 0; i < SERP_SIZE; ++i)
                {
                    users.data[serp.person];
                    users.data[serp.person][serp.urls[i]];
                    local[serp.urls[i]];
                    if (serp.type[i] == -2)
                    {
                        users.data[serp.person][serp.urls[i]].missed += 1;
                        local[serp.urls[i]].missed += 1;
                    }
                    if (serp.type[i] == -1)
                    {
                        users.data[serp.person][serp.urls[i]].skipped += 1;
                        local[serp.urls[i]].skipped += 1;
                    }
                    if (serp.type[i] == 0)
                    {
                        users.data[serp.person][serp.urls[i]].clicked += 1;
                        local[serp.urls[i]].clicked += 1;
                    }
                    if (serp.type[i] == 1)
                    {
                        users.data[serp.person][serp.urls[i]].clicked_1 += 1;
                        local[serp.urls[i]].clicked_1 += 1;
                    }
                    if (serp.type[i] == 2)
                    {
                        users.data[serp.person][serp.urls[i]].clicked_2 += 1;
                        local[serp.urls[i]].clicked_2 += 1;
                    }
                }
                last_person = serp.person;
                last_session = serp.session;


            }
        }
    }
}

