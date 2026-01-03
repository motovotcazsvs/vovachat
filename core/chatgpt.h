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


public slots:
    void onResponseFromJS(const QString &text);
};

#endif // CHATGPT_H
