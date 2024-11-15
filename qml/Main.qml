import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Tic Tac Toe")

    Item {
        width: parent.width - menu.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Board {
            property int size: Math.min(parent.width, parent.height)
            id: board
            width: size
            height: size
            anchors.centerIn: parent
        }
    }

    Menu {
        id: menu
        width: 140
        height: parent.height
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        gameMaster: board.gameMaster
    }
}
