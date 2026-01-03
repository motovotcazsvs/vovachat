#ifndef TRAYICONMANAGER_H
#define TRAYICONMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>


class TrayIconManager : public QObject
{
    Q_OBJECT
public:
    explicit TrayIconManager(QObject *parent = nullptr);
    void show();          // показати трей
    void hide();          // сховати трей

signals:
    void startSpeechRecognition();   // користувач натиснув "Сказати"
    void stopSpeechRecognition();    // користувач натиснув "Стоп"
    void exitRequested();            // вихід

private slots:
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QSystemTrayIcon *trayIcon = nullptr;
    QMenu *trayMenu = nullptr;


};

#endif // TRAYICONMANAGER_H
