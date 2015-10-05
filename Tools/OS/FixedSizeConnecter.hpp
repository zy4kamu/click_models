#ifndef FIXED_SIZE_CONNECTER
#define FIXED_SIZE_CONNECTER

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

template<size_t bufferSize = 256>
class FixedSizeConnecter
{
public:
    string Get(int connectionIndex)
    {
        try {
            char message[bufferSize];
            memset(message, 0, bufferSize);
            read(connectionIndex, message, bufferSize);
            return string(message);
        } catch(...) {
            throw ProcessServerConnectionException();
        }
    }

    void Send(const string& responce, int connectionIndex)
    {
        write(connectionIndex, responce.c_str(), responce.size());
    }
};

}
#endif

#endif // FIXED_SIZE_CONNECTER
