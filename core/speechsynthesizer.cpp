#include "speechsynthesizer.h"

#include <QDebug>

SpeechSynthesizer::SpeechSynthesizer(QObject *parent)
    : QObject(parent)
{
    tts = new QTextToSpeech(this);

    // Підключаємо сигнали
    connect(tts, &QTextToSpeech::stateChanged, this, [this](QTextToSpeech::State state){
        if (state == QTextToSpeech::Speaking)
            emit speakingStarted();
        if (state == QTextToSpeech::Ready)
            emit speakingFinished();
    });

    // Мова за замовчуванням — українська
    tts->setLocale(QLocale("uk_UA"));
}

void SpeechSynthesizer::speak(const QString &text)
{
    if (!tts)
        return;

    tts->say(text);
}

void SpeechSynthesizer::stop()
{
    if (tts)
        tts->stop();
}

void SpeechSynthesizer::setLanguage(const QString &langCode)
{
    if (!tts)
        return;

    QLocale locale(langCode);
    tts->setLocale(locale);

    qDebug() << "Language changed to:" << locale.name();
}

QString SpeechSynthesizer::currentLanguage() const
{
    return tts->locale().name();
}
