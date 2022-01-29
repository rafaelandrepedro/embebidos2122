import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0


Window {

    id: window
    minimumWidth: 375
    minimumHeight: 667
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    visible: true
    title: qsTr("App")
    color: "#E5E5E5"

    Label {
        id: screenTitle
        x: 10
        y: 80
        width: 355
        height: 60
        text: "Main Menu"
        color: "black"
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 30
        font.family: "Roboto"
        background: Rectangle{
            color: window.color
        }
    }

//    GridLayout
//    {

//    }

    MyButton {
        id: sendButton
        x: screenTitle.x + 30
        y: 190
        buttonColor: "#F7EFEE"
        labelText: "Send"
        textColor: "black"
    }

}

