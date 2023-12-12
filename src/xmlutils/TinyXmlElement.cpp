#include "TinyXmlElement.h"
#include <cstdint>

using namespace xmlutils;

TinyXmlElement::TinyXmlElement(tinyxml2::XMLElement *element) : m_element(element)
{
}

TinyXmlElement::TinyXmlElement(TinyXmlElement *element)
{
    if (element) {
        m_element = element->m_element;
    } else {
        m_element = nullptr;
    }
}

TinyXmlElement::~TinyXmlElement()
{
}

TinyXmlElement *TinyXmlElement::getSubElement()
{
    tinyxml2::XMLElement *element = m_element->FirstChildElement();
    if (!element) {
        return nullptr;
    }
    return new TinyXmlElement(element);
}

TinyXmlElement *TinyXmlElement::getSubElement(std::string name)
{
    tinyxml2::XMLElement *element = m_element->FirstChildElement(name.c_str());
    if (!element) {
        return nullptr;
    }
    return new TinyXmlElement(element);
}

TinyXmlElement *TinyXmlElement::getNextElement()
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->NextSiblingElement())) {
        return nullptr;
    }
    return new TinyXmlElement(element);
}

bool TinyXmlElement::toSubElement()
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement())) {
        return false;
    }
    m_element = element;
    return true;
}

bool TinyXmlElement::toNextElement()
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->NextSiblingElement())) {
        return false;
    }
    m_element = element;
    return true;
}

TinyXmlElement *TinyXmlElement::addSubElement(std::string name)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->InsertNewChildElement(name.c_str()))) {
        return nullptr;
    }
    return new TinyXmlElement(element);
}

/*********************************** addSubElementText ************************************/

TinyXmlElement *TinyXmlElement::addSubElement(std::string name, std::string text)
{
    TinyXmlElement *element = addSubElement(name);
    if (element != nullptr) {
        element->setText(text);
    }
    return element;
}

TinyXmlElement *TinyXmlElement::addSubElement(std::string name, bool text)
{
    TinyXmlElement *element = addSubElement(name);
    if (element != nullptr) {
        element->setText(text);
    }
    return element;
}

TinyXmlElement *TinyXmlElement::addSubElement(std::string name, float text)
{
    TinyXmlElement *element = addSubElement(name);
    if (element != nullptr) {
        element->setText(text);
    }
    return element;
}

TinyXmlElement *TinyXmlElement::addSubElement(std::string name, double text)
{
    TinyXmlElement *element = addSubElement(name);
    if (element != nullptr) {
        element->setText(text);
    }
    return element;
}

TinyXmlElement *TinyXmlElement::addSubElement(std::string name, int32_t text)
{
    TinyXmlElement *element = addSubElement(name);
    if (element != nullptr) {
        element->setText(text);
    }
    return element;
}

TinyXmlElement *TinyXmlElement::addSubElement(std::string name, uint32_t text)
{
    TinyXmlElement *element = addSubElement(name);
    if (element != nullptr) {
        element->setText(text);
    }
    return element;
}

TinyXmlElement *TinyXmlElement::addSubElement(std::string name, int64_t text)
{
    TinyXmlElement *element = addSubElement(name);
    if (element != nullptr) {
        element->setText(text);
    }
    return element;
}

TinyXmlElement *TinyXmlElement::addSubElement(std::string name, uint64_t text)
{
    TinyXmlElement *element = addSubElement(name);
    if (element != nullptr) {
        element->setText(text);
    }
    return element;
}

/*********************************** getSubElementText ************************************/

std::string TinyXmlElement::getSubElementText(std::string name, std::string defaultValue)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement(name.c_str()))) {
        return defaultValue;
    }
    return std::string(element->GetText());
}

bool TinyXmlElement::getSubElementBoolText(std::string name, bool defaultValue)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement(name.c_str()))) {
        return defaultValue;
    }
    return element->BoolText(defaultValue);
}

float TinyXmlElement::getSubElementFloatText(std::string name, float defaultValue)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement(name.c_str()))) {
        return defaultValue;
    }
    return element->FloatText(defaultValue);
}

double TinyXmlElement::getSubElementDoubleText(std::string name, double defaultValue)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement(name.c_str()))) {
        return defaultValue;
    }
    return element->DoubleText(defaultValue);
}

int32_t TinyXmlElement::getSubElementIntText(std::string name, int32_t defaultValue)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement(name.c_str()))) {
        return defaultValue;
    }
    return element->IntText(defaultValue);
}

uint32_t TinyXmlElement::getSubElementUintText(std::string name, uint32_t defaultValue)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement(name.c_str()))) {
        return defaultValue;
    }
    return element->UnsignedText(defaultValue);
}

int64_t TinyXmlElement::getSubElementInt64Text(std::string name, int64_t defaultValue)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement(name.c_str()))) {
        return defaultValue;
    }
    return element->Int64Text(defaultValue);
}

uint64_t TinyXmlElement::getSubElementUint64Text(std::string name, uint64_t defaultValue)
{
    tinyxml2::XMLElement *element = nullptr;
    if (!(element = m_element->FirstChildElement(name.c_str()))) {
        return defaultValue;
    }
    return element->Unsigned64Text(defaultValue);
}

std::string TinyXmlElement::getTagName()
{
    return m_element->Value();
}

/*********************************** getText ************************************/

std::string TinyXmlElement::getText()
{
    return m_element->GetText();
}

bool TinyXmlElement::getBoolText()
{
    return m_element->BoolText();
}

float TinyXmlElement::getFloatText()
{
    return m_element->FloatText();
}

double TinyXmlElement::getDoubleText()
{
    return m_element->DoubleText();
}

int32_t TinyXmlElement::getIntText()
{
    return m_element->IntText();
}

uint32_t TinyXmlElement::getUintText()
{
    return m_element->UnsignedText();
}

int64_t TinyXmlElement::getInt64Text()
{
    return m_element->Int64Text();
}

uint64_t TinyXmlElement::getUint64Text()
{
    return m_element->Unsigned64Text();
}

/*********************************** getAttribute ************************************/

std::string TinyXmlElement::getAttribute(std::string attr)
{
    return m_element->Attribute(attr.c_str());
}

bool TinyXmlElement::getBoolAttribute(std::string attr)
{
    return m_element->BoolAttribute(attr.c_str());
}

float TinyXmlElement::getFloatAttribute(std::string attr)
{
    return m_element->FloatAttribute(attr.c_str());
}

double TinyXmlElement::getDouble64Attribute(std::string attr)
{
    return m_element->DoubleAttribute(attr.c_str());
}

int32_t TinyXmlElement::getIntAttribute(std::string attr)
{
    return m_element->IntAttribute(attr.c_str());
}

uint32_t TinyXmlElement::getUintAttribute(std::string attr)
{
    return m_element->UnsignedAttribute(attr.c_str());
}

int64_t TinyXmlElement::getInt64Attribute(std::string attr)
{
    return m_element->Int64Attribute(attr.c_str());
}

uint64_t TinyXmlElement::getUint64Attribute(std::string attr)
{
    return m_element->Unsigned64Attribute(attr.c_str());
}

void TinyXmlElement::setTagName(std::string tag)
{
    m_element->SetValue(tag.c_str());
}

/*********************************** setText ************************************/

void TinyXmlElement::setText(std::string text)
{
    m_element->SetText(text.c_str());
}

void TinyXmlElement::setText(bool value)
{
    m_element->SetText(value);
}

void TinyXmlElement::setText(float value)
{
    m_element->SetText(value);
}

void TinyXmlElement::setText(double value)
{
    m_element->SetText(value);
}

void TinyXmlElement::setText(int32_t value)
{
    m_element->SetText(value);
}

void TinyXmlElement::setText(uint32_t value)
{
    m_element->SetText(value);
}

void TinyXmlElement::setText(int64_t value)
{
    m_element->SetText(value);
}

void TinyXmlElement::setText(uint64_t value)
{
    m_element->SetText(value);
}

/*********************************** setAttribute ************************************/

void TinyXmlElement::setAttribute(std::string attr, std::string value)
{
    m_element->SetAttribute(attr.c_str(), value.c_str());
}

void TinyXmlElement::setAttribute(std::string attr, bool value)
{
    m_element->SetAttribute(attr.c_str(), value);
}

void TinyXmlElement::setAttribute(std::string attr, float value)
{
    m_element->SetAttribute(attr.c_str(), value);
}

void TinyXmlElement::setAttribute(std::string attr, double value)
{
    m_element->SetAttribute(attr.c_str(), value);
}

void TinyXmlElement::setAttribute(std::string attr, int32_t value)
{
    m_element->SetAttribute(attr.c_str(), value);
}

void TinyXmlElement::setAttribute(std::string attr, uint32_t value)
{
    m_element->SetAttribute(attr.c_str(), value);
}

void TinyXmlElement::setAttribute(std::string attr, int64_t value)
{
    m_element->SetAttribute(attr.c_str(), value);
}

void TinyXmlElement::setAttribute(std::string attr, uint64_t value)
{
    m_element->SetAttribute(attr.c_str(), value);
}
