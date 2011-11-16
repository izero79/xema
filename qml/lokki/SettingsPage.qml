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
        checked: useSystematicSort
        onCheckedChanged: {
            console.log( "newstate: " + checked )
            useSystematicSort = checked
            window.saveSystematicSorting( checked )
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
            text: qsTr( "Minimi" )
            platformExclusiveGroup: group
            checked: defaultDetailLevel == 1
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
                    defaultDetailLevel = 1
                    console.log("newlevel 1: " + defaultDetailLevel )
                    window.saveDetailLevel( 1 )
                }
            }
        }
        RadioButton {
            id: button2
            text: qsTr( "Laajennettu" )
            platformExclusiveGroup: group
            checked: defaultDetailLevel == 2
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
                    defaultDetailLevel = 2
                    console.log("newlevel 2: " + defaultDetailLevel )
                    window.saveDetailLevel( 2 )
                }
            }
        }
        RadioButton {
            id: button3
            text: qsTr( "Kaikki" )
            platformExclusiveGroup: group
            checked: defaultDetailLevel == 3
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
                    defaultDetailLevel = 3
                    console.log("newlevel 3: " + defaultDetailLevel )
                    window.saveDetailLevel( 3 )
                }
            }
        }
    }
}
