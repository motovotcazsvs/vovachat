#ifndef LLM_H
#define LLM_H

#include <QObject>

class LLM : public QObject
{
    Q_OBJECT
public:
    explicit LLM(QObject *parent = nullptr);

signals:
};

#endif // LLM_H
