#include "hash.h"
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using namespace std;

string sha256(const string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;

    for (unsigned char i : hash)
    {
        ss << hex << setw(2) << setfill('0') << (int)i;
    }

    return ss.str();
}
