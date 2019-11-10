#include "longitudinalForce.h"

LongitudinalForce::LongitudinalForce()
    : ConstructionElement(),
      force_(1),
      nodeId_(1)
{
}

void LongitudinalForce::load(QJsonObject obj)
{
    name_ = obj.value("name").toString();
    force_ = obj.value("force").toDouble();
    nodeId_ = obj.value("nodeId").toInt();
}

QJsonObject LongitudinalForce::save() const
{
    QJsonObject obj;

    obj.insert("name", name_);
    obj.insert("force", force_);
    obj.insert("nodeId", nodeId_);

    return obj;
}

double LongitudinalForce::force() const
{
    return force_;
}

void LongitudinalForce::setForce(double force)
{
    force_ = force;
}

int LongitudinalForce::nodeId() const
{
    return nodeId_;
}

void LongitudinalForce::setNodeId(int nodeId)
{
    nodeId_ = nodeId;
}
