#include "PaymentDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void PaymentDTO::FromJSON(const std::string &json)
{
    Json::Value value;
    Json::Reader reader;

    if (!reader.parse(json, value))
    {
        throw JsonParserException("can't parse PaymentDTO object");
    }

    paymentUid = value["paymentUid"].asString();
    status = value["status"].asString();
    price = value["price"].asUInt64();
}

Json::Value GetJSON(const PaymentDTO &payment)
{
    Json::Value root;

    root["paymentUid"] = payment.paymentUid;
    root["status"] = payment.status;
    root["price"] = (uint)payment.price;

    return root;
}

std::string PaymentDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}

std::string ToJSON(const PaymentsDTO &payments)
{
    Json::Value arr = Json::arrayValue;

    for (auto payment : payments)
    {
        Json::Value root = GetJSON(payment);
        arr.append(root);
    }

    Json::FastWriter writer;

    return writer.write(arr);
}