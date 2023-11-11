#include "ErrorMessageDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void ErrorMessageDTO::FromJSON(const std::string &json)
{
    Json::Value value;
    Json::Reader reader;

    if (!reader.parse(json, value))
    {
        throw JsonParserException("can't parse PersonDTO object");
    }

    message = value["message"].asString();
}

Json::Value GetJSON(const ErrorMessageDTO &car)
{
    Json::Value root;

    root["message"] = car.message;

    return root;
}

std::string ErrorMessageDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}