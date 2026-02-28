#include "speechrecognizer.h"
#include <QDebug>

#if defined(Q_OS_ANDROID)
#include "recognizergoogle.h"
#elif defined(Q_OS_WIN)
#include "recognizervosk.h"
#endif

SpeechRecognizer::SpeechRecognizer(QObject *parent) :
    QObject{parent}
{
#if defined(Q_OS_ANDROID)
    impl = new RecognizerGoogle(this);
#elif defined(Q_OS_WIN)
    auto *vosk = new RecognizerVosk(this);
    impl = vosk;

    // Підключаємо сигнал розпізнаного тексту
    connect(vosk, &RecognizerVosk::textRecognized, this, &SpeechRecognizer::textRecognized);

    qDebug() << "SpeechRecognizer: Vosk backend, ready:" << vosk->isReady();
#endif
}

void SpeechRecognizer::start(const QString &lang)
{
    Q_UNUSED(lang);
    if (impl)
        QMetaObject::invokeMethod(impl, "start");
}

void SpeechRecognizer::stop()
{
    if (impl)
        QMetaObject::invokeMethod(impl, "stop");
}
