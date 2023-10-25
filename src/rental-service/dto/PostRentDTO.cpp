#include "PostRentDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void PostRentDTO::FromJSON(const std::string &json)
{
    Json::Value value;
    Json::Reader reader;

    if (!reader.parse(json, value))
    {
        throw JsonParserException("can't parse RentDTO object");
    }

    paymentUid = value["paymentUid"].asString();
    carUid = value["carUid"].asString();
    dateFrom = value["dateFrom"].asString();
    dateTo = value["dateTo"].asString();
}

Json::Value GetJSON(const PostRentDTO &rent)
{
    Json::Value root;

    root["paymentUid"] = rent.paymentUid;
    root["carUid"] = rent.carUid;
    root["dateFrom"] = rent.dateFrom;
    root["dateTo"] = rent.dateTo;

    return root;
}

std::string PostRentDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}