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
                    (function waitForTextarea() {
                        const textarea = document.querySelector("textarea");
                        if (!textarea) {
                            setTimeout(waitForTextarea, 500);
                            return;
                        }

                        textarea.focus();
                        textarea.value = "Привіт! Це тестовий запит 🚀";
                        textarea.dispatchEvent(new Event("input", { bubbles: true }));
                        textarea.dispatchEvent(
                            new KeyboardEvent("keydown", { key: "Enter", bubbles: true })
                        );
                    })();
                `);
            }
        }
    }

    // Binding {
    //     target: view
    //     property: "webChannel"
    //     value: chatRoot.externalChannel
    //     when: chatRoot.externalChannel !== undefined
    // }
}
