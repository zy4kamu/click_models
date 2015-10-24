#include "MyLearner.h"
#include <queue>
#include <cmath>
#include "FileWriter.h"
#include "FileReader.h"
#include "uumap.h"

double Dist(const vector<double>& first, const vector<double>& second)
{
    double sum = 0;
    for (size_t i = 0; i < first.size(); ++i)
    {
        double diff = first[i] - second[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

MyLearner::MyLearner(
    const string& usersFile,
    std::function<double(double)> probFunctor,
    std::function<double(double)> divLogFunctor,
    size_t dimension)
    : probFunctorCalculator(probFunctor, 0, 10, 0.001)
    , divLogFunctorCalculator(divLogFunctor, 0, 10, 0.001)
    , dimension(dimension)
    , uniformDistribution(0,1)
{
    size_t enumerator = 0;
    vector<size_t> users;
    FileManager::Read(usersFile, &users);
    for (size_t user : users)
    {
        if (++enumerator % 100000 == 0)
        {
            std::cout << "MyLearner reading file " << usersFile <<
                "; iteration = " << enumerator << std::endl;
        }
        tokenToIndex[user] = tokens.size();
        tokens.emplace_back();
        tokens.back().index = user;
        tokens.back().embedding.resize(dimension);
        for (double& item : tokens.back().embedding)
        {
            item = uniformDistribution(generator);
        }
    }
}

//MyLearner::MyLearner(const string& file)
//    : probFunctorCalculator([](double x) {return 0;}, 0, 1, 0.1)
//    , divLogFunctorCalculator([](double x) {return 0;}, 0, 1, 0.1)
//{
//    uumap uumap_data(file);
//    const unordered_map<size_t, vector<double> >& data = uumap_data.watch(0);
//    for (auto& item : data)
//    {
//        tokenToIndex[item.first] = tokens.size();
//        tokens.emplace_back();
//        tokens.back().index = item.first;
//        tokens.back().embedding = std::move(item.second);
//    }
//}

//vector<std::pair<size_t, double> > MyLearner::GetNearest(size_t index, size_t number)
//{
//    typedef std::pair<double, size_t> Pair;
//    std::priority_queue<Pair, vector<Pair>, std::less<Pair> > queue;
//    Token& token = tokens[tokenToIndex[index]];
//    for (Token& pretendent : tokens)
//    {
//        double dist = Dist(token.embedding, pretendent.embedding);
//        if (queue.size() < number) {
//            queue.push(std::pair<double, size_t>(dist, pretendent.index));
//        } else if (dist < queue.top().first) {
//            queue.pop();
//            queue.push(std::pair<double, size_t>(dist, pretendent.index));
//        }
//    }
//    vector<std::pair<size_t, double> > nearest;
//    while(!queue.empty())
//    {
//        nearest.emplace_back(queue.top().second, queue.top().first);
//        queue.pop();
//    }
//    return nearest;
//}

double MyLearner::Distance(size_t first, size_t second)
{
    return Dist(tokens[tokenToIndex[first]].embedding, tokens[tokenToIndex[second]].embedding);
}

vector<std::pair<size_t, double> > MyLearner::GetNearest(size_t index,
    size_t number, const unordered_set<size_t>& userList)
{
    typedef std::pair<double, size_t> Pair;
    std::priority_queue<Pair, vector<Pair>, std::less<Pair> > queue;
    Token& token = tokens[tokenToIndex[index]];
    for (size_t userIndex : userList)
    {
        if (userIndex == index)
        {
            continue;
        }
        Token& pretendent = tokens[tokenToIndex[userIndex]];
        double dist = Dist(token.embedding, pretendent.embedding);
        if (queue.size() < number) {
            queue.push(std::pair<double, size_t>(dist, pretendent.index));
        } else if (dist < queue.top().first) {
            queue.pop();
            queue.push(std::pair<double, size_t>(dist, pretendent.index));
        }
    }
    vector<std::pair<size_t, double> > nearest;
    while(!queue.empty())
    {
        nearest.emplace_back(queue.top().second, queue.top().first);
        queue.pop();
    }
    return nearest;
}

void MyLearner::Print(const string& file)
{
    vector<double> concatenated(this->dimension * this->tokens.size());
    for (size_t i = 0; i < this->tokens.size(); ++i)
    {
        const vector<double>& emb = this->tokens[i].embedding;
        size_t position = i * this->dimension;
        for (size_t j = 0; j < dimension; ++j)
        {
            concatenated[position + j] = emb[j];
        }
    }
    FILE* writer = fopen((file + ".embedding").c_str(), "wb");
    fwrite(&(*concatenated.begin()), sizeof(double), concatenated.size(), writer);
    fclose(writer);
    FileManager::Write(file + ".map", this->tokenToIndex);
}

void MyLearner::MakeOnePositiveStep(size_t firstHash, size_t secondHash, double rate)
{
    Token& first = tokens[tokenToIndex[firstHash]];
    Token& second = tokens[tokenToIndex[secondHash]];
    double distance = Dist(first.embedding, second.embedding);
    double coeff = rate * this->divLogFunctorCalculator.Get(distance);
    coeff /= 0.0001 + distance;
    vector<double> toAdd(dimension);
    for (size_t i = 0; i < dimension; ++i)
    {
        toAdd[i] = rate * coeff * (first.embedding[i] - second.embedding[i]);
        if (!std::isnormal(toAdd[i]))
        {
            std::cout << "";
        }
    }
    first.Add(toAdd);
    second.Substract(toAdd);
}

void MyLearner::MakeOneNegativeStep(size_t firstHash, size_t secondHash, double rate)
{
    Token& first = tokens[tokenToIndex[firstHash]];
    Token& second = tokens[tokenToIndex[secondHash]];
    double distance = Dist(first.embedding, second.embedding);
    double probDistance = this->probFunctorCalculator.Get(distance);
    double coeff = rate * this->divLogFunctorCalculator.Get(distance);
    coeff *= -probDistance;
    coeff /= 0.0001 + 1.0 - probDistance;
    coeff /= 0.0001 + distance;
    for (size_t i = 0; i < dimension; ++i)
    {
        double toAdd = rate * coeff * (first.embedding[i] - second.embedding[i]);
        if (!std::isnormal(toAdd))
        {
            std::cout << "";
        }
        first.embedding[i] += toAdd;
        second.embedding[i] -= toAdd;
    }
}
