#include "Tools.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <thread>

using std::wcout;
using std::wcin;
using std::locale;
using std::string;

//class SimpleServer : public Tools::ProcessServer<Tools::PythonFloatSizeConnecter<> >
//{
//public:
//    SimpleServer(const string& file) : Tools::ProcessServer<Tools::PythonFloatSizeConnecter<> >(file){}
//    string GetResponce(const string& request) const
//    {
//        return "Recieved " + request;
//    }
//};
//
//void RunServer()
//{
//    SimpleServer server("/tmp/socket");
//    server.Start();
//}
//
//void RunClient(const string& message)
//{
//    try {
//        Tools::ProcessClient<Tools::PythonFloatSizeConnecter<> > client("/tmp/socket");
//        for (size_t i = 0; i < 10; ++i)
//        {
//            client.Send(message);
//            std::cout << client.Get() << std::endl;
//        }
//        client.Close();
//    } catch (...) {
//        std::cout << "Client error;" << std::endl;
//        return;
//    }
//}

int main()
{
    Tools::PrepareConsole();
}
