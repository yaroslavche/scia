#pragma once

#include <optional>
#include <string>
#include <QTime>

class QmlBaseEntity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long long id READ id NOTIFY idChanged FINAL)

signals:
    void idChanged();

public:
    explicit QmlBaseEntity(QObject *parent = nullptr)
        : QObject(parent)
        , m_id(-1)
    {}

    [[nodiscard]] virtual long long id() const { return m_id; }

protected:
    long long m_id;

    template<typename T>
    QVariant optionalToQVariant(const std::optional<T> &opt) const
    {
        if (opt.has_value())
            return QVariant::fromValue(opt.value());
        return QVariant{};
    }

    static QVariant optionalToQVariant(const std::optional<std::string> &opt)
    {
        if (opt.has_value())
            return QVariant::fromValue(QString::fromStdString(opt.value()));
        return QVariant{};
    }

    static QDateTime convertStringToQDateTime(const std::string &dateTimeStr)
    {
        if (dateTimeStr.empty()) {
            return QDateTime{};
        }
        QDateTime dt = QDateTime::fromString(QString::fromStdString(dateTimeStr), Qt::ISODate);
        if (!dt.isValid()) {
            dt = QDateTime::fromString(QString::fromStdString(dateTimeStr), "yyyy-MM-dd");
        }
        return dt;
    }

    static QTime convertStringToQTime(const std::string &timeStr)
    {
        if (timeStr.empty()) {
            return QTime{};
        }
        QTime time = QTime::fromString(QString::fromStdString(timeStr), "HH:mm:ss");
        if (!time.isValid()) {
            time = QTime::fromString(QString::fromStdString(timeStr), "HH:mm");
        }
        return time;
    }

    static QDateTime convertTimePointToQDateTime(
        const std::chrono::system_clock::time_point &timePoint)
    {
        const auto duration = timePoint.time_since_epoch();
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        return QDateTime::fromMSecsSinceEpoch(ms);
    }
};
