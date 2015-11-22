#include <iostream>
#include <string>
#include <unordered_set>

#include "collaborative_filtering.h"
#include "day_data.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "macro_parameters.h"
#include "learner.h"

using std::string;
using std::unordered_set;

using collaborative_filtering::CollaborativeFiltering;
using collaborative_filtering::Learner;

void getUserDocQueries()
{
    unordered_set<size_t> users, queries, docs;
    for (size_t i = FIRST_TRAINING_DAY; i <= LAST_TRAINING_DAY; ++i)
    {
        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                queries.insert(serp.id);
                users.insert(serp.person);
                for (size_t url : serp.urls) docs.insert(url);
            }
        }
    }
    vector<size_t> userVec(users.begin(), users.end());
    std::sort(userVec.begin(), userVec.end());
    FileManager::Write(INPUT_FOLDER + "users", userVec);

    vector<size_t> queryVec(queries.begin(), queries.end());
    std::sort(queryVec.begin(), queryVec.end());
    FileManager::Write(INPUT_FOLDER + "queries", queryVec);

    vector<size_t> docVec(docs.begin(), docs.end());
    std::sort(docVec.begin(), docVec.end());
    FileManager::Write(INPUT_FOLDER + "docs", docVec);
}

void prepareCollaborativeFiltering()
{
    CollaborativeFiltering processor(DIMENSION);
    vector<size_t> users, queries, docs;
    std::cout << "reading users ..." << std::endl;
    FileManager::Read(INPUT_FOLDER + "users", &users);
    std::cout << "reading queries ..." << std::endl;
    FileManager::Read(INPUT_FOLDER + "queries", &queries);
    std::cout << "reading docs ..." << std::endl;
    FileManager::Read(INPUT_FOLDER + "docs", &docs);
    std::cout << "initializing ..." << std::endl;
    processor.initialize(users, queries, docs, DIMENSION);
    std::cout << "writing to file ..." << std::endl;
    processor.write(INITIAL_MODEL_PARAMETERS_FOLDER);
}

void getLogLikelihood()
{
    CollaborativeFiltering processor(
        INITIAL_MODEL_PARAMETERS_FOLDER, DIMENSION);
    double sumLikelihood = 0;
    size_t numQueries = 0;
    for (size_t i = FIRST_TRAINING_DAY; i <= LAST_TRAINING_DAY; ++i)
    {
        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                sumLikelihood += processor.calculateLogLikelihood(serp);
                ++numQueries;
            }
        }
        double likelihood = sumLikelihood / numQueries;
        std::cout << "likelihood = " << likelihood << std::endl;
    }
}

void calculateUQD()
{
    unordered_set<string> uqd;
    unordered_set<size_t> users, queries, docs;
    for (size_t i = FIRST_TRAINING_DAY; i <= LAST_TRAINING_DAY; ++i)
    {
        DayData data = read_day(DAY_DATA_FOLDER + std::to_string(i) + ".txt");
        for (const auto& personData : data)
        {
            const unordered_map<size_t, Query>& sessionData = personData.second;
            for (const auto& session : sessionData)
            {
                const Query& serp = session.second;
                size_t user = serp.person;
                size_t query = serp.id;
                string uq = std::to_string(user) + "_" + std::to_string(query) + "_";
                for (size_t doc : serp.urls)
                {
                    uqd.insert(uq + std::to_string(doc));
                }
            }
        }
        std::cout << "number of uqd = " << uqd.size() << std::endl;
    }
}

void learn()
{
    Learner learner(INITIAL_MODEL_PARAMETERS_FOLDER, DIMENSION);
    learner.learn();
}

int main()
{
    //getUserDocQueries();
    //prepareCollaborativeFiltering();
    //getLogLikelihood();
    //calculateUQD();
    learn();
}

