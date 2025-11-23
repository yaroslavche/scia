#pragma once

#include <QDateTime>
#include <QObject>

class QmlUtilities final : public QObject
{
    Q_OBJECT

public:
    explicit QmlUtilities(QObject *parent = nullptr);

    Q_INVOKABLE [[nodiscard]] static QString formatDate(
        const QDateTime &dateTime, const QString &format = "dd.MM.yyyy");
    Q_INVOKABLE [[nodiscard]] static QString formatDate(
        const QDate &date, const QString &format = "dd.MM.yyyy");
    Q_INVOKABLE [[nodiscard]] static QString formatTime(
        const QTime &time, const QString &format = "hh:mm");
    Q_INVOKABLE [[nodiscard]] static QString formatDateTime(
        const QDateTime &dateTime, const QString &format = "dd.MM.yyyy hh:mm");
};
