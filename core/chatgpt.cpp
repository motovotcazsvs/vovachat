#include "chatgpt.h"
#include <QDebug>

chatGPT::chatGPT(QObject *parent):
    QObject{parent}
{

}

void chatGPT::sendMessage(const QString &text)
{
    emit messageReceived("[SEND] " + text);
    // Реальна відправка відбувається у QML → webview.runJavaScript()
}

void chatGPT::onResponseFromJS(const QString &text)
{
    qDebug() << "ChatGPT reply:" << text;
    emit messageReceived(text);
}
