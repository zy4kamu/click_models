#include "Embedding.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cmath>
#include <queue>
#include "FileReader.h"


Embedding::Embedding(const string& file, size_t dimension)
    : dimension(dimension)
{
    this->readEmbedding(file + ".embedding");
    FileManager::Read(file + ".map", &this->tokenToIndex);
}

void Embedding::readEmbedding(const string& file)
{
    int fdIn;
    if ((fdIn = open(file.c_str(), O_RDONLY)) < 0)
    {
        perror("ReadBinaryFileToPointer: couldn't create file descriptor");
    }

    struct stat fileStatistics;
    if (fstat(fdIn, &fileStatistics) < 0)
    {
        perror("ReadBinaryFileToPointer: couldn't find file size");
    }
    size_t fileSize = fileStatistics.st_size;
    size_t numberOfDoubles = fileSize / sizeof(double);
    this->numberOfPoints = numberOfDoubles / dimension;
    this->embedding.resize(numberOfDoubles);

    void* source;
    if ((source = mmap(0, fileSize, PROT_READ, MAP_SHARED, fdIn, 0)) == MAP_FAILED)
    {
        perror("ReadBinaryFileToPointer: couldn't use mmap to map file to pointer");
    }

    memcpy(&(*this->embedding.begin()), source, fileSize);

    if (munmap(source, fileSize) < 0)
    {
        perror("ReadBinaryFileToPointer: couldn't munmap");
    }
    close(fdIn);
}

double Embedding::Distance(size_t first, size_t second)
{
    size_t firstPosition = this->tokenToIndex[first] * dimension;
    size_t secondPosition = this->tokenToIndex[second] * dimension;
    double distance = 0;
    for (size_t i = 0; i < dimension; ++i)
    {
        double diff = this->embedding[firstPosition + i] - this->embedding[secondPosition + i];
        distance += diff * diff;
    }
    return std::sqrt(distance);
}

vector<std::pair<size_t, double> > Embedding::GetNearest(size_t index, size_t number)
{
    typedef std::pair<double, size_t> Pair;
    std::priority_queue<Pair, vector<Pair>, std::less<Pair> > queue;
    for (const auto& kvp : this->tokenToIndex)
    {
        size_t pretendent = kvp.first;
        if (pretendent == index)
        {
            continue;
        }
        double dist = this->Distance(pretendent, index);
        if (queue.size() < number) {
            queue.push(std::pair<double, size_t>(dist, pretendent));
        } else if (dist < queue.top().first) {
            queue.pop();
            queue.push(std::pair<double, size_t>(dist, pretendent));
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

vector<std::pair<size_t, double> > Embedding::GetNearest(size_t index,
    size_t number, const unordered_set<size_t>& userList)
{
    typedef std::pair<double, size_t> Pair;
    std::priority_queue<Pair, vector<Pair>, std::less<Pair> > queue;
    for (size_t pretendent : userList)
    {
        if (pretendent == index)
        {
            continue;
        }
        double dist = this->Distance(index, pretendent);
        if (queue.size() < number) {
            queue.push(std::pair<double, size_t>(dist, pretendent));
        } else if (dist < queue.top().first) {
            queue.pop();
            queue.push(std::pair<double, size_t>(dist, pretendent));
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
