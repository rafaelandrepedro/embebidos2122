import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0



Window {
    id: mainmenu
    minimumWidth: 375
    minimumHeight: 667
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    visible: true
    title: qsTr("App")
    color: "#E5E5E5"

    Label {
        id: menuTitle
        x: 10
        y: 80
        width: 355
        height: 60
        text: "Main Menu"
        color: "black"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 30
        font.family: "Roboto"
        background: Rectangle{
            color: mainmenu.color
        }
    }

    GridLayout {
        id: gridlayout1
        anchors.margins: 50
        anchors.top: menuTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        columnSpacing: 20
        rowSpacing: 10
        columns: 2
        rows: 3

        ListModel{
            id: model1
            ListElement{
                buttonColor: "#B0EDF3"
                labelText: "Connect\nGreenhouse"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#CDF6C3"
                labelText: "On/Off\nSystem"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#F8BDD9"
                labelText: "Plantation"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#B0BFF3"
                labelText: "Real Time\nInfo"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#F5F3B5"
                labelText: "Generate\nReport"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#FBAEAE"
                labelText: "Exit"
                textColor: "black"
            }

        }
        Repeater{
            model: model1
            MyButton{
                buttonColor: model.buttonColor
                labelText: model.labelText
                textColor: model.textColor
            }
        }
    }





}
