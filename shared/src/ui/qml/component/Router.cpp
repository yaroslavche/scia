#include <ui/PageNames.hpp>
#include <ui/qml/component/Router.hpp>

Router::Router(QObject *parent)
    : QObject(parent)
    , m_currentRouteQml(nullptr)
    , m_currentRouteInternal({PageNames::Home, {}})
{
    m_currentRouteQml
        = new QmlRoute(m_currentRouteInternal.page, m_currentRouteInternal.params, this);
    emit currentRouteChanged();
}

QmlRoute *Router::currentRoute() const
{
    return m_currentRouteQml;
}

void Router::navigateTo(const QString &page, const QVariantMap &params)
{
    if (!m_pageRegistry.contains(page)) {
        emit navigationError("Page '" + page + "' not registered.");
        qWarning() << "Navigation Error: Page not registered:" << page;
        return;
    }

    const Route newRoute = {page, params};

    if (m_currentRouteInternal.page == newRoute.page
        && m_currentRouteInternal.params == newRoute.params) {
        m_currentRouteQml->setPage(newRoute.page);
        m_currentRouteQml->setParams(newRoute.params);
        return;
    }

    if (!m_currentRouteInternal.page.isEmpty()) {
        if (m_history.isEmpty() || m_history.top() != m_currentRouteInternal) {
            m_history.push(m_currentRouteInternal);
            emit canGoBackChanged();
        }
    }

    updateCurrentRoute(newRoute);
}

void Router::goBack()
{
    if (!m_history.isEmpty()) {
        const Route previousRoute = m_history.pop();
        updateCurrentRoute(previousRoute);
        emit canGoBackChanged();
    }
}

bool Router::canGoBack() const
{
    return !m_history.isEmpty();
}

void Router::registerPage(const QString &pageName, const QString &qmlUrl)
{
    m_pageRegistry[pageName] = qmlUrl;
}

QString Router::componentForRoute(const QString &pageName) const
{
    const auto it = m_pageRegistry.find(pageName);
    if (it == m_pageRegistry.end()) {
        qWarning() << "Router Error: Page not found in registry (componentForRoute):" << pageName;
        return QString{};
    }
    return it.value();
}

void Router::updateCurrentRoute(const Route &newRoute)
{
    m_currentRouteInternal = newRoute;

    m_currentRouteQml->setPage(newRoute.page);
    m_currentRouteQml->setParams(newRoute.params);

    emit currentRouteChanged();
}
