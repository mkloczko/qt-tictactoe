import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Tic Tac Toe")
    Board {
        anchors.fill: parent
    }
}
