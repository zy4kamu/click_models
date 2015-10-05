#ifndef CONSOLE_TOOLS
#define CONSOLE_TOOLS

#include <string>

using std::string;
using std::wstring;

namespace Tools
{

/**
 * @brief adds russian (cyrillic) characters to console
 * @param [in] string input wstring
 * @remark outputs string character by character, using russian locale. If meets bad character, skips it, and clears badBit state to make console valid again
 */
void AddToConsole( wstring string );

string ExecuteShellScript(const string& script);

}

#endif // CONSOLE_TOOLS
