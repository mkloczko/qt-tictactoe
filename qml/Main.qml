import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Tic Tac Toe")
    Canvas {
        id: mycanvas
        width: parent.width/2
        height: parent.height/2
        anchors.centerIn: parent
        onPaint: {
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

            var drawGrid = function(margin) {
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

            drawGrid(14)
            for (var i = 0; i < 9; i++) {
                var drawer;
                var x = i % 3
                var y = Math.floor(i / 3)

                if (i % 2 == 0)
                    drawer = drawCross;
                else
                    drawer = drawNough;

                drawer(x*width/3,y*height/3,width/3,height/3,10);
            }
        }
    }
}
