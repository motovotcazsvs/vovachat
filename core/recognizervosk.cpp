#include "recognizervosk.h"
#include <QAudioFormat>
#include <QDebug>

#include "vosk_api.h"   // заголовок Vosk
RecognizerVosk::RecognizerVosk(QObject *parent)
    : QObject(parent)
{
    // Завантаження моделі
    model = vosk_model_new("vosk/model");    // шлях до моделі
    if (!model) {
        qWarning() << "Не вдалося завантажити модель Vosk!";
        return;
    }

    // Створюємо розпізнавач
    recognizer = vosk_recognizer_new(model, 16000.0f);
    if (!recognizer) {
        qWarning() << "Не вдалося створити recognizer Vosk!";
        return;
    }

    ready = true;
    qDebug() << "Vosk готовий";
}

bool RecognizerVosk::isReady() const {
    return ready;
}

void RecognizerVosk::start()
{
    if (!ready) {
        qWarning() << "Vosk не готовий!";
        return;
    }

    // Налаштування аудіо
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    audioInput = new QAudioInput(format, this);
    audioDevice = audioInput->start();

    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray data = audioDevice->readAll();

        if (vosk_recognizer_accept_waveform(recognizer, data.data(), data.size())) {
            const char *res = vosk_recognizer_result(recognizer);
            emit finalResult(QString::fromUtf8(res));
        } else {
            const char *res = vosk_recognizer_partial_result(recognizer);
            emit partialResult(QString::fromUtf8(res));
        }
    });

    qDebug() << "Vosk start";
}

void RecognizerVosk::stop()
{
    if (audioInput) {
        audioInput->stop();
        audioInput->deleteLater();
        audioInput = nullptr;
    }

    if (audioDevice) {
        audioDevice->deleteLater();
        audioDevice = nullptr;
    }

    const char *res = vosk_recognizer_final_result(recognizer);
    emit finalResult(QString::fromUtf8(res));

    qDebug() << "Vosk stop";
}


