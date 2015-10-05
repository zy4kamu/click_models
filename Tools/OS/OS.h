#ifndef OS_TOOLS
#define OS_TOOLS

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Tools
{

size_t GetNumberOfFiles(const string& folder);

vector<string> GetFiles(const string& folder);

bool FileExists(const string& file);

int RemoveFile(const string& file);

}

#endif // OS_TOOLS
