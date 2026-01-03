#ifndef SPEECHSYNTHESIZER_H
#define SPEECHSYNTHESIZER_H

#include <QObject>
#include <QTextToSpeech>
#include <QLocale>

class SpeechSynthesizer : public QObject
{
    Q_OBJECT
public:
    explicit SpeechSynthesizer(QObject *parent = nullptr);
    void speak(const QString &text);
    void stop();

    void setLanguage(const QString &langCode); // "uk-UA", "ru-RU", "en-US"
    QString currentLanguage() const;

signals:
    void speakingStarted();
    void speakingFinished();

private:
    QTextToSpeech *tts;

};

#endif // SPEECHSYNTHESIZER_H
