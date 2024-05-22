#include <random>
#include <sstream>


#ifndef JWT_AUTH_CLION_UUID_H
#define JWT_AUTH_CLION_UUID_H


namespace uuid {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string generate_uuid_v4();
}


#endif //JWT_AUTH_CLION_UUID_H
