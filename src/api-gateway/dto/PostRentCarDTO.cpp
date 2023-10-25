#include "PostRentCarDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void PostRentCarDTO::FromJSON(const std::string &json)
{
    Json::Value value;
    Json::Reader reader;

    if (!reader.parse(json, value))
    {
        throw JsonParserException("can't parse PersonDTO object");
    }

    carUid = value["carUid"].asString();
    dateFrom = value["dateFrom"].asString();
    dateTo = value["dateTo"].asString();
}

Json::Value GetJSON(const PostRentCarDTO &rentCar)
{
    Json::Value root;

    root["carUid"] = rentCar.carUid;
    root["dateFrom"] = rentCar.dateFrom;
    root["dateTo"] = rentCar.dateTo;

    return root;
}

std::string PostRentCarDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}