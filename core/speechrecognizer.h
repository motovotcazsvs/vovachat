#ifndef SPEECHRECOGNIZER_H
#define SPEECHRECOGNIZER_H

#include <QObject>

class SpeechRecognizer : public QObject
{
    Q_OBJECT
public:
    explicit SpeechRecognizer(QObject *parent = nullptr);

    Q_INVOKABLE void start(const QString &lang = "uk-UA");
    Q_INVOKABLE void stop();

signals:
    void textRecognized(const QString &text);

private:
    QObject *impl = nullptr;
};

#endif // SPEECHRECOGNIZER_H
