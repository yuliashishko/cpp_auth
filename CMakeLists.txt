cmake_minimum_required(VERSION  3.25.1)
project(untitled)

#find_package(jwt-cpp REQUIRED)
#find_package(Poco REQUIRED Net Util Data JSON)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled src/main.cpp
        src/handler.cpp
        src/handler.h
        src/handler_factory.cpp
        src/handler_factory.h
        src/app.cpp
        src/app.h
        src/jwt_signer.cpp
        src/jwt_signer.h
        src/uuid.cpp
        src/uuid.h
        src/model/user.cpp
        src/model/user.h
        src/hash.cpp
        src/hash.h
)
include_directories(
#        third_party/openssl-3.3.0/include
#        C:\\Program Files\\FireDaemonOpenSSL3\\include
        third_party/jwt-cpp/include
        third_party/poco/Net/include
        third_party/poco/Util/include
        third_party/poco/Data/include
        third_party/poco/JSON/include
        third_party/poco/Data/PostgreSQL/include
)
#add_subdirectory(third_party/openssl-3.3.0)
add_subdirectory(third_party/jwt-cpp)
add_subdirectory(third_party/poco/)
target_link_libraries(untitled jwt-cpp::jwt-cpp)
target_link_libraries(untitled Poco::Util Poco::Net Poco::Data Poco::JSON DataPostgreSQL)


#set(ALL_LIBS jwt-cpp Net Util Data JSON)
#target_link_libraries(untitled ${ALL_LIBS})