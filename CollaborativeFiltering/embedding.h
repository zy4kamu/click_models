#include <vector>
#include <unordered_map>
#include <string>

namespace collaborative_filtering
{

using std::vector;
using std::unordered_map;
using std::string;

class Embedding
{
public:
    Embedding();
    Embedding(const string& file, size_t dimension);
    void initialize(const vector<size_t>& items, size_t dimension);
    void write(const string& file);
    double* get(size_t item);
private:
    void readEmbedding(const string& file);

    size_t numberOfPoints;
    size_t dimension;
    vector<double> embedding;
    unordered_map<size_t, size_t> itemToIndex;
};

}
