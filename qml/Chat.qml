import QtQuick
import QtWebEngine
import QtWebChannel
import vovachat

Item {
    id: chatRoot
    property var externalChannel

    WebEngineView {
        id: view
        anchors.fill: parent
        url: "https://chatgpt.com"
        webChannel: chatRoot.externalChannel

        settings.javascriptEnabled: true
        settings.localContentCanAccessRemoteUrls: true

        // Ждем завершения загрузки страницы
        onLoadingChanged: function(loadRequest) {
            if (loadRequest.status === WebEngineView.LoadSucceededStatus) {
                console.log("Страница загружена, отправляем тестовый запрос...");
                sendTestPrompt("Привет, ChatGPT! Это тестовое сообщение из Qt 2026.");
            }
        }

        // Функция для вставки текста в поле ввода ChatGPT
        function sendTestPrompt(promptText) {
            let script = `
                (function() {
                    // Находим поле ввода (в 2026 году селектор может быть #prompt-textarea)
                    var inputField = document.querySelector('#prompt-textarea') || document.querySelector('textarea');
                    if (inputField) {
                        // Вставляем текст
                        inputField.value = "${promptText}";
                        inputField.dispatchEvent(new Event('input', { bubbles: true }));

                        // Ищем кнопку отправки (обычно это кнопка с атрибутом data-testid="send-button")
                        setTimeout(() => {
                            var sendButton = document.querySelector('[data-testid="send-button"]') ||
                                             document.querySelector('button[disabled="false"]') ||
                                             document.querySelector('button > svg').parentElement;
                            if (sendButton) {
                                sendButton.click();
                            }
                        }, 50000); // Небольшая задержка для обработки ввода
                    }
                })();
            `;
            view.runJavaScript(script);
        }
    }
}
