#ifndef RECOGNIZERVOSK_H
#define RECOGNIZERVOSK_H

#include <QObject>
#include <QAudioSource>
#include <QIODevice>

// Forward declarations для Vosk API (void* у реалізації)
class RecognizerVosk : public QObject
{
    Q_OBJECT
public:
    explicit RecognizerVosk(QObject *parent = nullptr);
    ~RecognizerVosk();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool isReady() const;

signals:
    void textRecognized(const QString &text);
    void partialResult(const QString &text);

private:
    QAudioSource *audioSource = nullptr;
    QIODevice *audioDevice = nullptr;

    void *model = nullptr;
    void *recognizer = nullptr;

    bool ready = false;
};

#endif // RECOGNIZERVOSK_H
