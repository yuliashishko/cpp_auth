#include <iostream>
#include "user.h"
#include "../uuid.h"

using namespace std;

string withDefault(Poco::Nullable<string> value) {
    if (value.isNull()) {
        return "";
    }

    return value.value();
}

void User::fromJson(const Poco::JSON::Object::Ptr& object) {
    id = uuid::generate_uuid_v4();
    firstName = withDefault(object->getNullableValue<std::string>("first_name"));
    lastName = withDefault(object->getNullableValue<std::string>("last_name"));
    auto nullAge = object->getNullableValue<int>("age");
    if (!nullAge.isNull()) {
        age = nullAge.value();
    }
    email = withDefault(object->getNullableValue<std::string>("email"));
    password = withDefault(object->getNullableValue<std::string>("password"));
}

std::string User::toJson() {
    auto object = new Poco::JSON::Object;
    object->set("id", id);
    object->set("first_name", firstName);
    object->set("last_name", lastName);
    object->set("email", email);

    std::stringstream str;
    object->stringify(str);
    return str.str();
}
