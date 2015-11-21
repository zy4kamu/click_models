#include <iostream>
#include <string>
#include <unordered_set>

#include "collaborative_filtering.h"
#include "day_data.h"
#include "FileReader.h"
#include "FileWriter.h"

using std::string;
using std::unordered_set;

using collaborative_filtering::CollaborativeFiltering;

const string DAY_DATA_FOLDER = "/home/stepan/click_models_data/data_by_days/";
const string INPUT_FOLDER = "/home/stepan/click_models_data/collaborative_filtering/input/";
const string MODEL_PARAMETERS_FOLDER = "/home/stepan/click_models_data/collaborative_filtering/model_parameters/";
const size_t DIMENSION = 20;

void getUserDocQueries()
{
    unordered_set<size_t> users, queries, docs;
    for (size_t i = 1; i <= 27; ++i)
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
    CollaborativeFiltering processor;
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
    processor.write(MODEL_PARAMETERS_FOLDER);
}

int main()
{
    //getUserDocQueries();
    prepareCollaborativeFiltering();
}

