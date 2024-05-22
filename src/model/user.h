#ifndef JWT_AUTH_CLION_USER_H
#define JWT_AUTH_CLION_USER_H


#include <Poco/JSON/Object.h>

class User {
public:
    std::string id;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string password;
    int age = 0;

    void fromJson(const Poco::JSON::Object::Ptr&);
    std::string toJson();
};


#endif //JWT_AUTH_CLION_USER_H
