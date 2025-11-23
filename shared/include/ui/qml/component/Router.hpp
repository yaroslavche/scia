#pragma once

#include <ui/qml/component/QmlRoute.hpp>
#include <QStack>

struct Route
{
    QString page;
    QVariantMap params;

    bool operator==(const Route &other) const
    {
        return page == other.page && params == other.params;
    }
};

class Router final : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QmlRoute *currentRoute READ currentRoute NOTIFY currentRouteChanged)
    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged)

public:
    explicit Router(QObject *parent = nullptr);
    ~Router() override = default;

    [[nodiscard]] QmlRoute *currentRoute() const;
    Q_INVOKABLE void navigateTo(const QString &page, const QVariantMap &params = {});
    Q_INVOKABLE void goBack();
    [[nodiscard]] bool canGoBack() const;
    void registerPage(const QString &pageName, const QString &qmlUrl);
    Q_INVOKABLE [[nodiscard]] QString componentForRoute(const QString &pageName) const;

signals:
    void currentRouteChanged();
    void canGoBackChanged();
    void navigationError(const QString &message);

private:
    QmlRoute *m_currentRouteQml;
    Route m_currentRouteInternal;
    QMap<QString, QString> m_pageRegistry;
    QStack<Route> m_history;

    void updateCurrentRoute(const Route &newRoute);
};
