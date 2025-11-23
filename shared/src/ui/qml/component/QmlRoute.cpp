#include <ui/qml/component/QmlRoute.hpp>
#include <utility>

QmlRoute::QmlRoute(QObject *parent)
    : QObject(parent)
{}

QmlRoute::QmlRoute(QString page, QVariantMap params, QObject *parent)
    : QObject(parent)
    , m_page(std::move(page))
    , m_params(std::move(params))
{}

QString QmlRoute::page() const
{
    return m_page;
}

QVariantMap QmlRoute::params() const
{
    return m_params;
}

void QmlRoute::setPage(const QString &page)
{
    if (m_page == page)
        return;

    m_page = page;
    emit pageChanged();
}

void QmlRoute::setParams(const QVariantMap &params)
{
    if (m_params == params)
        return;

    m_params = params;
    emit paramsChanged();
}
