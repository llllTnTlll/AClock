import QtQuick
import QtQuick.Controls

Window {
    property string darkColor: "#131313"
    property string midColor: "#464646"
    property string lightColor: "#00aa18"

    id:mainWindow
    width: 730
    height: 330
    visible: true
    color: darkColor

    ListView {
        id: menuList
        width: 100
        height: mainWindow.height - 20
        anchors.top: parent.top
        anchors.topMargin: 20
        spacing: 10

        highlightFollowsCurrentItem: true
        model: ListModel {
            ListElement {
                btnText: "Charts"
                iconPath: "qrc:/icon/img/menuIcon_charts.svg"
                screen:"ChartsScreen.qml"
            }
            ListElement {
                btnText: "Settings"
                iconPath: "qrc:/icon/img/menuIcon_settings.svg"
                screen:'SettingsScreen.qml'
            }
            ListElement {
                btnText: "Exit"
                iconPath: "qrc:/icon/img/menuIcon_exit.svg"
                screen:''

            }
        }
        delegate: Rectangle{
            id:itemRectangle
            anchors.horizontalCenter: parent.horizontalCenter
            height: width
            width: menuList.width * 0.85
            color: menuList.currentIndex === index ? lightColor : darkColor
            radius: 10

            property bool ishovered: false

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    menuList.currentIndex = index
                    // stackView.replace(model.screen)
                }

                onEntered: {
                    ishovered = true
                }

                onExited: {
                    ishovered = false
                }
            }

            Column{
                height: parent.height * 0.8
                width: height
                anchors.centerIn: parent
                spacing: 5
                IconImage{
                    id:iconImg
                    objectName: 'iconImg'
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height * 0.75
                    color: menuList.currentIndex === index ? 'white' : (ishovered ? lightColor : midColor)
                    width: height
                    source:iconPath
                }
                Text{
                    id:iconText
                    color: menuList.currentIndex === index ? 'white' : (ishovered ? lightColor : midColor)
                    text: btnText
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    StackView {
        anchors.right: parent.right
        height: mainWindow.height
        width: mainWindow.width - menuList.width
        initialItem: "qrc:/screen/ChartScreen.qml"
        replaceEnter: Transition {
            // PropertyAnimation {
            //     target: stackView
            //     property: "opacity"
            //     from: 0
            //     to: 1
            //     duration:700
            //     easing.type: Easing.InOutElastic;
            //     easing.amplitude: 2.0;
            //     easing.period: 1.5
            // }
        }
        replaceExit: Transition {
            // PropertyAnimation {
            //     target: stackView
            //     property: "opacity"
            //     from: 1
            //     to: 0
            //     duration:500
            //     easing.type: Easing.InOutElastic;
            //     easing.amplitude: 2.0;
            //     easing.period: 1.5
            // }
        }
    }
}
