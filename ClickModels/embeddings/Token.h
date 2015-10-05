#ifndef TOKEN
#define TOKEN
#include "utils.h"
#include <mutex>

struct Token
{
    size_t index;
    vector<double> embedding;
    std::unique_ptr<std::mutex> blocker;

    Token();
    Token(const Token& token) = delete;
    Token(Token&& token) noexcept;
    Token& operator = (const Token& token) = delete;
    Token& operator = (Token&& token) = default;

    void Add(double toAdd);
    void Add(const vector<double>& toAdd);
    void Substract(const vector<double>& toSubstract);
};

#endif
