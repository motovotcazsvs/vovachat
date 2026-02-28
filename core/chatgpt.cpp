#include "chatgpt.h"
#include <QDebug>

chatGPT::chatGPT(QObject *parent):
    QObject{parent}
{

}

void chatGPT::sendMessage(const QString &text)
{
    emit messageReceived("[SEND] " + text);
    emit requestSendMessage(text);
}

void chatGPT::onResponseFromJS(const QString &text)
{
    qDebug() << "ChatGPT reply:" << text;
    emit messageReceived(text);
    emit responseReady(text);

    m_responseCount++;
    if (m_responseCount == 1) {
        QString followUp = "Interesting! Now tell me a short joke about programmers.";
        qDebug() << "Sending follow-up message:" << followUp;
        sendMessage(followUp);
    }
}
