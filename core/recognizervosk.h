#ifndef RECOGNIZERVOSK_H
#define RECOGNIZERVOSK_H

#include <QAudioInput>
#include <QIODevice>
#include <memory>



class RecognizerVosk
{
public:
    RecognizerVosk();
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool isReady() const;

signals:
    void partialResult(QString text);
    void finalResult(QString text);

private:
    QAudioInput *audioInput = nullptr;
    QIODevice *audioDevice = nullptr;

    void *model = nullptr;
    void *recognizer = nullptr;

    bool ready = false;

};

#endif // RECOGNIZERVOSK_H
