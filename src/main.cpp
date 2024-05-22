#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/kazuho-picojson/defaults.h>
#include <fstream>
#include "app.h"
using namespace std;

//void encode_decode() {
//    // openssl ecparam -name secp256k1 -genkey -noout -out ec-secp256k1-priv-key.pem
//    std::string es256k_priv_key = R"(-----BEGIN EC PRIVATE KEY-----
//MHQCAQEEIArnQWnspKtjiVuZuzuZ/l1Uqqq8gb2unLJ/6U/Saf4ioAcGBSuBBAAK
//oUQDQgAEfy03KCKUpIPMIJBtIG4xOwGm0Np/yHKaK9EDZi0mZ7VUeeNKq476CU5X
//940yusahgneePQrDMF2nWFEtBCOiXQ==
//-----END EC PRIVATE KEY-----)";
//    // openssl ec -in ec-secp256k1-priv-key.pem -pubout > ec-secp256k1-pub-key.pem
//    std::string es256k_pub_key = R"(-----BEGIN PUBLIC KEY-----
//MFYwEAYHKoZIzj0CAQYFK4EEAAoDQgAEfy03KCKUpIPMIJBtIG4xOwGm0Np/yHKa
//K9EDZi0mZ7VUeeNKq476CU5X940yusahgneePQrDMF2nWFEtBCOiXQ==
//-----END PUBLIC KEY-----)";
//
//    auto token = jwt::create()
//            .set_issuer("auth0")
//            .set_type("JWT")
//            .set_id("es256k-create-example")
//            .set_issued_at(std::chrono::system_clock::now())
//            .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{36000})
//            .set_payload_claim("sample", jwt::claim(std::string{"test"}))
//            .sign(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key, "", ""));
//
//    auto verify = jwt::verify()
//            .allow_algorithm(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key, "", ""))
//            .with_issuer("auth0");
//
//    auto decoded = jwt::decode(token);
//
//    verify.verify(decoded);
//
//    cout << decoded.get_payload_json().at("sample2") << endl;
//
//    std::cout << "token:\n" << token << std::endl;
//    for (auto &e: decoded.get_header_json())
//        std::cout << e.first << " = " << e.second << std::endl;
//    for (auto &e: decoded.get_payload_json())
//        std::cout << e.first << " = " << e.second << std::endl;
//}

int main(int argc, char** argv)
{
    ifstream privateKeyFile("privatekey.pem");
    ifstream publicKeyFile("publickey.pem");

    std::string privateKey((std::istreambuf_iterator<char>(privateKeyFile)), std::istreambuf_iterator<char>());
    std::string publicKey((std::istreambuf_iterator<char>(publicKeyFile)), std::istreambuf_iterator<char>());

    JwtSigner signer("localhost", "jwt_auth_app", 3600, publicKey, privateKey);

    AuthApp app(signer);
    return app.run(argc, argv);
}
