import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0




Window {
    id: infoscreen
    minimumWidth: 375
    minimumHeight: 667
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    visible: true
    title: qsTr("App")
    color: "#E5E5E5"

    Label {
        id: infoTitle
        x: 10
        y: 20
        width: 355
        height: 40
        text: "Real Time Info"
        color: "black"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 30
        font.family: "Roboto"
        background: Rectangle{
        color: "#E5E5E5"
            //color: "yellow"
        }
    }

    Label {
        id: sensorsTitle
        x: 10
        y: 70
        width: 60
        height: 40
        text: "Sensors"
        color: "black"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 12
        font.family: "Roboto"
        background: Rectangle{
        color: "#E5E5E5"
            //color: "yellow"
        }
    }
    MyButton{
        id: backInfoScreen
        x: 150
        y: 70
        width: 60
        height: 40
        labelText: "Back"
        buttonColor: "grey"
        textColor: "black"
    }

    GridLayout{
        id: gridlayout2
        anchors.margins:10
        anchors.top: sensorsTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        columnSpacing: 0
        rowSpacing: 10
        columns: 1
        rows: 3

        ListModel{
            id: model2
            ListElement{
                name: "Temperature"
                symbol: "ºC"
            }
            ListElement{
                name: "Humidity"
                symbol: "%"
            }
            ListElement{
                name: "Luminosity"
                symbol: "%"
            }
        }
        Repeater{
            model: model2
            SensorsLabel{
                name: model.name
                symbol: model.symbol
                value: 100
            }
        }
    }

    Label {
        id: actuatorsTitle
        x: 10
        y: 320
        width: 355
        height: 30
        text: "Actuators"
        color: "black"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 12
        font.family: "Roboto"
        background: Rectangle{
            color: "#E5E5E5"
            //color: "yellow"
        }
    }

    GridLayout{
        id: gridlayout3
        anchors.margins:10
        anchors.top: actuatorsTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        columnSpacing: 0
        rowSpacing: 10
        columns: 1
        rows: 4

        ListModel{
            id: model3
            ListElement{
                name: "Heater"
            }
            ListElement{
                name: "Water Pump"
            }
            ListElement{
                name: "Light"
            }
            ListElement{
                name: "Window"
            }
        }
        Repeater{
            model: model3
            ActuatorsLabel{
                name: model.name
                actuatorstate: "On"
                backgroundColor: "#F98989"
            }
        }
    }




}
