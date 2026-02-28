#ifndef CHATGPT_H
#define CHATGPT_H

#include <QObject>


class chatGPT : public QObject
{
    Q_OBJECT
public:
    explicit chatGPT(QObject *parent = nullptr);
    Q_INVOKABLE void sendMessage(const QString &text);

signals:
    void messageReceived(const QString &text);
    void requestSendMessage(const QString &text);
    void responseReady(const QString &text);

public slots:
    void onResponseFromJS(const QString &text);

private:
    int m_responseCount = 0;
};

#endif // CHATGPT_H
