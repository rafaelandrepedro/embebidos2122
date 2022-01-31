import QtQuick 2.15
import QtQuick.Controls 2.15
import Wifi 1.0


Item {

    property color textColor: "blue"
    property color buttonColor: "black"
    property int width1: 120
    property int height1: 120
    property string labelText: ""

    width: width1
    height: height1

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
            if(labelText == "Connect To\nGreenhouse")
                mywifi.connectToServer();
            if(labelText == "Real Time\nInfo")
            {
                loader.source = "InfoScreen.qml"
            }
            if(labelText == "Plantation")
            {
                loader.source = "PlantationScreen.qml"
            }
            if(labelText == "Back")
            {
                loader.source = "MainScreen.qml";
            }


        }
    }
}

