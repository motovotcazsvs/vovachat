import QtQuick
import QtWebView
import QtWebChannel
import vovachat

Item {
    id: chatRoot

    // Вместо алиаса используем явное свойство, которое примет объект из Main.qml
    property var externalChannel

    WebView {
        id: view
        anchors.fill: parent
        url: "https://chat.openai.com"

        // В некоторых сборках Qt 6 на MSVC WebView требует явного указания,
        // что мы присваиваем объект WebChannel
    }

    // Используем Binding, чтобы избежать ошибки при создании объекта
    Binding {
        target: view
        property: "webChannel"
        value: chatRoot.externalChannel
        when: chatRoot.externalChannel !== undefined
    }
}
