#include "MapValue.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>

MapValue::MapValue(MapValueType type)
{
    static const char emptyString[] = "";
    initBasic(type);
    switch (type) {
        case MapValueType::nullValue:
            break;
        case MapValueType::intValue:
        case MapValueType::uintValue:
            value_.int_ = 0;
            break;
        case MapValueType::realValue:
            value_.real_ = 0.0;
            break;
        case MapValueType::stringValue:
            // allocated_ == false, so this is safe.
            value_.string_ = const_cast<char *>(static_cast<char const *>(emptyString));
            break;
        case MapValueType::arrayValue:
            value_.arr_ = new ArrayValues();
            break;
        case MapValueType::objectValue:
            value_.map_ = new ObjectValues();
            break;
        case MapValueType::booleanValue:
            value_.bool_ = false;
            break;
    }
}

MapValue::MapValue(bool value)
{
    initBasic(MapValueType::booleanValue);
    value_.bool_ = value;
}

MapValue::MapValue(int32_t value)
{
    initBasic(MapValueType::intValue);
    value_.int_ = value;
}

MapValue::MapValue(uint32_t value)
{
    initBasic(MapValueType::uintValue);
    value_.uint_ = value;
}

MapValue::MapValue(int64_t value)
{
    initBasic(MapValueType::intValue);
    value_.int_ = value;
}

MapValue::MapValue(uint64_t value)
{
    initBasic(MapValueType::uintValue);
    value_.uint_ = value;
}

MapValue::MapValue(double value)
{
    initBasic(MapValueType::realValue);
    value_.real_ = value;
}

MapValue::MapValue(const char *value)
{
    initBasic(MapValueType::stringValue);
    uint32_t bufSize = strlen(value);
    value_.string_ = new char[bufSize + 1];
    memcpy(value_.string_, value, bufSize);
    value_.string_[bufSize] = '\0';
}

MapValue::MapValue(const std::string &value)
{
    initBasic(MapValueType::stringValue);
    uint32_t bufSize = value.size();
    value_.string_ = new char[bufSize + 1];
    memcpy(value_.string_, value.c_str(), bufSize);
    value_.string_[bufSize] = '\0';
}

MapValue::~MapValue()
{
}

const MapValue &MapValue::operator[](const char *key) const
{
    assert(type() == MapValueType::nullValue || type() == MapValueType::objectValue);
    std::map<std::string, MapValue>::iterator iter = value_.map_->find(key);
    if (iter == value_.map_->end()) {
        return MapValue::nullSingleton();
    }
    return iter->second;
}

MapValue &MapValue::operator[](const char *key)
{
    assert(type() == MapValueType::nullValue || type() == MapValueType::objectValue);
    if (type() == MapValueType::nullValue) {
        *this = MapValue(MapValueType::objectValue);
    }
    std::map<std::string, MapValue>::iterator iter = value_.map_->find(key);
    if (iter != value_.map_->end()) {
        return iter->second;
    }

    ObjectValues::value_type defaultValue(key, nullSingleton());
    iter = value_.map_->insert(iter, defaultValue);
    return iter->second;
}

const MapValue &MapValue::operator[](const std::string &key) const
{
    assert(type() == MapValueType::nullValue || type() == MapValueType::objectValue);
    std::map<std::string, MapValue>::iterator iter = value_.map_->find(key);
    if (iter == value_.map_->end()) {
        return MapValue::nullSingleton();
    }
    return (*iter).second;
}

MapValue &MapValue::operator[](const std::string &key)
{
    assert(type() == MapValueType::nullValue || type() == MapValueType::objectValue);
    assert(type() == MapValueType::nullValue || type() == MapValueType::objectValue);
    if (type() == MapValueType::nullValue) {
        *this = MapValue(MapValueType::objectValue);
    }
    std::map<std::string, MapValue>::iterator iter = value_.map_->find(key);
    if (iter != value_.map_->end()) {
        return iter->second;
    }

    ObjectValues::value_type defaultValue(key, nullSingleton());
    iter = value_.map_->insert(iter, defaultValue);
    return iter->second;
}

const MapValue &MapValue::operator[](uint32_t index) const
{
    assert(type() == MapValueType::nullValue || type() == MapValueType::arrayValue);
    std::map<uint32_t, MapValue>::iterator iter = value_.arr_->find(index);
    if (iter == value_.arr_->end()) {
        return MapValue::nullSingleton();
    }
    return iter->second;
}

// MapValue &MapValue::operator[](int index)
// {
//     assert(type() == MapValueType::nullValue || type() == MapValueType::arrayValue);
//     if (type() == MapValueType::nullValue) {
//         *this = MapValue(MapValueType::arrayValue);
//     }
// }

MapValue const &MapValue::nullSingleton()
{
    static MapValue const nullStatic;
    return nullStatic;
}

void MapValue::initBasic(MapValueType type, bool allocated)
{
    bits_.value_type_ = static_cast<unsigned char>(type);
    bits_.allocated_ = allocated;
}

bool MapValue::isAllocated() const
{
    return bits_.allocated_;
}

MapValueType MapValue::type() const
{
    return static_cast<MapValueType>(bits_.value_type_);
}

void MapValue::releasePayload()
{
    switch (type()) {
        case MapValueType::nullValue:
        case MapValueType::intValue:
        case MapValueType::uintValue:
        case MapValueType::realValue:
        case MapValueType::booleanValue:
            break;
        case MapValueType::stringValue:
            if (isAllocated()) {
                delete[] value_.string_;
                bits_.allocated_ = false;
            }
            break;
        case MapValueType::arrayValue:
            delete value_.arr_;
            break;
        case MapValueType::objectValue:
            delete value_.map_;
            break;
    }
}

bool MapValue::asBool() const
{
    assert(type() == MapValueType::booleanValue);
    return value_.bool_;
}

int MapValue::asInt() const
{
    assert(type() == MapValueType::intValue || type() == MapValueType::uintValue);
    return value_.int_;
}

uint32_t MapValue::asUInt() const
{
    assert(type() == MapValueType::intValue || type() == MapValueType::uintValue);
    return value_.uint_;
}

int MapValue::asInt64() const
{
    assert(type() == MapValueType::intValue || type() == MapValueType::uintValue);
    return value_.int_;
}

uint32_t MapValue::asUInt64() const
{
    assert(type() == MapValueType::intValue || type() == MapValueType::uintValue);
    return value_.uint_;
}

double MapValue::asDouble() const
{
    assert(type() == MapValueType::realValue);
    return value_.real_;
}

std::string MapValue::asString() const
{
    assert(type() == MapValueType::stringValue);
    return value_.string_;
}

MapValue &MapValue::append(MapValue &&value)
{
    return append(MapValue(value));
}

MapValue &MapValue::append(const MapValue &value)
{
    assert(type() == MapValueType::nullValue || type() == MapValueType::arrayValue);
    if (type() == MapValueType::nullValue) {
        *this = MapValue(MapValueType::arrayValue);
    }
    return value_.arr_->emplace(value_.arr_->size(), std::move(value)).first->second;
}