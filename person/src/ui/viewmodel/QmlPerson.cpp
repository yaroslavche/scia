#include <ui/viewmodel/QmlPerson.hpp>

QmlPerson::QmlPerson(QObject *parent)
    : QmlBaseEntity(parent)
    , m_name("")
    , m_active(false)
{
    m_id = -1;
}

QmlPerson::QmlPerson(const PersonDto &person, QObject *parent)
    : QmlBaseEntity(parent)
    , m_name(QString::fromStdString(person.name))
    , m_active(person.active)
{
    m_id = person.id;
}

QmlPerson::QmlPerson(const QmlPerson &other, QObject *parent)
    : QmlBaseEntity(parent)
    , m_name(other.m_name)
    , m_active(other.m_active)
{
    m_id = other.m_id;
}
