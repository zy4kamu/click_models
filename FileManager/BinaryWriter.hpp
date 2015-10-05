#ifndef BINARY_WRITER
#define BINARY_WRITER

#include "Tools.h"

using std::string;
using std::wstring;

namespace FileManager
{

template <class _Writer>
class BinaryWriter
{
public:
    typedef _Writer Writer;

    BinaryWriter(shared_ptr<Writer> writer,
        std::locale writerLocale);
    Writer& GetWriter();
    BinaryWriter& operator <<(int data);
    BinaryWriter& operator <<(short data);
    BinaryWriter& operator <<(float data);
    BinaryWriter& operator <<(double data);
    BinaryWriter& operator <<(size_t data);
    BinaryWriter& operator <<(char data);
    BinaryWriter& operator <<(wchar_t data);
    BinaryWriter& operator <<(const string& data);
    BinaryWriter& operator <<(const wstring& data);
private:
    shared_ptr<Writer> writer;
    std::locale writerLocale;
};

template <class Writer>
BinaryWriter<Writer>::BinaryWriter(
    shared_ptr<Writer> writer,
    std::locale writerLocale)
    : writer(writer)
    , writerLocale(writerLocale)
{
}

template <class Writer>
typename BinaryWriter<Writer>::Writer&
BinaryWriter<Writer>::GetWriter()
{
    return *writer;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(int data)
{
    writer->write((char*)&data, sizeof(int));
    return *this;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(short data)
{
    writer->write((char*)&data, sizeof(short));
    return *this;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(double data)
{
    writer->write((char*)&data, sizeof(double));
    return *this;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(float data)
{
    writer->write((char*)&data, sizeof(float));
    return *this;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(size_t data)
{
    writer->write((char*)&data, sizeof(size_t));
    return *this;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(char data)
{
    writer->write((char*)&data, sizeof(char));
    return *this;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(wchar_t data)
{
    writer->write((char*)&data, sizeof(wchar_t));
    return *this;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(const string& data)
{
    (*this) << short(data.size());
    if (data.size() > 0)
    {
        writer->write((char*)data.c_str(), data.size() * sizeof(char));
    }
    return *this;
}

template <class Writer>
BinaryWriter<Writer>&
BinaryWriter<Writer>::operator <<(const wstring& data)
{
    if (data.size() > 0) {
        string converted;
        Tools::ConvertWstringToUTF8(data, converted,
            writerLocale);
        *this << converted;
    } else {
        *this << short(0);
    }
    return *this;
}


}

#endif // BINARY_WRITER
