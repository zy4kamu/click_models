#ifndef EMBEDDING
#define EMBEDDING

#include <vector>
#include <unordered_map>
#include <string>

namespace collaborativefiltering
{

using std::vector;
using std::unordered_map;
using std::string;

class Embedding
{
public:
    Embedding(size_t dimension);
    void readDictionary(const string& file);
    void setPtr(double* ptr);
    void readEmbedding(const string& file);
    void initialize(const vector<size_t>& items, size_t dimension);
    void write(const string& file);
    double* get(size_t item);
    size_t size();
private:
    size_t numberOfPoints;
    size_t dimension;
    double* embedding;
    unordered_map<size_t, size_t> itemToIndex;
};

}

#endif
