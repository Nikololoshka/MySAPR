#include "sectionalForce.h"

SectionalForce::SectionalForce()
    : ConstructionElement(),
      force_(1),
      rodId_(1)
{
}

void SectionalForce::load(QJsonObject obj)
{
    name_ = obj.value("name").toString();
    force_ = obj.value("force").toDouble();
    rodId_ = obj.value("rodId").toInt();
}

QJsonObject SectionalForce::save() const
{
    QJsonObject obj;

    obj.insert("name", name_);
    obj.insert("force", force_);
    obj.insert("rodId", rodId_);

    return obj;
}

double SectionalForce::force() const
{
    return force_;
}

void SectionalForce::setForce(double force)
{
    force_ = force;
}

int SectionalForce::rodId() const
{
    return rodId_;
}

void SectionalForce::setRodId(int rodId)
{
    rodId_ = rodId;
}
