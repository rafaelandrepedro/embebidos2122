import QtQuick 2.15
import QtQuick.Controls 2.15
import Wifi 1.0

Item {
    property color textColor: "black"
    property color backgroundColor: "#BDE6F8"
    property string symbol: "%"
    property string name: "Temperature"
    property int value: 0

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
        font.pointSize: 13
        text: name
        color: textColor
    }
    Label{
        id: label2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        x: parent.x + 290
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        font.family: "Roboto"
        font.pointSize: 20
        text: symbol
        color: textColor
    }
    Label{
        id: label3
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: label2.left
        verticalAlignment: Text.AlignVCenter
        x: 150
        font.family: "Roboto"
        font.pointSize: 20
        text: value
        color: textColor
    }
}
