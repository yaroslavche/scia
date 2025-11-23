#include <ui/PageNames.hpp>
#include <ui/qml/component/QmlRouterManager.hpp>
#include <utility>


QmlRouterManager::QmlRouterManager(QString qmlUriPrefix, QObject *parent)
    : QObject(parent)
    , m_qmlUriPrefix(std::move(qmlUriPrefix))
{
    m_router = std::make_unique<Router>();
    registerCommonRoutes();
    registerPersonRoutes();
}

void QmlRouterManager::registerCommonRoutes() const
{
    QMap<QString, QString> commonRoutes = {
        {PageNames::Home, "Page/Home.qml"},
    };
    for (auto it = commonRoutes.begin(); it != commonRoutes.end(); ++it) {
        m_router->registerPage(it.key(), m_qmlUriPrefix + it.value());
    }
}

void QmlRouterManager::registerPersonRoutes() const
{
    QMap<QString, QString> personRoutes = {
        {PageNames::PersonList, "Page/Person/PersonList.qml"},
    };
    for (auto it = personRoutes.begin(); it != personRoutes.end(); ++it) {
        m_router->registerPage(it.key(), m_qmlUriPrefix + it.value());
    }
}

QVariantList QmlRouterManager::getMenuItems()
{
    QVariantList list;

    QVariantMap homeItem;
    homeItem["name"] = "Home";
    homeItem["page"] = PageNames::Home;
    homeItem["title"] = tr("Home");
    homeItem["icon"] = "home";
    list.append(homeItem);

    QVariantMap personItem;
    personItem["name"] = "Person";
    personItem["page"] = PageNames::PersonList;
    personItem["title"] = tr("Person");
    personItem["icon"] = "person";
    list.append(personItem);

    QVariantMap unknownItem;
    unknownItem["name"] = "Not registered page";
    unknownItem["page"] = "Unknown";
    unknownItem["title"] = tr("Unknown");
    unknownItem["icon"] = "unknown";
    list.append(unknownItem);

    return list;
}
