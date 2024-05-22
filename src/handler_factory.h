#include <iostream>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include "jwt_signer.h"

#ifndef JWT_AUTH_CLION_HANDLER_FACTORY_H
#define JWT_AUTH_CLION_HANDLER_FACTORY_H


class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
private:
    JwtSigner signer;

public:
    RequestHandlerFactory(const JwtSigner& signer);

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &) override;
};


#endif //JWT_AUTH_CLION_HANDLER_FACTORY_H
