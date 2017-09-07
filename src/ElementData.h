//
//  ElementData.h
//  drafter
//
//  Created by Jiri Kratochvil on 08/08/17.
//  Copyright (c) 2017 Apiary Inc. All rights reserved.
//
#ifndef DRAFTER_ELEMENTDATA_H
#define DRAFTER_ELEMENTDATA_H

#include "RefractSourceMap.h"

namespace drafter {

    template <typename T>
    struct ElementData {
        typedef T ElementType;

        typedef typename ElementType::ValueType ValueType;
        typedef snowcrash::SourceMap<ValueType> ValueSourceMapType;

        typedef typename std::conditional<
            std::is_same<ValueType, refract::RefractElements>::value || std::is_same<ValueType, refract::IElement*>::value, // check for primitive values
            std::false_type, std::true_type>::type
            IsPrimitive;

        // This is required because snowcrash internal stuctures holds data 
        // for primitive types as "string" for complex types as "element array"
        // it will be converted into apropriated element type once all required data are colected
        
        typedef typename std::conditional<
            IsPrimitive::value, 
            std::string, // for primitive values, we will hold data as string
            refract::RefractElements // for complex types, we will hold elements
            >::type StoredType;

        typedef std::tuple<StoredType, ValueSourceMapType> ElementInfo; // [value, sourceMap]
        typedef std::tuple<std::string, snowcrash::SourceMap<std::string> > DescriptionInfo; // [description, sourceMap]

        typedef std::vector<ElementInfo> ElementInfoContainer;
        typedef std::vector<DescriptionInfo> DescriptionInfoContainer;

        ElementInfoContainer values;
        ElementInfoContainer defaults;
        ElementInfoContainer samples;
        ElementInfoContainer enumerations;

        DescriptionInfoContainer descriptions;
    };

} // namespace drafter

#endif // #ifndef DRAFTER_ELEMENTDATA_H

