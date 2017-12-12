//
//  refract/JsonValueIO.h
//  librefract
//
//  Created by Thomas Jandečka on 12/11/17.
//  Copyright (c) 2017 Apiary Inc. All rights reserved.
//

#include <iostream>
#include "JsonValue.h"

#ifndef REFRACT_JSON_VALUE_IO_H
#define REFRACT_JSON_VALUE_IO_H

namespace refract
{
    namespace json
    {
        std::ostream& operator<<(std::ostream& out, const number& v);
        std::ostream& operator<<(std::ostream& out, const string& v);
        std::ostream& operator<<(std::ostream& out, const boolean& v);
        std::ostream& operator<<(std::ostream& out, const array& v);
        std::ostream& operator<<(std::ostream& out, const object& v);
        std::ostream& operator<<(std::ostream& out, const null& v);
        std::ostream& operator<<(std::ostream& out, const value& v);
    }
}

#endif
