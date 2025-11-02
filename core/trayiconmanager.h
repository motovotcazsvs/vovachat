#ifndef TRAYICONMANAGER_H
#define TRAYICONMANAGER_H

#include <QObject>

class TrayIconManager : public QObject
{
    Q_OBJECT
public:
    explicit TrayIconManager(QObject *parent = nullptr);

signals:
};

#endif // TRAYICONMANAGER_H
