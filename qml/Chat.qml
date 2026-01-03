import QtQuick
import QtWebView
import QtWebChannel
import vovachat

Item {
    id: chatRoot
    property var externalChannel

    WebView {
        id: view
        anchors.fill: parent
        url: "https://chat.openai.com"

        onLoadingChanged: function(load) {
            if (load.status === WebView.LoadSucceededStatus) {
                console.log("[QML] Page loaded, injecting test input")
                view.runJavaScript(`
                (function waitForEditor() {
                    const editor = document.querySelector('div[contenteditable="true"]');
                    if (!editor) {
                        setTimeout(waitForEditor, 500);
                        return;
                    }

                    editor.focus();

                    document.execCommand("insertText", false, "Привіт! Це тестовий запит 🚀");

                    setTimeout(() => {
                        editor.dispatchEvent(
                            new KeyboardEvent("keydown", {
                                key: "Enter",
                                bubbles: true
                            })
                        );
                        console.log("[JS] Sent via Enter");
                    }, 200);
                })();
                `);
            }
        }
    }
}
