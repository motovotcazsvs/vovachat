import QtQuick
import QtQuick.Controls
import QtWebView
import vovachat

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("VovaChat 2026")

    Chat {
        id: webchat
        anchors.fill: parent
        // Передаем глобальный контекстный webChannel внутрь компонента
        externalChannel: webChannel
    }
}
