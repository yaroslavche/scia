#pragma once

#include <QObject>

class UIMessage final : public QObject
{
    Q_OBJECT

public:
    explicit UIMessage(QObject *parent = nullptr);

    Q_INVOKABLE void show(const QString &message, const QString &type = "info");

signals:
    void messageRaised(const QString &message, const QString &type);
};
