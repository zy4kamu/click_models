#ifndef PROCESS_CLIENT
#define PROCESS_CLIENT

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

using std::string;

static const string RESPONSE_ERROR_MESSAGE = "bad responce message";
static const string END_SESSION_STRING = "end session";

template<class Connector>
class ProcessClient
{
public:
    ProcessClient(const string& file, size_t numberOfTrials = size_t(-1));
    ~ProcessClient();
    void Send(const string& request);
    void Close();
    string Get();
private:
    Connector connector;
    int socketIndex;
};

template<class Connector>
ProcessClient<Connector>::ProcessClient(const string& file, size_t numberOfTrials)
{
    socketIndex = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socketIndex < 0)
    {
        throw ProcessClientCreationException();
    }
    sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, file.c_str());
    int result = -1;
    int enumerator = 0;
    while (result == -1)
    {
        result = connect(socketIndex, (sockaddr*)&address, sizeof(address));
        ++enumerator;
        if (numberOfTrials != size_t(-1) && enumerator > numberOfTrials) {
            throw ProcessClientCreationException();
        }
    }
}

template<class Connector>
void ProcessClient<Connector>::Send(const string& request)
{
    try {
        connector.Send(request, socketIndex);
    } catch(...) {
        close(socketIndex);
    }
}

template<class Connector>
string ProcessClient<Connector>::Get()
{
    string recieved = connector.Get(socketIndex);
    if (recieved == RESPONSE_ERROR_MESSAGE)
    {
        this->Close();
    }
    return recieved;
}

template<class Connector>
void ProcessClient<Connector>::Close()
{
    this->Send(END_SESSION_STRING);
    close(socketIndex);
}

template<class Connector>
ProcessClient<Connector>::~ProcessClient()
{
    close(socketIndex);
}

}
#endif //MSVC
#endif // PROCESS_CLIENT
