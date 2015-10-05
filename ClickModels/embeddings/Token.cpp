#include "Token.h"

void Token::Add(double toAdd)
{
    std::lock_guard<std::mutex> lock(*blocker);
    for (size_t i = 0; i < embedding.size(); ++i)
    {
        embedding[i] += toAdd;
    }
}

void Token::Add(const vector<double>& toAdd)
{
    std::lock_guard<std::mutex> lock(*blocker);
    for (size_t i = 0; i < toAdd.size(); ++i)
    {
        embedding[i] += toAdd[i];
    }
}

void Token::Substract(const vector<double>& toSubstract)
{
    std::lock_guard<std::mutex> lock(*blocker);
    for (size_t i = 0; i < toSubstract.size(); ++i)
    {
        embedding[i] -= toSubstract[i];
    }
}

Token::Token()
    : blocker(new std::mutex())
{
}

Token::Token(Token&& tok) noexcept
    : index(tok.index)
    , embedding(std::move(tok.embedding))
    , blocker(std::move(tok.blocker))
{}
