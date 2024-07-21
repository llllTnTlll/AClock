import QtQuick

Item{
    id:gaugesItem
    anchors.centerIn: parent
    height: gaugesHeight
    width: gaugesWidth

    property int gaugesHeight: parent.height;
    property int gaugesWidth: parent.width;
    property string textColor: "#636363"
    property string text: "Content"

    property string trhoughColor: "#1a1a1a"
    property string progressColor: "#3fd21a"

    property int trhoughThickness: 15
    property int trhoughRadius: (gaugesWidth - 40)/2
    property int progressPercent: 80
    Text{
        color: gaugesItem.textColor
        text: gaugesItem.text
        font.pixelSize: 30
        anchors.centerIn: parent
    }

    Canvas{
        id:canvas
        height: gaugesItem.gaugesHeight
        width: gaugesItem.gaugesWidth

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, canvas.width, canvas.height);

            ctx.strokeStyle = gaugesItem.trhoughColor
            ctx.lineWidth = gaugesItem.trhoughThickness

            ctx.beginPath();
            ctx.arc(gaugesItem.gaugesWidth / 2, gaugesItem.gaugesHeight / 2, gaugesItem.trhoughRadius, 140 * Math.PI / 180, 400 * Math.PI / 180);
            ctx.stroke();

            ctx.strokeStyle = gaugesItem.progressColor
            ctx.beginPath();
            ctx.arc(gaugesItem.gaugesWidth / 2, gaugesItem.gaugesHeight / 2, gaugesItem.trhoughRadius, 140 * Math.PI / 180, (140+2.6*progressPercent) * Math.PI / 180);
            ctx.stroke();
        }
    }

    PropertyAnimation{
        id: drawAnimation
        target: gaugesItem
        property: "progressPercent"
        from: 0
        to:0
        duration: 300
        easing.type: Easing.InOutQuad
    }

    onTrhoughColorChanged: canvas.requestPaint()
    onTrhoughThicknessChanged: canvas.requestPaint()
    onTrhoughRadiusChanged: canvas.requestPaint()
    onGaugesHeightChanged: canvas.requestPaint()
    onGaugesWidthChanged: canvas.requestPaint()
    onProgressPercentChanged: canvas.requestPaint()

    function setPercentage(value:int){
        if(0 <= value <= 100){
            drawAnimation.from = progressPercent
            drawAnimation.to = value
            drawAnimation.start()
            // gaugesItem.progressPercent = value
        }
    }
}
