#include <ui/qml/component/QmlUtilities.hpp>

QmlUtilities::QmlUtilities(QObject *parent)
    : QObject(parent)
{}

QString QmlUtilities::formatDate(const QDateTime &dateTime, const QString &format)
{
    if (!dateTime.isValid()) {
        return "";
    }
    return dateTime.toString(format);
}

QString QmlUtilities::formatDate(const QDate &date, const QString &format)
{
    if (!date.isValid()) {
        return "";
    }
    return date.toString(format);
}

QString QmlUtilities::formatTime(const QTime &time, const QString &format)
{
    if (!time.isValid()) {
        return "";
    }
    return time.toString(format);
}

QString QmlUtilities::formatDateTime(const QDateTime &dateTime, const QString &format)
{
    if (!dateTime.isValid()) {
        return "";
    }
    return dateTime.toString(format);
}
