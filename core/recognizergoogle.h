#ifndef RECOGNIZERGOOGLE_H
#define RECOGNIZERGOOGLE_H

#include <QObject>



class RecognizerGoogle : public QObject {
public:
    RecognizerGoogle();
    Q_INVOKABLE void start(const QString &lang = "uk-UA");
    Q_INVOKABLE void stop();

signals:
    void textRecognized(const QString &text);
};

#endif // RECOGNIZERGOOGLE_H
