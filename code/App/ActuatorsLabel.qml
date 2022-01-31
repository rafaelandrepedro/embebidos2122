import QtQuick 2.15
import QtQuick.Controls 2.15
import Wifi 1.0

Item {
    property color textColor: "black"
    property color backgroundColor: "#F98989"
    property string actuatorstate: "Off"
    property string name: "Heater"

    width: 340
    height: 60

    Rectangle{
        id: itemBackground
        anchors.fill: parent
        color: backgroundColor
        radius: 10
    }

    Label{
        id: label1
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        x: parent.x + 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.family: "Roboto"
        font.pointSize: 15
        text: name
        color: textColor
    }
    Label{
        id: label2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        x: parent.x + 280
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        font.family: "Roboto"
        font.pointSize: 16
        text: actuatorstate
        color: textColor
    }
}
