#include "PostPaymentDTO.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <exceptions/server_exceptions.h>

void PostPaymentDTO::FromJSON(const std::string &json)
{
    Json::Value value;
    Json::Reader reader;

    if (!reader.parse(json, value))
    {
        throw JsonParserException("can't parse PaymentDTO object");
    }

    price = value["price"].asUInt64();
}

Json::Value GetJSON(const PostPaymentDTO &payment)
{
    Json::Value root;

    root["price"] = (uint)payment.price;

    return root;
}

std::string PostPaymentDTO::ToJSON() const
{
    Json::Value root = GetJSON(*this);

    Json::FastWriter writer;

    return writer.write(root);
}