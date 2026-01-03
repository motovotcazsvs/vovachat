#include "speechrecognizer.h"

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
    impl = new RecognizerVosk(this);
#endif

    if(impl) {
        QObject::connect(impl, SIGNAL(textRecognized(QString)), this, SIGNAL(textRecognized(QString)));
    }
}

void SpeechRecognizer::start(const QString &lang)
{
    if (impl)
        QMetaObject::invokeMethod(impl, "start", Q_ARG(QString, lang));
}

void SpeechRecognizer::stop()
{
    if (impl)
        QMetaObject::invokeMethod(impl, "stop");
}
