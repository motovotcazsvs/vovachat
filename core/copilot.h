#ifndef COPILOT_H
#define COPILOT_H

#include <QObject>

class Copilot : public QObject
{
    Q_OBJECT
public:
    explicit Copilot(QObject *parent = nullptr);

signals:
};

#endif // COPILOT_H
