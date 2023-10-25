#include "RentDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void RentDTO::FromJSON(const std::string &json)
{
    Json::Value value;
    Json::Reader reader;

    if (!reader.parse(json, value))
    {
        throw JsonParserException("can't parse RentDTO object");
    }

    id = value["id"].asUInt64();
    rentUid = value["rentalUid"].asString();
    username = value["username"].asString();
    paymentUid = value["paymentUid"].asString();
    carUid = value["carUid"].asString();
    dateFrom = value["dateFrom"].asString();
    dateTo = value["dateTo"].asString();
    status = value["status"].asString();
}

Json::Value GetJSON(const RentDTO &rent)
{
    Json::Value root;

    root["id"] = (uint)rent.id;
    root["rentalUid"] = rent.rentUid;
    root["username"] = rent.username;
    root["paymentUid"] = rent.paymentUid;
    root["carUid"] = rent.carUid;
    root["dateFrom"] = rent.dateFrom;
    root["dateTo"] = rent.dateTo;
    root["status"] = rent.status;

    return root;
}

std::string RentDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}

std::string ToJSON(const RentsDTO &rents)
{
    Json::Value arr = Json::arrayValue;

    for (auto rent : rents)
    {
        Json::Value root = GetJSON(rent);
        arr.append(root);
    }

    Json::FastWriter writer;

    return writer.write(arr);
}

RentsDTO FromJSON(const std::string & json)
{
    Json::Value values;
    Json::Reader reader;

    if (!reader.parse(json, values))
    {
        throw JsonParserException("can't parse RentsDTO object");
    }

    RentsDTO rents;
    for (const auto& value : values)
    {
        RentDTO rent;
        rent.id = value["id"].asUInt64();
        rent.rentUid = value["rentalUid"].asString();
        rent.username = value["username"].asString();
        rent.paymentUid = value["paymentUid"].asString();
        rent.carUid = value["carUid"].asString();
        rent.dateFrom = value["dateFrom"].asString();
        rent.dateTo = value["dateTo"].asString();
        rent.status = value["status"].asString();

        rents.push_back(rent);
    }

    return rents;
}