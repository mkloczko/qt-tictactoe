import QtQuick
import tictactoe

Item {
    property var gameMaster: GameMaster {
        onBoardUpdated: (ix) => {
            canvas.drawElement(ix)
            canvas.requestPaint()
        }

        onRestarted: {
            canvas.clear()
            canvas.drawGrid()
            canvas.requestPaint()
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: (mouse) => {
            var x = Math.floor(mouse.x/(width/3));
            var y = Math.floor(mouse.y/(height/3));
            var player = parent.gameMaster.getCurrentHumanPlayer();
            if (player)
                player.selectSlot(3*y + x)
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent

        function drawGrid()
        {
            var ctx = getContext("2d");
            var margin = 14;

            ctx.lineWidth = 2;
            ctx.strokeStyle = "#334433"
            ctx.beginPath()
            ctx.moveTo(width/3, margin)
            ctx.lineTo(width/3, height - margin)
            ctx.moveTo(2*width/3, margin)
            ctx.lineTo(2*width/3, height - margin)
            ctx.moveTo(margin, height/3)
            ctx.lineTo(width - margin, height/3)
            ctx.moveTo(margin, 2*height/3)
            ctx.lineTo(width - margin, 2*height/3)
            ctx.stroke()
        }

        function drawElement(ix)
        {
            var ctx = getContext("2d");
            var drawCross = function(left, top, width, height, margin){
                ctx.lineWidth = 3;
                ctx.strokeStyle = "#AA3322"
                ctx.beginPath()
                ctx.moveTo(left+margin, top+margin)
                ctx.lineTo(left + width-margin, top + height-margin)
                ctx.moveTo(left + width-margin, top+margin)
                ctx.lineTo(left + margin, top + height - margin)
                ctx.stroke()
            }

            var drawNough = function(left, top, width, height,margin){
                ctx.lineWidth = 3;
                ctx.strokeStyle = "#00AA00"
                ctx.beginPath()
                ctx.ellipse(left+margin, top+margin, width-2*margin, height-2*margin)
                ctx.stroke()
            }


            var drawer;
            var x = ix % 3
            var y = Math.floor(ix / 3)
            var slotState = parent.gameMaster.gameState.getSlotAt(ix)
            if (slotState === GameState.SlotState.Cross)
                drawer = drawCross;
            else if (slotState === GameState.SlotState.Nough)
                drawer = drawNough;
            else
                return;

            drawer(x*width/3,y*height/3,width/3,height/3,10);
        }

        function clear()
        {
            var ctx = getContext("2d");
            ctx.reset();
        }
    }
}