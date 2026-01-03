#include "trayiconmanager.h"

#include <QAction>
#include <QApplication>

TrayIconManager::TrayIconManager(QObject *parent) :
    QObject(parent)
{
#ifdef Q_OS_WINDOWS
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/mic.png"));   // постав свій значок
    trayIcon->setToolTip("VovaChat - голосовий чат");

    // Меню
    trayMenu = new QMenu();

    QAction *startAction = new QAction("🎤 Почати говорити", trayMenu);
    QObject::connect(startAction, &QAction::triggered, this, &TrayIconManager::startSpeechRecognition);
    trayMenu->addAction(startAction);

    QAction *stopAction = new QAction("🛑 Зупинити", trayMenu);
    QObject::connect(stopAction, &QAction::triggered, this, &TrayIconManager::stopSpeechRecognition);
    trayMenu->addAction(stopAction);

    trayMenu->addSeparator();

    QAction *exitAction = new QAction("❌ Вийти", trayMenu);
    QObject::connect(exitAction, &QAction::triggered, this, &TrayIconManager::exitRequested);
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);

    connect(trayIcon, &QSystemTrayIcon::activated,
            this, &TrayIconManager::onIconActivated);
#endif
}

void TrayIconManager::show()
{
#ifdef Q_OS_WINDOWS
    if (trayIcon)
        trayIcon->show();
#endif
}

void TrayIconManager::hide()
{
#ifdef Q_OS_WINDOWS
    if (trayIcon)
        trayIcon->hide();
#endif
}

void TrayIconManager::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
#ifdef Q_OS_WINDOWS
    if (reason == QSystemTrayIcon::Trigger)
        emit startSpeechRecognition();
#endif
}
