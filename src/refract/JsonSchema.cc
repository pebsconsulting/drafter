//
//  refract/JsonSchema.cc
//  librefract
//
//  Created by Thomas Jandečka on 12/11/17.
//  Copyright (c) 2017 Apiary Inc. All rights reserved.
//

#include "JsonSchema.h"
#include "Element.h"
#include "TypeQueryVisitor.h"
#include "dsd/ElementData.h"
#include "VisitorUtils.h"

using namespace refract;
using namespace json;

namespace
{
    template <typename E>
    bool is_named_type(const E& e) noexcept
    {
        return e.element() != E::ValueType::name;
    }

    template <typename E>
    struct schema;

    template <>
    struct schema<BooleanElement> {
    };

    template <>
    struct schema<NumberElement> {
    };

    template <>
    struct schema<StringElement> {
    };

    template <>
    struct schema<NullElement> {
    };

    template <>
    struct schema<ObjectElement> {
        object properties;
        object patternProperties;
        array required;
    };

    template <>
    struct schema<ArrayElement> {
        array items;
    };

    template <>
    struct schema<EnumElement> {
        array oneOf;
    };
}

namespace
{
    template <typename E, typename = std::enable_if<is_primitive<E> > >
    void addPrimitiveDefinition(schema<E>& s, const E& el)
    {
        assert(is_named_type(el));

        s.definitions.value.emplace_back(std::make_pair("type", string{ "number" }));
    }

    void schema_to_object(schema<ObjectElement> s, object& o)
    {
        if (!s.definitions.value.empty())
            o.value.emplace_back(std::make_pair("definitions", std::move(s.definitions)));

        o.value.emplace_back(std::make_pair("type", string("object")));

        o.value.emplace_back(std::make_pair("properties", std::move(s.properties)));
        o.value.emplace_back(std::make_pair("required", std::move(s.required)));
    }
}

namespace
{
    template <typename E>
    schema<E> generateSchema(const E& el, object& definitions)
    {
        if (el.element() != E::ValueType) // Named type
        {
            assert(false);
        } else {
            assert(false);
        }
    }
}

object refract::generateSchema(const IElement& el)
{
    return visit(el, [](const auto& e) {

        object result;
        result.value.emplace_back(std::make_pair("$schema", string("http://json-schema.org/draft-04/schema#")));

        std::object_t definitions;
        auto s = generateSchema(e, definitions);

        schema_to_object(std::move(s), result);
        return std::move(result);
    });
}

void json::addDefinition(schema& s, const IElement& el)
{
    visit(el, [&s](const auto& e) { addDefinition(s, e); });
}

void json::addDefinition(schema& s, const NumberElement& el)
{
    addPrimitiveDefinition(s, el);
}

void json::addDefinition(schema& s, const StringElement& el)
{
    addPrimitiveDefinition(s, el);
}

void json::addDefinition(schema& s, const BooleanElement& el)
{
    addPrimitiveDefinition(s, el);
}

void json::addDefinition(schema& s, const ObjectElement& el)
{
    assert(is_named_type(el));

    if (s.definitions.value.end() != find(s.definitions, el.element()))
        return;

    schema def;
    def.type = string{ "object" };

    for (const auto& e : el.get()) {
        if (const auto m = TypeQueryVisitor::as<const MemberElement>(e.get())) {
            if (HasTypeAttribute(*m, "required") || HasTypeAttribute(*m, "fixed")) {
                if (auto str = TypeQueryVisitor::as<const StringElement>(m->get().key())) {
                    def.required.value.emplace_back(str->get());
                }
            }

            if (is_named_type(*m->get().value())) {
                addDefinition(s, *m->get().value());
                def.properties.value.emplace_back(std::make_pair("$ref", //
                    string{ "#/definitions/" + m->get().value()->element() }));
            } else {
                assert(false);
            }

        } else if (const auto m = TypeQueryVisitor::as<const SelectElement>(e.get())) {
            assert(false);
        } else
            assert(false);
    }
}
