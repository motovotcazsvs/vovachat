import QtQuick
import QtWebEngine // Вместо QtWebView
import QtWebChannel
import vovachat

Item {
    id: chatRoot
    property var externalChannel

    WebEngineView { // Вместо WebView
        id: view
        anchors.fill: parent
        url: "https://chatgpt.com"

        // Теперь эта строка 100% сработает в MSVC
        webChannel: chatRoot.externalChannel

        // Включаем поддержку плагинов и каналов
        settings.javascriptEnabled: true
        settings.localContentCanAccessRemoteUrls: true
    }
}
