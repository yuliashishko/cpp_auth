#include <vector>
#include <iostream>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Util/ServerApplication.h>

#include "jwt_signer.h"

#ifndef JWT_AUTH_CLION_APP_H
#define JWT_AUTH_CLION_APP_H

class AuthApp : public Poco::Util::ServerApplication
{
private:
    JwtSigner signer;

protected:
    int main(const std::vector<std::string> &) override;

public:
    AuthApp(JwtSigner signer);
};


#endif //JWT_AUTH_CLION_APP_H
