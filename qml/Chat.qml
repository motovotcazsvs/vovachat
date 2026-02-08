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
                view.startBridge("Привіт! Це фінальна перевірка. Напиши 'OK', якщо все працює.");
            }
        }

        function startBridge(promptText) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "qrc:///qtwebchannel/qwebchannel.js");
            xhr.onreadystatechange = function() {
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    if (xhr.status === 200 || xhr.status === 0) {
                        injectAndRun(xhr.responseText, promptText);
                    } else {
                        console.log("QML: Помилка завантаження qwebchannel.js. Спроба без бібліотеки.");
                        injectAndRun("", promptText);
                    }
                }
            };
            xhr.send();
        }

        function injectAndRun(libraryCode, promptText) {
            let bridgeLogic = `
                (function() {
                    console.log("JS: Міст ініціалізовано.");
                    
                    function runWithBackend(backend) {
                        const observer = new MutationObserver(() => {
                            let msgs = document.querySelectorAll('[data-message-author-role="assistant"]');
                            if (msgs.length > 0) {
                                let lastMsg = msgs[msgs.length - 1];
                                let stopBtn = document.querySelector('button[aria-label="Stop generating"]');
                                if (!stopBtn && lastMsg.innerText.trim().length > 0) {
                                    let txt = lastMsg.innerText.trim();
                                    if (backend) backend.onResponseFromJS(txt);
                                }
                            }
                        });
                        observer.observe(document.body, { childList: true, subtree: true });

                        var input = document.querySelector('#prompt-textarea') || document.querySelector('textarea');
                        if (input) {
                            input.focus();
                            input.value = "${promptText}";
                            input.dispatchEvent(new Event('input', { bubbles: true }));
                            setTimeout(() => {
                                var btn = document.querySelector('[data-testid="send-button"]');
                                if (btn) btn.click();
                                else input.dispatchEvent(new KeyboardEvent('keydown', {key: 'Enter', bubbles: true}));
                            }, 1000);
                        }
                    }

                    if (typeof QWebChannel !== 'undefined') {
                        new QWebChannel(qt.webChannelTransport, function(channel) {
                            runWithBackend(channel.objects.qtBackend);
                        });
                    }
                })();
            `;
            view.runJavaScript(libraryCode + "\n" + bridgeLogic);
        }
    }
}
