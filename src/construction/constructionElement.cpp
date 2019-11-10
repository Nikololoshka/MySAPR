#include "constructionElement.h"


QString ConstructionElement::name() const
{
    return name_;
}

void ConstructionElement::setName(const QString &name)
{
    name_ = name;
}

size_t ConstructionElement::id() const
{
    return id_;
}

void ConstructionElement::setId(size_t id)
{
    id_ = id;
}
