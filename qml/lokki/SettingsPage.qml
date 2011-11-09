import QtQuick 1.0
import com.nokia.symbian 1.0
import Qt.labs.components 1.0


Page {
    id: settingsPage

    Text{
        id: systematicSortText
        text: qsTr( "Systemaattinen lajiluettelo")
        font.pixelSize: 20
        verticalAlignment: Text.AlignVCenter
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.left: systematicSort.right
        horizontalAlignment: Text.AlignLeft
        anchors.top: parent.top
        height: systematicSort.height
        color: "#ffffff"
    }

    Switch {
        id: systematicSort
        anchors.left: parent.left
        anchors.leftMargin: 0

    }

    CheckableGroup { id: group }
    Text {
        id: groupHeader
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.top: systematicSortText.bottom
        anchors.topMargin: 8
        text: qsTr( "Kenttien oletusmäärä")
        font.pixelSize: 20
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        color: "#ffffff"
    }

    Column {
        id: row
        spacing: platformStyle.paddingMedium
        anchors.top: groupHeader.bottom
        anchors.left: parent.left
        RadioButton {
            id: button1
            text: "Minimi"
            platformExclusiveGroup: group
        }
        RadioButton {
            id: button2
            text: "Laajennettu"
            platformExclusiveGroup: group
        }
        RadioButton {
            id: button3
            text: "Kaikki"
            platformExclusiveGroup: group
            checked: true
        }
    }
}
