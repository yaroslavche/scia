#pragma once

#include <application/dto/PersonDto.hpp>
#include <ui/qml/QmlBaseEntity.hpp>

class QmlPerson final : public QmlBaseEntity
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged FINAL)

public:
    explicit QmlPerson(QObject *parent = nullptr);
    explicit QmlPerson(const PersonDto &person, QObject *parent = nullptr);
    QmlPerson(const QmlPerson &other, QObject *parent = nullptr);

    [[nodiscard]] QString name() const { return m_name; }
    [[nodiscard]] bool active() const { return m_active; }

signals:
    void nameChanged();
    void activeChanged();

private:
    QString m_name;
    bool m_active;
};
