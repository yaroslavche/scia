#pragma once

#include <DIContainer.hpp>
#include <ui/qml/component/QmlRouterManager.hpp>
#include <ui/qml/component/UIMessage.hpp>
#include <ui/viewmodel/PersonViewModel.hpp>
#include <QQmlApplicationEngine>

/**
 * IMPORTANT: This class intentionally uses Service Locator to avoid
 * constructor explosion when adding new bounded contexts.
 * Each View Model MUST only access its own (or needed) context.
 */
class UIComposer final
{
public:
    explicit UIComposer(DIContainer *diContainer);
    ~UIComposer();

private:
    DIContainer *m_diContainer;
    std::unique_ptr<QQmlApplicationEngine> m_engine;
    std::unique_ptr<QmlRouterManager> m_qmlRouterManager;
    std::unique_ptr<UIMessage> m_uiMessage;

    std::unique_ptr<PersonViewModel> m_personViewModel;

    void registerQmlViewModels();
    void registerQmlTypes(const char *uri) const;
};
