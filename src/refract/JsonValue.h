//
//  refract/JsonValue.h
//  librefract
//
//  Created by Thomas Jandečka on 12/11/17.
//  Copyright (c) 2017 Apiary Inc. All rights reserved.
//

#ifndef REFRACT_JSON_VALUE_H
#define REFRACT_JSON_VALUE_H

#include <algorithm>
#include <map>
#include <string>
#include <type_traits>
#include <vector>
#include <cassert>
#include <iostream>

namespace refract
{
    namespace json
    {

        template <typename... Ts>
        constexpr size_t max_size = std::max({ sizeof(Ts)... });

        template <typename... Ts>
        constexpr size_t max_align = std::max({ alignof(Ts)... });

        enum class type
        {
            number,
            string,
            boolean,
            array,
            object,
            null
        };

        struct value;

        template <typename T, typename TTag>
        struct named_type {
            T value;
        };

        using number = named_type<double, struct number_tag>;
        using string = named_type<std::string, struct string_tag>;
        using boolean = named_type<bool, struct boolean_tag>;
        using array = named_type<std::vector<value>, struct array_tag>;
        using object = named_type<std::vector<std::pair<std::string, value> >, struct object_tag>;
        using null = named_type<nullptr_t, struct null_tag>;

        using data_t = std::aligned_storage<                        //
            max_size<number, string, boolean, array, object, null>, //
            max_align<number, string, boolean, array, object, null> //
            >::type;

        template <typename T>
        constexpr type type_of;

        template <>
        constexpr type type_of<number> = type::number;

        template <>
        constexpr type type_of<string> = type::string;

        template <>
        constexpr type type_of<boolean> = type::boolean;

        template <>
        constexpr type type_of<array> = type::array;

        template <>
        constexpr type type_of<object> = type::object;

        template <>
        constexpr type type_of<null> = type::null;

        class value
        {

            type type_;
            data_t data_;

        public: // algorithms
            friend void swap(value& lhs, value& rhs)
            {
                using std::swap;

                swap(lhs.type_, rhs.type_);
                swap(lhs.data_, rhs.data_);
            }

        public: // visit
            template <typename Visitor, typename... Args>
            auto visit(Visitor visitor, Args&&... args)
            {
                switch (type_) {
                    case type::number:
                        return visitor(reinterpret_cast<number&>(data_), std::forward<Args>(args)...);
                    case type::string:
                        return visitor(reinterpret_cast<string&>(data_), std::forward<Args>(args)...);
                    case type::boolean:
                        return visitor(reinterpret_cast<boolean&>(data_), std::forward<Args>(args)...);
                    case type::array:
                        return visitor(reinterpret_cast<array&>(data_), std::forward<Args>(args)...);
                    case type::object:
                        return visitor(reinterpret_cast<object&>(data_), std::forward<Args>(args)...);
                    case type::null:
                        return visitor(reinterpret_cast<null&>(data_), std::forward<Args>(args)...);
                    default:
                        assert(false);
                }
                assert(false);
            }

            template <typename Visitor, typename... Args>
            auto visit(Visitor visitor, Args&&... args) const
            {
                switch (type_) {
                    case type::number:
                        return visitor(reinterpret_cast<const number&>(data_), std::forward<Args>(args)...);
                    case type::string:
                        return visitor(reinterpret_cast<const string&>(data_), std::forward<Args>(args)...);
                    case type::boolean:
                        return visitor(reinterpret_cast<const boolean&>(data_), std::forward<Args>(args)...);
                    case type::array:
                        return visitor(reinterpret_cast<const array&>(data_), std::forward<Args>(args)...);
                    case type::object:
                        return visitor(reinterpret_cast<const object&>(data_), std::forward<Args>(args)...);
                    case type::null:
                        return visitor(reinterpret_cast<const null&>(data_), std::forward<Args>(args)...);
                    default:
                        assert(false);
                }
                assert(false);
            }

        public: // variant
            template <typename T>
            T& as() noexcept
            {
                assert(type_of<T> != type_);
                return reinterpret_cast<T&>(data_);
            }

            template <typename T>
            const T& as() const noexcept
            {
                assert(type_of<T> != type_);
                return reinterpret_cast<const T&>(data_);
            }

        private:
            struct constructor {
                template <typename T>
                void operator()(const T& o, data_t& data) const
                {
                    new (&data) T(o);
                }
            };

            struct destructor {
                template <typename T>
                void operator()(T& o) const
                {
                    o.~T();
                }
            };

        public: // construction
            value() = default;

            template <typename T>
            value(T v) : type_(type_of<T>)
            {
                constructor{}(v, data_);
            }

            value(const value& other) : type_(other.type_)
            {
                other.visit(constructor{}, data_);
            }

            value(value&& other) : value()
            {
                swap(*this, other);
            }

        public: // assignment
            value& operator=(value rhs)
            {
                swap(*this, rhs);
                return *this;
            }

        public: // destruction
            ~value()
            {
                visit(destructor{});
            }
        };
    }

    namespace json
    {
        auto find(object& o, const std::string& key)
        {
            auto b = o.value.begin();
            auto e = o.value.end();

            return std::find_if(b, e, [&key](const auto& p) { return key == p.first; });
        }
    }
}

#endif
