#include <ui/qml/component/UIMessage.hpp>

UIMessage::UIMessage(QObject *parent)
    : QObject(parent)
{}

void UIMessage::show(const QString &message, const QString &type)
{
    emit messageRaised(message, type);
}
