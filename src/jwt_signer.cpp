#include <jwt-cpp/jwt.h>
#include "jwt_signer.h"
using namespace std;

JwtSigner::JwtSigner(
        string issuer,
        string id,
        int durationInSeconds,
        string publicKey,
        string privateKey
): issuer(std::move(issuer)),
    id(std::move(id)),
    durationInSeconds(durationInSeconds),
    publicKey(std::move(publicKey)),
    privateKey(std::move(privateKey)) {}

string JwtSigner::signWithUserId(const string& userId) {
    auto algorithm = jwt::algorithm::es256k(publicKey, privateKey, "", "");

    return jwt::create()
            .set_issuer(issuer)
            .set_type("JWT")
            .set_id(id)
            .set_issued_at(std::chrono::system_clock::now())
            .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{durationInSeconds})
            .set_payload_claim("user-id", jwt::claim(userId))
            .sign(algorithm);
}

string JwtSigner::verifyAndGetUserId(const string& token) {
    auto verify = jwt::verify()
            .allow_algorithm(jwt::algorithm::es256k(publicKey, privateKey, "", ""))
            .with_issuer(issuer);

    auto decoded = jwt::decode(token);
    verify.verify(decoded);

    return decoded.get_payload_json().at("user-id").to_str();
}
