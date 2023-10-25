#include "GetRentDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void GetRentDTO::FromJSON(const std::string &)
{
    Json::Value value;
    Json::Reader reader;

    throw JsonParserException("GetRentDTO from json not implemented");
}

Json::Value GetJSON(const GetRentDTO &rent)
{
    Json::Value root;

    root["rentalUid"] = rent.rentUid;
    root["status"] = rent.status;
    root["dateFrom"] = rent.dateFrom;
    root["dateTo"] = rent.dateTo;

    Json::Value car;
    car["carUid"] = rent.car.carUid;
    car["brand"] = rent.car.brand;
    car["model"] = rent.car.model;
    car["registrationNumber"] = rent.car.registrationNumber;

    root["car"] = car;

    Json::Value payment;
    payment["paymentUid"] = rent.payment.paymentUid;
    payment["status"] = rent.payment.status;
    payment["price"] = (uint)rent.payment.price;

    root["payment"] = payment;

    return root;
}

std::string GetRentDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}

std::string ToJSON(const GetRentsDTO &rents)
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
