#include <vector>
#include <string>
#include <unordered_map>

using std::vector;
using std::string;
using std::unordered_map;

class Embedding
{
public:
    Embedding(const string& file, size_t dimension);
    double Distance(size_t first, size_t second);
private:
    void readEmbedding(const string& file);

    size_t dimension;
    size_t numberOfPoints;
    vector<double> embedding;
    unordered_map<size_t, size_t> tokenToIndex;
};
