import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Window {
    id: plantationscreen
    minimumWidth: 375
    minimumHeight: 667
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    visible: true
    title: qsTr("App")
    color: "#E5E5E5"

    Label {
        id: plantationTitle
        x: 10
        y: 80
        width: 355
        height: 60
        text: "Plantation"
        color: "black"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 30
        font.family: "Roboto"
        background: Rectangle{
        color: "#E5E5E5"
        }
    }
    MyButton{
        id: backPlantationScreen
        x: 150
        y: 145
        width: 60
        height: 40
        labelText: "Back"
        buttonColor: "grey"
        textColor: "black"
    }

    GridLayout {
        id: gridlayout4
        anchors.margins: 50
        anchors.top: plantationTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        columnSpacing: 20
        rowSpacing: 10
        columns: 2
        rows: 3

        ListModel{
            id: model4
            ListElement{
                buttonColor: "#B0EDF3"
                labelText: "Tomatoes"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#CDF6C3"
                labelText: "Lettuce"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#F8BDD9"
                labelText: "Spinach"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#B0BFF3"
                labelText: "Cucumber"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#F5F3B5"
                labelText: "Potatoes"
                textColor: "black"
            }
            ListElement{
                buttonColor: "#FBAEAE"
                labelText: "Carrots"
                textColor: "black"
            }

        }
        Repeater{
            model: model4
            MyButton{
                buttonColor: model.buttonColor
                labelText: model.labelText
                textColor: model.textColor
            }
        }
    }

}
