//
//  refract/JsonSchema.h
//  librefract
//
//  Created by Thomas Jandečka on 12/11/17.
//  Copyright (c) 2017 Apiary Inc. All rights reserved.
//

#include <iostream>
#include "JsonValue.h"
#include "ElementFwd.h"
#include "ElementIfc.h"

#ifndef REFRACT_JSON_SCHEMA_H
#define REFRACT_JSON_SCHEMA_H

namespace refract
{
    json::object generateSchema(const IElement&);
}

#endif
