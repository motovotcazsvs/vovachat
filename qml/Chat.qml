import QtQuick
import QtWebEngine
import QtWebChannel
import vovachat

Item {
    id: chatRoot
    width: 640
    height: 480

    WebChannel {
        id: internalChannel
    }

    Component.onCompleted: {
        internalChannel.registerObject("qtBackend", chatBackendCpp)
    }

    Connections {
        target: chatBackendCpp
        function onRequestSendMessage(text) {
            console.log("QML: Отримано запит на відправку повідомлення: " + text);
            view.sendToChatGPT(text);
        }
    }

    WebEngineView {
        id: view
        anchors.fill: parent
        url: "https://chatgpt.com"
        webChannel: internalChannel

        settings.javascriptEnabled: true
        settings.localContentCanAccessRemoteUrls: true

        // Обробник логів браузера
        onJavaScriptConsoleMessage: function(consoleMessage) {
            console.log("[ChatGPT Browser] " + consoleMessage.message);
        }

        onLoadingChanged: function(loadRequest) {
            if (loadRequest.status === WebEngineView.LoadSucceededStatus) {
                console.log("QML: ChatGPT сторінка завантажена.");
                sendTimer.start()
            }
        }

        Timer {
            id: sendTimer
            interval: 5000
            repeat: false
            onTriggered: {
                view.startBridge();
                chatBackendCpp.sendMessage("Привіт! Це автоматичний тест. Напиши 'OK', якщо бачиш це.");
            }
        }

        function sendToChatGPT(promptText) {
            let script = `
                (function() {
                    var input = document.querySelector('#prompt-textarea') || document.querySelector('textarea');
                    if (input) {
                        input.focus();
                        // Надійний спосіб введення тексту для React/Next.js
                        document.execCommand('insertText', false, "${promptText}");
                        
                        setTimeout(() => {
                            var btn = document.querySelector('[data-testid="send-button"]') || 
                                      document.querySelector('button[aria-label="Send message"]');
                            if (btn && !btn.disabled) {
                                btn.click();
                                console.log("JS: Кнопку Send натиснуто.");
                            } else {
                                var enterEvent = new KeyboardEvent('keydown', {
                                    key: 'Enter', code: 'Enter', keyCode: 13, which: 13, bubbles: true
                                });
                                input.dispatchEvent(enterEvent);
                                console.log("JS: Натиснуто Enter.");
                            }
                        }, 500);
                    } else {
                        console.error("JS: Не вдалося знайти поле вводу.");
                    }
                })();
            `;
            view.runJavaScript(script);
        }

        function startBridge() {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "qrc:///qtwebchannel/qwebchannel.js");
            xhr.onreadystatechange = function() {
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    if (xhr.status === 200 || xhr.status === 0) {
                        injectAndRun(xhr.responseText);
                    } else {
                        console.log("QML: Помилка завантаження qwebchannel.js. Спроба без бібліотеки.");
                        injectAndRun("");
                    }
                }
            };
            xhr.send();
        }

        function injectAndRun(libraryCode) {
            let bridgeLogic = `
                (function() {
                    console.log("JS: Міст ініціалізовано.");
                    window.lastHandledMessage = "";
                    
                    function setupObserver(backend) {
                        const observer = new MutationObserver(() => {
                            let msgs = document.querySelectorAll('[data-message-author-role="assistant"]');
                            if (msgs.length > 0) {
                                let lastMsg = msgs[msgs.length - 1];
                                let txt = lastMsg.innerText.trim();
                                let stopBtn = document.querySelector('button[aria-label="Stop generating"]');
                                
                                // Перевіряємо, чи генерація ПРИПИНИЛАСЯ і чи це нове повідомлення
                                if (!stopBtn && txt.length > 0 && txt !== window.lastHandledMessage) {
                                    window.lastHandledMessage = txt;
                                    console.log("JS: Нова відповідь отримана:", txt.substring(0, 30) + "...");
                                    if (backend && typeof backend.onResponseFromJS === 'function') {
                                        backend.onResponseFromJS(txt);
                                    }
                                }
                            }
                        });
                        observer.observe(document.body, { childList: true, subtree: true });
                    }

                    if (typeof QWebChannel !== 'undefined') {
                        new QWebChannel(qt.webChannelTransport, function(channel) {
                            window.qtBackend = channel.objects.qtBackend;
                            setupObserver(window.qtBackend);
                            console.log("JS: WebChannel готовий.");
                        });
                    }
                })();
            `;
            view.runJavaScript(libraryCode + "\n" + bridgeLogic);
        }
    }
}
