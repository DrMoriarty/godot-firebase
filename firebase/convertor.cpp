#include "convertor.h"

Variant Convertor::fromFirebaseVariant(const firebase::Variant& arg)
{
    if(arg.is_null()) {
        return Variant((void*)NULL);
    } else if(arg.is_vector()) {
        const std::vector<firebase::Variant>& vector = arg.vector();
        Vector<Variant> vecRes;
        for(int i=0; i<vector.size(); i++) {
            vecRes.push_back(fromFirebaseVariant(vector[i]));
        }
        return Variant(vecRes);
    } else if(arg.is_map()) {
        const std::map<firebase::Variant, firebase::Variant>& map = arg.map();
        Dictionary dictRes;
        for(std::map<firebase::Variant, firebase::Variant>::const_iterator i=map.begin(); i!=map.end(); i++) {
            firebase::Variant first = i->first;
            firebase::Variant second = i->second;
            String key;
            key.parse_utf8(first.string_value());
            dictRes[key] = fromFirebaseVariant(second);
        }
        return Variant(dictRes);
    } else if(arg.is_int64()) {
        return Variant(arg.int64_value());
    } else if(arg.is_double()) {
        return Variant(arg.double_value());
    } else if(arg.is_bool()) {
        return Variant(arg.bool_value());
    } else if(arg.is_string()) {
        String str;
        if(!str.parse_utf8(arg.string_value()))
            return Variant(str);
        else
            return Variant(arg.string_value());
    } else {
        return Variant((void*)NULL);
    }
}

firebase::Variant Convertor::toFirebaseVariant(const String& arg)
{
    std::string tmp(arg.utf8().get_data());
    return firebase::Variant(tmp);
}

firebase::Variant Convertor::toFirebaseVariant(const Dictionary& arg)
{
    std::map<firebase::Variant, firebase::Variant> map;
    for(int i=0; i<arg.size(); i++) {
        Variant key = arg.get_key_at_index(i);
        Variant val = arg.get_value_at_index(i);
        if (key.get_type() != Variant::STRING) {
            print_line(String("Unknown key type: ") + itos(key.get_type()));
            continue;
        }
        firebase::Variant fkey = toFirebaseVariant((String)key);
        if(val.get_type() == Variant::INT) {
            map[fkey] = firebase::Variant((int)val);
        } else if(val.get_type() == Variant::REAL) {
            map[fkey] = firebase::Variant((double)val);
        } else if (val.get_type() == Variant::STRING) {
            map[fkey] = toFirebaseVariant((String)val);
        } else if(val.get_type() == Variant::BOOL) {
            map[fkey] = firebase::Variant((bool)val);
        } else {
            map[fkey] = firebase::Variant(false);
            print_line(String("Unknown variant type: ") + itos(val.get_type()));
        }
    }

    return firebase::Variant(map);
}
