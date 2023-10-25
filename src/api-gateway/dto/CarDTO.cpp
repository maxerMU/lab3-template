#include "CarDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void CarDTO::FromJSON(const std::string &json)
{
    Json::Value value;
    Json::Reader reader;

    if (!reader.parse(json, value))
    {
        throw JsonParserException("can't parse PersonDTO object");
    }

    carUid = value["carUid"].asString();
    brand = value["brand"].asString();
    model = value["model"].asString();
    registrationNumber = value["registrationNumber"].asString();
    power = value["power"].asUInt64();
    price = value["price"].asUInt64();
    type = value["type"].asString();
    available = value["available"].asBool();
}

Json::Value GetJSON(const CarDTO &person)
{
    Json::Value root;

    root["carUid"] = person.carUid;
    root["brand"] = person.brand;
    root["model"] = person.model;
    root["registrationNumber"] = person.registrationNumber;
    root["power"] = (uint)person.power;
    root["price"] = (uint)person.price;
    root["type"] = person.type;
    root["available"] = person.available;

    return root;
}

std::string CarDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}

std::string ToJSON(const CarsDTO &cars)
{
    Json::Value arr = Json::arrayValue;

    for (auto car : cars)
    {
        Json::Value root = GetJSON(car);
        arr.append(root);
    }

    Json::FastWriter writer;

    return writer.write(arr);
}