#include "recognizergoogle.h"
#include <QJniObject>
#include <QNativeInterface>
#include <QCoreApplication>

static RecognizerGoogle *g_instance = nullptr;

RecognizerGoogle::RecognizerGoogle(QObject *parent) :
    QObject(parent)
{
    g_instance = this;
}

void RecognizerGoogle::start(const QString &lang)
{
#if defined(Q_OS_ANDROID)
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    QJniObject helper("com/vovachat/SpeechHelper",
                      "(Landroid/app/Activity;)V",
                      activity.object());
    helper.callMethod<void>("startListening",
                            "(Ljava/lang/String;)V",
                            QJniObject::fromString(lang).object<jstring>());
#endif
}

void RecognizerGoogle::stop()
{
#if defined(Q_OS_ANDROID)
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    QJniObject helper("com/vovachat/SpeechHelper",
                      "(Landroid/app/Activity;)V",
                      activity.object());
    helper.callMethod<void>("stopListening");
#endif
}

extern "C" JNIEXPORT void JNICALL
Java_com_vovachat_SpeechHelper_onSpeechResult(JNIEnv *, jclass, jstring text)
{
    QString result = QJniObject(text).toString();
    if (g_instance)
        emit g_instance->textRecognized(result);
}
RecognizerGoogle::RecognizerGoogle() {}
