#ifndef CHAT_H
#define CHAT_H

#include <QObject>

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = nullptr);

signals:
};

#endif // CHAT_H
