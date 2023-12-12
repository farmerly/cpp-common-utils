#pragma once

#include "tinyxml2.h"
#include <string>

namespace xmlutils {
class TinyXmlElement
{
public:
    TinyXmlElement(tinyxml2::XMLElement *element);
    TinyXmlElement(TinyXmlElement *element);
    ~TinyXmlElement();

public:
    TinyXmlElement *getSubElement();                 // 获取子节点
    TinyXmlElement *getSubElement(std::string name); // 获取子节点
    TinyXmlElement *getNextElement();                // 获取当前节点的下一节点
    bool            toSubElement();                  // 跳转到第一个子节点
    bool            toNextElement();                 // 跳转到下一节点

    // 添加子节点
    TinyXmlElement *addSubElement(std::string name);
    TinyXmlElement *addSubElement(std::string name, std::string text);
    TinyXmlElement *addSubElement(std::string name, bool text);
    TinyXmlElement *addSubElement(std::string name, float text);
    TinyXmlElement *addSubElement(std::string name, double text);
    TinyXmlElement *addSubElement(std::string name, int32_t text);
    TinyXmlElement *addSubElement(std::string name, uint32_t text);
    TinyXmlElement *addSubElement(std::string name, int64_t text);
    TinyXmlElement *addSubElement(std::string name, uint64_t text);

public:
    std::string getSubElementText(std::string name, std::string defaultValue = "");
    bool        getSubElementBoolText(std::string name, bool defaultValue = false);
    float       getSubElementFloatText(std::string name, float defaultValue = 0.0);
    double      getSubElementDoubleText(std::string name, double defaultValue = 0.0);
    int32_t     getSubElementIntText(std::string name, int32_t defaultValue = 0);
    uint32_t    getSubElementUintText(std::string name, uint32_t defaultValue = 0);
    int64_t     getSubElementInt64Text(std::string name, int64_t defaultValue = 0);
    uint64_t    getSubElementUint64Text(std::string name, uint64_t defaultValue = 0);

public:
    std::string getTagName(); // 获取当前元素标签名称

    // 获取标签值
    std::string getText();
    bool        getBoolText();
    float       getFloatText();
    double      getDoubleText();
    int32_t     getIntText();
    uint32_t    getUintText();
    int64_t     getInt64Text();
    uint64_t    getUint64Text();

    // 获取属性
    std::string getAttribute(std::string attr); // 获取属性值
    bool        getBoolAttribute(std::string attr);
    float       getFloatAttribute(std::string attr);
    double      getDouble64Attribute(std::string attr);
    int32_t     getIntAttribute(std::string attr);
    uint32_t    getUintAttribute(std::string attr);
    int64_t     getInt64Attribute(std::string attr);
    uint64_t    getUint64Attribute(std::string attr);

public:
    void setTagName(std::string tag); // 设置标签名称

    // 写入标签内容
    void setText(std::string text); // 设置标签值
    void setText(bool value);
    void setText(float value);
    void setText(double value);
    void setText(int32_t value);
    void setText(uint32_t value);
    void setText(int64_t value);
    void setText(uint64_t value);

    // 写入属性
    void setAttribute(std::string attr, std::string value);
    void setAttribute(std::string attr, bool value);
    void setAttribute(std::string attr, float value);
    void setAttribute(std::string attr, double value);
    void setAttribute(std::string attr, int32_t value);
    void setAttribute(std::string attr, uint32_t value);
    void setAttribute(std::string attr, int64_t value);
    void setAttribute(std::string attr, uint64_t value);

private:
    tinyxml2::XMLElement *m_element;
};
} // namespace xmlutils