#include <iostream>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/PostgreSQL/Connector.h"
#include <Poco/JSON/Parser.h>
#include "jwt-cpp/jwt.h"

#include "handler.h"
#include "uuid.h"
#include "model/user.h"
#include "hash.h"

using Poco::Data::Session;
using Poco::Data::Statement;

using namespace Poco::Data::Keywords;
using namespace Poco::Net;
using namespace std;

void error(Poco::Net::HTTPServerResponse &resp, const string& msg, HTTPResponse::HTTPStatus status) {
    resp.setStatus(status);
    resp.setContentType("application/json");
    ostream& out = resp.send();
    out << R"({"error": ")" << msg << R"("})" << endl;
    out.flush();
}

bool parseUserFromReq(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp, User& user) {
    try {
        istream &inputStream = req.stream();
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(inputStream);
        const auto& object = result.extract<Poco::JSON::Object::Ptr>();
        user.fromJson(object);
    } catch (Poco::JSON::JSONException& ex) {
        error(resp, "failed to parse json: " + ex.displayText(), Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        return false;
    }

    if (user.email.empty() || user.password.empty()) {
        error(resp, "email and password fields mustn't be empty", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        return false;
    }

    return true;
}

void registrate(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp) {
    if (req.getMethod() != "POST") {
        error(resp, "method not allowed", Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        return;
    }

    User user;
    if (!parseUserFromReq(req, resp, user)) {
        return;
    }

    Poco::Data::PostgreSQL::Connector::registerConnector();
    Session session(
            "PostgreSQL", "host=postgres port=5432 dbname=authdb user=admin password=admin"
    );

    user.password = sha256(user.password);

    Statement insert(session);
    insert << "INSERT INTO users VALUES($1, $2, $3, $4, $5, $6)",
            use(user.id), use(user.firstName), use(user.lastName), use(user.age), use(user.email), use(user.password);
    insert.execute();

    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    resp.setContentType("application/json");
    ostream& out = resp.send();
    out << R"({"id": ")" << user.id << R"("})" << endl;
    out.flush();
}

void login(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp, JwtSigner& signer) {
    if (req.getMethod() != "POST") {
        error(resp, "method not allowed", Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        return;
    }

    User user;
    if (!parseUserFromReq(req, resp, user)) {
        return;
    }

    Poco::Data::PostgreSQL::Connector::registerConnector();
    Session session(
            "PostgreSQL", "host=localhost port=5432 dbname=authdb user=admin password=admin"
    );

    auto password = user.password;
    Statement select(session);
    select << "SELECT id, password FROM users WHERE email=$1",
        into(user.id), into(user.password), use(user.email);
    select.execute();

    if (select.subTotalRowCount() != 1) {
        error(resp, "wrong password or email", Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        return;
    }

    if (sha256(password) != user.password) {
        error(resp, "wrong password or email", Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        return;
    }

    auto token = signer.signWithUserId(user.id);
    resp.set("Authorization", "Bearer " + token);
    resp.setStatus(Poco::Net::HTTPResponse::HTTP_NO_CONTENT);
    resp.send();
}

void info(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp, JwtSigner& signer) {
    if (req.getMethod() != "GET") {
        error(resp, "method not allowed", Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        return;
    }

    if (!req.has("Authorization")) {
        error(resp, "unauthorized", Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        return;
    }

    auto authHeader = req.get("Authorization");
    auto spacePos = authHeader.find(' ');
    if (spacePos == -1 || authHeader.substr(0, spacePos) != "Bearer") {
        error(resp, "unauthorized", Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        return;
    }

    string userId;
    auto token = authHeader.erase(0, spacePos + 1);
    try {
        userId = signer.verifyAndGetUserId(token);
    } catch (exception& ex) {
        error(resp, "unauthorized", Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        return;
    }

    Poco::Data::PostgreSQL::Connector::registerConnector();
    Session session(
            "PostgreSQL", "host=localhost port=5432 dbname=authdb user=admin password=admin"
    );

    User user;
    Statement select(session);
    select << "SELECT * FROM users WHERE id=$1",
        into(user.id), into(user.firstName), into(user.lastName), into(user.age), into(user.email), use(userId);
    if (select.done()) {
        error(resp, "user not found", Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    select.execute();

    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    resp.setContentType("application/json");
    ostream& out = resp.send();
    out << user.toJson();
    out.flush();
}

RequestHandler::RequestHandler(const JwtSigner& signer): signer(signer) {}

void RequestHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp) {
    try
    {
        if (req.getURI() == "/register") {
            registrate(req, resp);
        } else if (req.getURI() == "/login") {
            login(req, resp, signer);
        } else {
            info(req, resp, signer);
        }
    }
    catch (Poco::Exception& exc)
    {
        cout << exc.displayText() << endl;
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }
}
