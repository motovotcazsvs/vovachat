#ifndef SPEECHSYNTHESIZER_H
#define SPEECHSYNTHESIZER_H

#include <QObject>

class SpeechSynthesizer : public QObject
{
    Q_OBJECT
public:
    explicit SpeechSynthesizer(QObject *parent = nullptr);

signals:
};

#endif // SPEECHSYNTHESIZER_H
