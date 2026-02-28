#include "recognizervosk.h"
#include <QAudioFormat>
#include <QMediaDevices>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <QFile>

#include "vosk_api.h"

RecognizerVosk::RecognizerVosk(QObject *parent)
    : QObject(parent)
{
    // Шлях до моделі відносно exe
    QString modelPath = QCoreApplication::applicationDirPath() + "/vosk/model";

    // Спробуємо також відносно вихідного коду (для розробки)
    if (!QFile::exists(modelPath)) {
        modelPath = "vosk/model";
    }

    qDebug() << "Vosk: завантаження моделі з" << modelPath;

    model = vosk_model_new(modelPath.toUtf8().constData());
    if (!model) {
        qWarning() << "Vosk: не вдалося завантажити модель!";
        return;
    }

    recognizer = vosk_recognizer_new(static_cast<VoskModel*>(model), 16000.0f);
    if (!recognizer) {
        qWarning() << "Vosk: не вдалося створити recognizer!";
        return;
    }

    ready = true;
    qDebug() << "Vosk: готовий до роботи";
}

RecognizerVosk::~RecognizerVosk()
{
    stop();
    if (recognizer)
        vosk_recognizer_free(static_cast<VoskRecognizer*>(recognizer));
    if (model)
        vosk_model_free(static_cast<VoskModel*>(model));
}

bool RecognizerVosk::isReady() const {
    return ready;
}

void RecognizerVosk::start()
{
    if (!ready) {
        qWarning() << "Vosk: не готовий!";
        return;
    }

    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioInput();
    if (defaultDevice.isNull()) {
        qWarning() << "Vosk: мікрофон не знайдено!";
        return;
    }

    qDebug() << "Vosk: використовую мікрофон:" << defaultDevice.description();

    audioSource = new QAudioSource(defaultDevice, format, this);
    audioDevice = audioSource->start();

    if (!audioDevice) {
        qWarning() << "Vosk: не вдалося відкрити аудіопотік!";
        return;
    }

    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray data = audioDevice->readAll();

        int result = vosk_recognizer_accept_waveform(
            static_cast<VoskRecognizer*>(recognizer),
            data.data(), data.size());

        if (result) {
            const char *res = vosk_recognizer_result(
                static_cast<VoskRecognizer*>(recognizer));
            // Парсимо JSON {"text": "розпізнаний текст"}
            QJsonDocument doc = QJsonDocument::fromJson(QByteArray(res));
            QString text = doc.object().value("text").toString().trimmed();
            if (!text.isEmpty()) {
                qDebug() << "Vosk final:" << text;
                emit textRecognized(text);
            }
        } else {
            const char *res = vosk_recognizer_partial_result(
                static_cast<VoskRecognizer*>(recognizer));
            QJsonDocument doc = QJsonDocument::fromJson(QByteArray(res));
            QString text = doc.object().value("partial").toString().trimmed();
            if (!text.isEmpty()) {
                emit partialResult(text);
            }
        }
    });

    qDebug() << "Vosk: запис розпочато";
}

void RecognizerVosk::stop()
{
    if (audioSource) {
        audioSource->stop();
        audioSource->deleteLater();
        audioSource = nullptr;
    }
    audioDevice = nullptr;

    if (recognizer) {
        const char *res = vosk_recognizer_final_result(
            static_cast<VoskRecognizer*>(recognizer));
        QJsonDocument doc = QJsonDocument::fromJson(QByteArray(res));
        QString text = doc.object().value("text").toString().trimmed();
        if (!text.isEmpty()) {
            qDebug() << "Vosk final (stop):" << text;
            emit textRecognized(text);
        }
    }

    qDebug() << "Vosk: запис зупинено";
}
