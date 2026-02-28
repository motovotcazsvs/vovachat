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
    qDebug() << "TTS engine:" << tts->engine();
    qDebug() << "TTS locale:" << tts->locale().name();
    qDebug() << "TTS state:" << tts->state();
    qDebug() << "Available locales:" << tts->availableLocales();
}

void SpeechSynthesizer::speak(const QString &text)
{
    if (!tts)
        return;

    qDebug() << "SpeechSynthesizer::speak() called. Text length:" << text.length();
    qDebug() << "TTS state before speak:" << tts->state();
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
