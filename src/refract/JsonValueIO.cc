//
//  refract/JsonValueIO.cc
//  librefract
//
//  Created by Thomas Jandečka on 12/11/17.
//  Copyright (c) 2017 Apiary Inc. All rights reserved.
//

#include "JsonValueIO.h"

using namespace refract;
using namespace json;

std::ostream& json::operator<<(std::ostream& out, const number& v)
{
    out << v.value;
    return out;
}

std::ostream& json::operator<<(std::ostream& out, const string& v)
{
    out << v.value;
    return out;
}

std::ostream& json::operator<<(std::ostream& out, const boolean& v)
{
    out << v.value;
    return out;
}

std::ostream& json::operator<<(std::ostream& out, const array& v)
{
    out << '[';
    auto it = v.value.begin();
    const auto e = v.value.end();
    while (it != e) {
        out << *it;
        if (++it != e)
            out << ", ";
    }

    out << ']';
    return out;
}

std::ostream& json::operator<<(std::ostream& out, const object& v)
{
    out << '{';
    auto it = v.value.begin();
    const auto e = v.value.end();
    while (it != e) {
        out << '"' << it->first << '"' << ": " << it->second;
        if (++it != e)
            out << ", ";
    }

    out << '}';
    return out;
}

std::ostream& json::operator<<(std::ostream& out, const null& v)
{
    out << "null";
    return out;
}

std::ostream& json::operator<<(std::ostream& out, const value& v)
{
    v.visit([](const auto& val, std::ostream& o) { o << val; }, out);
    return out;
}
