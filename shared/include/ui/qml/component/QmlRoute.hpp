#pragma once

#include <QVariantMap>

class QmlRoute : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString page READ page WRITE setPage NOTIFY pageChanged)
    Q_PROPERTY(QVariantMap params READ params WRITE setParams NOTIFY paramsChanged)

signals:
    void pageChanged();
    void paramsChanged();

public:
    explicit QmlRoute(QObject *parent = nullptr);
    QmlRoute(QString page, QVariantMap params, QObject *parent = nullptr);

    [[nodiscard]] QString page() const;
    [[nodiscard]] QVariantMap params() const;

    void setPage(const QString &page);
    void setParams(const QVariantMap &params);

private:
    QString m_page;
    QVariantMap m_params;
};
