#include <iostream>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Util/ServerApplication.h>

#include "handler_factory.h"
#include "app.h"

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

AuthApp::AuthApp(JwtSigner signer): signer(std::move(signer)) {}

int AuthApp::main(const std::vector<std::string> &) {
    HTTPServer s(new RequestHandlerFactory(signer), ServerSocket(9090), new HTTPServerParams);

    s.start();
    cout << endl << "Server started" << endl;

    waitForTerminationRequest();  // wait for CTRL-C or kill

    cout << endl << "Shutting down..." << endl;
    s.stop();

    return Application::EXIT_OK;
}
