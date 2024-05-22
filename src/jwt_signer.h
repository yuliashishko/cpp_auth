#include <iostream>

#ifndef JWT_AUTH_CLION_JWT_SIGNER_H
#define JWT_AUTH_CLION_JWT_SIGNER_H


class JwtSigner {
private:
    std::string issuer;
    std::string id;
    int durationInSeconds;

    // openssl ecparam -name secp256k1 -genkey -noout -out ec-secp256k1-priv-key.pem
    std::string privateKey;

    // openssl ec -in ec-secp256k1-priv-key.pem -pubout > ec-secp256k1-pub-key.pem
    std::string publicKey;

public:
    JwtSigner(
            std::string issuer,
            std::string id,
            int durationInSeconds,
            std::string publicKey,
            std::string privateKey
    );

    std::string signWithUserId(const std::string& userId);
    std::string verifyAndGetUserId(const std::string& token);
};


#endif //JWT_AUTH_CLION_JWT_SIGNER_H
