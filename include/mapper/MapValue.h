#pragma once

#include <deque>
#include <map>
#include <string>

enum class MapValueType {
    nullValue = 0, // 'null' value
    intValue,      // signed integer value
    uintValue,     // unsigned integer value
    realValue,     // double value
    stringValue,   // UTF-8 string value
    booleanValue,  // bool value
    arrayValue,    // array value (ordered list)
    objectValue    // object value (collection of name/value pairs).
};

class MapValue
{
public:
    MapValue(MapValueType type = MapValueType::nullValue);
    MapValue(bool value);
    MapValue(int32_t value);
    MapValue(uint32_t value);
    MapValue(int64_t value);
    MapValue(uint64_t value);
    MapValue(double value);
    MapValue(const char *value);
    MapValue(const std::string &value);
    ~MapValue();

public:
    const MapValue &operator[](const char *key) const;
    MapValue       &operator[](const char *key);
    const MapValue &operator[](const std::string &key) const;
    MapValue       &operator[](const std::string &key);
    const MapValue &operator[](uint32_t index) const;
    // MapValue       &operator[](int index);

public:
    static MapValue const                  &nullSingleton();
    typedef std::map<std::string, MapValue> ObjectValues;
    typedef std::map<uint32_t, MapValue>    ArrayValues;

private:
    void initBasic(MapValueType type, bool allocated = false);
    bool isAllocated() const;
    void releasePayload();

public:
    MapValueType type() const;
    bool         asBool() const;
    int          asInt() const;
    uint32_t     asUInt() const;
    int          asInt64() const;
    uint32_t     asUInt64() const;
    double       asDouble() const;
    std::string  asString() const;
    MapValue    &append(MapValue &&value);
    MapValue    &append(const MapValue &value);

private:
    union ValueHolder
    {
        int64_t       int_;
        uint64_t      uint_;
        double        real_;
        bool          bool_;
        char         *string_;
        ArrayValues  *arr_;
        ObjectValues *map_;
    } value_;

    struct {
        unsigned int value_type_ : 8;
        unsigned int allocated_ : 1;
    } bits_;
};