#include <iostream>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "Poco/Logger.h"
#include "jwt_signer.h"

#ifndef JWT_AUTH_CLION_HANDLER_H
#define JWT_AUTH_CLION_HANDLER_H


class RequestHandler : public Poco::Net::HTTPRequestHandler
{
private:
    JwtSigner signer;

public:
    RequestHandler(const JwtSigner& signer);

    void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp) override;
};

#endif //JWT_AUTH_CLION_HANDLER_H
