#include "rod.h"

#include <QtWidgets>

Rod::Rod()
    : ConstructionElement(),
      length_(1),
      sectionalArea_(1),
      elasticUnit_(2e11),
      allowableStrain_(1.6e8)
{
}

void Rod::load(QJsonObject obj)
{
    name_ = obj.value("name").toString();
    length_ = obj.value("length").toDouble();
    sectionalArea_ = obj.value("sectionalArea").toDouble();
    elasticUnit_ = obj.value("elasticUnit").toDouble();
    allowableStrain_ = obj.value("allowableStrain").toDouble();
}

QJsonObject Rod::save() const
{
    QJsonObject obj;

    obj.insert("name", name_);
    obj.insert("length", length_);
    obj.insert("sectionalArea", sectionalArea_);
    obj.insert("elasticUnit", elasticUnit_);
    obj.insert("allowableStrain", allowableStrain_);

    return obj;
}

double Rod::length() const
{
    return length_;
}

void Rod::setLength(double value)
{
    length_ = value;
}

double Rod::sectionalArea() const
{
    return sectionalArea_;
}

void Rod::setSectionalArea(double value)
{
    sectionalArea_ = value;
}

double Rod::elasticUnit() const
{
    return elasticUnit_;
}

void Rod::setElasticUnit(double value)
{
    elasticUnit_ = value;
}

double Rod::allowableStrain() const
{
    return allowableStrain_;
}

void Rod::setAllowableStrain(double value)
{
    allowableStrain_ = value;
}
