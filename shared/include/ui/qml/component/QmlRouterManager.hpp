#pragma once

#include <ui/qml/component/Router.hpp>

class QmlRouterManager final : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList menuItems READ getMenuItems CONSTANT)

public:
    explicit QmlRouterManager(QString qmlUriPrefix, QObject *parent = nullptr);
    ~QmlRouterManager() override = default;

    [[nodiscard]] Router *getRouter() const { return m_router.get(); }
    [[nodiscard]] static QVariantList getMenuItems();

private:
    std::unique_ptr<Router> m_router;
    const QString m_qmlUriPrefix;

    void registerCommonRoutes() const;
    void registerPersonRoutes() const;
};
