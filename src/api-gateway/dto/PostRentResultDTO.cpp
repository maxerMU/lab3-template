#include "PostRentResultDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void PostRentResultDTO::FromJSON(const std::string &)
{
    Json::Value value;
    Json::Reader reader;

    throw JsonParserException("PostRentResultDTO from json not implemented");
}

Json::Value GetJSON(const PostRentResultDTO &rent)
{
    Json::Value root;

    root["rentalUid"] = rent.rentUid;
    root["status"] = rent.status;
    root["dateFrom"] = rent.dateFrom;
    root["dateTo"] = rent.dateTo;
    root["carUid"] = rent.carUid;

    Json::Value payment;
    payment["paymentUid"] = rent.payment.paymentUid;
    payment["status"] = rent.payment.status;
    payment["price"] = (uint)rent.payment.price;

    root["payment"] = payment;

    return root;
}

std::string PostRentResultDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}