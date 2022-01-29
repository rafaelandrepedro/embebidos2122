import QtQuick 2.15
import QtQuick.Controls 2.15
import Wifi 1.0

Item {

    property color textColor: "blue"
    property color buttonColor: "black"

    property string labelText: ""

    width: 120
    height: 120

    Wifi{
        id: mywifi
    }

    Rectangle{
        id: itemBackground
        anchors.fill: parent
        color: buttonColor
        radius: 10
    }

    Label{
        id: label
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.family: "Roboto"
        font.pointSize: 13
        text: labelText
        color: textColor
    }

    MouseArea{
        anchors.fill: parent

        onClicked: {
            if(labelText == "Send")
                mywifi.func()


        }
    }
}

