import QtQuick
import QtQuick.Controls
import tictactoe

Rectangle {
    property var gameMaster;

    id: root
    color: "#CCCCCC"

    Column {
        width: parent.width
        height: parent.height
        spacing: 0

        CheckBox {
            id: noughHuman
            text: "Play as noughs"
        }

        CheckBox {
            id: crossHuman
            text: "Play as crosses"
            checked: true
        }

        Button {
            width: parent.width
            text: "Start a game"
            onClicked: {
                root.gameMaster.restart(GameMaster.Initiative.Cross, noughHuman.checked, crossHuman.checked)
            }
        }
    }
}
