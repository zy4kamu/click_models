#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;

class Embedding
{
public:
    Embedding(const string& file, size_t dimension);
    double Distance(size_t first, size_t second);
    vector<std::pair<size_t, double> > GetNearest(size_t index, size_t number);
    vector<std::pair<size_t, double> > GetNearest(size_t index,
        size_t number, const unordered_set<size_t>& userList);
private:
    void readEmbedding(const string& file);

    size_t dimension;
    size_t numberOfPoints;
    vector<double> embedding;
    unordered_map<size_t, size_t> tokenToIndex;
};
