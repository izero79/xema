import QtQuick 1.0
import com.nokia.symbian 1.1
import Qt.labs.components 1.1


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
        checked: window.useSystematicSort
        onCheckedChanged: {
            console.log( "newstate: " + checked )
            window.useSystematicSort = checked
        }

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
            checked: window.defaultDetailLevel == 1
            property bool wasPressed: false
            onPressedChanged: {
                if( pressed == true )
                {
                    wasPressed = true
                }
            }

            onCheckedChanged: {
                if( checked == true && wasPressed == true )
                {
                    wasPressed = false
                    window.defaultDetailLevel = 1
                    console.log("newlevel 1: " + window.defaultDetailLevel )
                }
            }
        }
        RadioButton {
            id: button2
            text: "Laajennettu"
            platformExclusiveGroup: group
            checked: window.defaultDetailLevel == 2
            property bool wasPressed: false
            onPressedChanged: {
                if( pressed == true )
                {
                    wasPressed = true
                }
            }

            onCheckedChanged: {
                if( checked == true && wasPressed == true )
                {
                    wasPressed = false
                    window.defaultDetailLevel = 2
                    console.log("newlevel 2: " + window.defaultDetailLevel )
                }
            }
        }
        RadioButton {
            id: button3
            text: "Kaikki"
            platformExclusiveGroup: group
            checked: window.defaultDetailLevel == 3
            property bool wasPressed: false
            onPressedChanged: {
                if( pressed == true )
                {
                    wasPressed = true
                }
            }

            onCheckedChanged: {
                if( checked == true && wasPressed == true )
                {
                    wasPressed = false
                    window.defaultDetailLevel = 3
                    console.log("newlevel 3: " + window.defaultDetailLevel )
                }
            }
        }
    }
}
