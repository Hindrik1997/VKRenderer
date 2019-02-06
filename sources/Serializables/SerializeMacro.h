//
// Created by Hindrik Stegenga on 11/11/2018.
//

#ifndef VKRENDERER_SERIALIZEMACRO_H
#define VKRENDERER_SERIALIZEMACRO_H

#include <nlohmannJSON/json.hpp>
#include <MetaStuff/Meta.h>
#include <MetaStuff/JsonCast.h>

using namespace meta;
using namespace nlohmann;

#define SERIALIZE_START(Type) namespace meta { \
    template <> \
    inline auto registerMembers<Type>() \
    { \
        return members( \

#define SERIALIZE_MEMBER(name, member_var) member(name, &member_var)
#define SERIALIZE_GETSET(name, member_get, member_set) member(name, &member_get, &member_set)

#define SERIALIZE_END ); \
    } \
}


namespace Serializable {
    template <typename T>
    void from_json(const json& j, T& obj) {
        meta::deserialize(obj, j);
    }

    template <typename T>
    void to_json(json & j, const T& obj) {
        j = meta::serialize(obj);
    }
}

#endif //VKRENDERER_SERIALIZEMACRO_H
