#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

class LinguisticTools
{
public:
	LinguisticTools(void);
	~LinguisticTools(void);
	std::vector<std::wstring> tokenize(const std::wstring& input);
	std::vector<std::wstring> split(std::wstring line, std::wstring delim);
	std::unordered_set<wchar_t> punctuation;
private:
	void init();
};

