#include <DIContainer.hpp>
#include <UIComposer.hpp>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    qputenv("LANG", "C.UTF-8");
    qputenv("LC_ALL", "C.UTF-8");
    const QGuiApplication app(argc, argv);

    const auto diContainer = std::make_unique<DIContainer>();
    const auto uiComposer = std::make_unique<UIComposer>(diContainer.get());

    return app.exec();
}
