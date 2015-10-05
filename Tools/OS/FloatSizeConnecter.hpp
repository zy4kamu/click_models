#ifndef FLOAT_SIZE_CONNECTER
#define FLOAT_SIZE_CONNECTER

#ifdef MSVC

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <iostream>
#include <string>

namespace Tools
{

template<class SyzeType = int>
class FloatSizeConnecter
{
public:
    string Get(int connectionIndex)
    {
        try {
            SyzeType bufferSize;
            read(connectionIndex, &bufferSize, sizeof(SyzeType));

            char message[bufferSize + 1];
            memset(message, 0, bufferSize + 1);
            read(connectionIndex, message, bufferSize);
            return string(message);
        } catch(...) {
            throw ProcessServerConnectionException();
        }
    }

    void Send(const string& responce, int connectionIndex)
    {
        SyzeType size = static_cast<SyzeType>(responce.size());
        write(connectionIndex, &size, sizeof(SyzeType));
        write(connectionIndex, responce.c_str(), responce.size());
    }
};

}
#endif

#endif // FLOAT_SIZE_CONNECTER

