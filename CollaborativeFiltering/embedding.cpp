#include "embedding.h"
#include "FileReader.h"
#include "FileWriter.h"
#include <random>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace collaborative_filtering
{

Embedding::Embedding(size_t dimension)
    : dimension(dimension)
{
}

void Embedding::readDictionary(const string& file)
{
    FileManager::Read(file + ".map", &this->itemToIndex);
    this->numberOfPoints = this->itemToIndex.size();
}

void Embedding::setPtr(double* ptr)
{
    this->embedding = ptr;
}

void Embedding::readEmbedding(const string& file)
{
    std::cout << "embedding::readEmbedding " << file << std::endl;
    int fdIn;
    if ((fdIn = open(file.c_str(), O_RDONLY)) < 0)
    {
        perror("embedding.cpp: couldn't create file descriptor");
    }

    struct stat fileStatistics;
    if (fstat(fdIn, &fileStatistics) < 0)
    {
        perror("embedding.cpp: couldn't find file size");
    }
    size_t fileSize = fileStatistics.st_size;

    void* source;
    if ((source = mmap(0, fileSize, PROT_READ, MAP_SHARED, fdIn, 0)) == MAP_FAILED)
    {
        perror("embedding.cpp: couldn't use mmap to map file to pointer");
    }

    memcpy(this->embedding, source, fileSize);
    std::cout << "embedding: first element " << this->embedding[0] << std::endl;
    std::cout << "embedding: last element " << this->embedding[this->numberOfPoints - 1] << std::endl;

    if (munmap(source, fileSize) < 0)
    {
        perror("embedding.cpp: couldn't munmap");
    }
    close(fdIn);
}

void Embedding::initialize(const vector<size_t>& items, size_t dimension)
{
    this->numberOfPoints = items.size();
    this->dimension = dimension;
    for (size_t item : items)
    {
        this->itemToIndex[item] = this->itemToIndex.size();
    }
    std::random_device device;
    std::uniform_real_distribution<double> distribution(0, 1);
    for (size_t i = 0; i < items.size(); ++i)
    {
        if (i % 10000 == 0)
        {
            std::cout << "embedding.cpp: initialize " <<
                i << " of " << items.size();
        }
        size_t start = dimension * i;
        double sum = 0;
        for (size_t j = 0; j < dimension; ++j)
        {
            double rand = distribution(device);
            embedding[start + j] = rand;
            sum += rand;
        }
        for (size_t j = 0; j < dimension; ++j)
        {
            embedding[start + j] /= sum;
        }
        std::cout << "\r";
    }
    std::cout << std::endl;
}

void Embedding::write(const string& file)
{
    ofstream out(file + ".embedding", ios::binary);
    out.write((char*)this->embedding, this->numberOfPoints * this->dimension * sizeof(double));
    out.close();
    FileManager::Write(file + ".map", this->itemToIndex);
}

double* Embedding::get(size_t item)
{
    auto found = this->itemToIndex.find(item);
    if (found == this->itemToIndex.end()) {
        std::cout << "embedding not found item ..." << std::endl;
        return 0;
    } else {
        return embedding + this->dimension * found->second;
    }
}

size_t Embedding::size()
{
    return this->numberOfPoints;
}

}
