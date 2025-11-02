#ifndef CHATGPT_H
#define CHATGPT_H

#include <QObject>

class chatGPT : public QObject
{
    Q_OBJECT
public:
    explicit chatGPT(QObject *parent = nullptr);

signals:
};

#endif // CHATGPT_H
