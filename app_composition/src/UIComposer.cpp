#include <UIComposer.hpp>
#include <ui/qml/component/QmlUtilities.hpp>
#include <QCoreApplication>
#include <QObject>
#include <QQmlContext>
#include <QQuickStyle>

UIComposer::UIComposer(DIContainer *diContainer)
    : m_diContainer(diContainer)
{
    m_engine = std::make_unique<QQmlApplicationEngine>();
    QObject::connect(
        m_engine.get(),
        &QQmlApplicationEngine::objectCreationFailed,
        QCoreApplication::instance(),
        [] { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    QQuickStyle::setStyle("Material");
    m_uiMessage = std::make_unique<UIMessage>();
    const auto qmlUriPrefix = QString{"qrc:/qt/qml/%1/"}.arg(diContainer->getProjectName());
    m_qmlRouterManager = std::make_unique<QmlRouterManager>(qmlUriPrefix);
    const QString appDirPath = QCoreApplication::applicationDirPath();
    m_engine->addImportPath(appDirPath + "/qt/qml");
    registerQmlViewModels();
    const QByteArray byteArray = diContainer->getProjectName().toUtf8();
    const char *appNameCString = byteArray.constData();
    registerQmlTypes(appNameCString);
    m_engine->loadFromModule(diContainer->getProjectName(), "Main");
}

UIComposer::~UIComposer()
{
    if (m_engine) {
        m_engine->clearComponentCache();
        m_engine->quit();
        m_engine = nullptr;
    }
}

void UIComposer::registerQmlViewModels()
{
    m_personViewModel = std::make_unique<PersonViewModel>(
        m_diContainer->getCompositeLogger(),
        m_diContainer->getPersonContext()->getPersonCommandService(),
        m_diContainer->getPersonContext()->getPersonQueryService());
    m_engine->rootContext()->setContextProperty("personViewModel", m_personViewModel.get());
}

void UIComposer::registerQmlTypes(const char *uri) const
{
    qmlRegisterSingletonInstance(uri, 1, 0, "QmlRouterManager", m_qmlRouterManager.get());
    qmlRegisterSingletonInstance(uri, 1, 0, "Router", m_qmlRouterManager.get()->getRouter());
    qmlRegisterSingletonInstance(uri, 1, 0, "UIMessage", m_uiMessage.get());
    qmlRegisterSingletonType<QmlUtilities>(
        uri,
        1,
        0,
        "QmlUtilities",
        [](const QQmlEngine *engine, const QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return new QmlUtilities();
        });
}
