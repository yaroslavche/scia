#include <DIContainer.hpp>
#include <UIComposer.hpp>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    const QString appName("scia");
    qputenv("LANG", "C.UTF-8");
    qputenv("LC_ALL", "C.UTF-8");
    const QGuiApplication app(argc, argv);
    app.setApplicationName(appName);

    const auto diContainer = std::make_unique<DIContainer>(appName);
    const auto uiComposer = std::make_unique<UIComposer>(diContainer.get());

    return app.exec();
}
