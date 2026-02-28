#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngineQuick/QtWebEngineQuick>
#include <QtWebChannel/QWebChannel>
#include <QQmlContext>
#include "core/chatgpt.h"
#include "core/speechsynthesizer.h"

int main(int argc, char *argv[])
{
    // Дозволяємо XMLHttpRequest читати локальні файли (потрібно для qrc:///...qwebchannel.js)
    qputenv("QML_XHR_ALLOW_FILE_READ", "1");

    // Ініціалізація програми
    QGuiApplication app(argc, argv);

    // ВАЖЛИВО: WebView має ініціалізуватися перед створенням двигуна QML
    QtWebEngineQuick::initialize();

    chatGPT backend;
    SpeechSynthesizer speechSynth;
    QWebChannel *channel = new QWebChannel(&app);

    // Реєструємо C++ об'єкт для використання через WebChannel (в JS частині)
    channel->registerObject(QStringLiteral("qtBackend"), &backend);

    // Підключаємо озвучення відповідей ChatGPT
    QObject::connect(&backend, &chatGPT::responseReady,
                     &speechSynth, &SpeechSynthesizer::speak);

    QQmlApplicationEngine engine;

    // --- РЕЄСТРАЦІЯ ОБ'ЄКТІВ ДЛЯ QML (Singleton) ---
    qmlRegisterSingletonInstance("vovachat", 1, 0, "SharedChannel", channel);
    qmlRegisterSingletonInstance("vovachat", 1, 0, "ChatBackendCpp", &backend);
    qmlRegisterSingletonInstance("vovachat", 1, 0, "SpeechSynth", &speechSynth);

    // Додатково реєструємо як контекстні властивості для зворотної сумісності
    engine.rootContext()->setContextProperty(QStringLiteral("webChannel"), channel);
    engine.rootContext()->setContextProperty(QStringLiteral("chatBackendCpp"), &backend);
    engine.rootContext()->setContextProperty(QStringLiteral("speechSynth"), &speechSynth);

    // Обробка помилок завантаження QML
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    // Завантаження основного модуля
    // Переконайтеся, що назва модуля "vovachat" збігається з вашим CMakeLists.txt
    engine.loadFromModule("vovachat", "Main");

    return app.exec();
}
