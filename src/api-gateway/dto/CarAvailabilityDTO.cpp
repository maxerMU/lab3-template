#include "CarAvailabilityDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void CarAvailabilityDTO::FromJSON(const std::string &json)
{
    Json::Value value;
    Json::Reader reader;

    if (!reader.parse(json, value))
    {
        throw JsonParserException("can't parse PersonDTO object");
    }

    carUid = value["car_uid"].asString();
    available = value["available"].asBool();
}

Json::Value GetJSON(const CarAvailabilityDTO &car)
{
    Json::Value root;

    root["car_uid"] = car.carUid;
    root["available"] = car.available;

    return root;
}

std::string CarAvailabilityDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}