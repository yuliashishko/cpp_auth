#include "handler_factory.h"
#include "handler.h"

RequestHandlerFactory::RequestHandlerFactory(const JwtSigner& signer): signer(signer) {}

Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &) {
    return new RequestHandler(signer);
}
