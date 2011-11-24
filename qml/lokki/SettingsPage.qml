import QtQuick 1.1
import com.nokia.symbian 1.1
import Qt.labs.components 1.1


Page {
    id: settingsPage
    tools: toolBarLayout

    Text{
        id: systematicSortText
        text: qsTr( "Systemaattinen lajiluettelo")
        font.pixelSize: 20
        verticalAlignment: Text.AlignVCenter
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.left: parent.left
        horizontalAlignment: Text.AlignLeft
        anchors.top: parent.top
        color: "#ffffff"
    }

    CheckableGroup { id: orderGroup }
    Column {
        id: row1
        spacing: platformStyle.paddingMedium
        anchors.top: systematicSortText.bottom
        anchors.left: parent.left
        RadioButton {
            id: alphaButton
            text: qsTr( "Aakkostettu" )
            platformExclusiveGroup: orderGroup
            checked: window.useSystematicSort == false
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
                    window.useSystematicSort = false
                    console.log("newlevel 1: " + window.useSystematicSort )
                    window.saveSystematicSorting( !checked )
                }
            }
        }
        RadioButton {
            id: systemButton
            text: qsTr( "Systemaattinen" )
            platformExclusiveGroup: orderGroup
            checked: window.useSystematicSort == true
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
                    window.useSystematicSort = true
                    console.log("newlevel sort: " + window.useSystematicSort )
                    window.saveSystematicSorting( checked )
                }
            }
        }
    }

    CheckableGroup { id: group }
    Text {
        id: groupHeader
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.top: row1.bottom
        anchors.topMargin: 8
        text: qsTr( "Kenttien oletusmäärä")
        font.pixelSize: 20
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        color: "#ffffff"
    }

    Column {
        id: row2
        spacing: platformStyle.paddingMedium
        anchors.top: groupHeader.bottom
        anchors.left: parent.left
        RadioButton {
            id: button1
            text: qsTr( "Minimi" )
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
                    window.saveDetailLevel( 1 )
                }
            }
        }
        RadioButton {
            id: button2
            text: qsTr( "Laajennettu" )
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
                    window.saveDetailLevel( 2 )
                }
            }
        }
        RadioButton {
            id: button3
            text: qsTr( "Kaikki" )
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
                    window.saveDetailLevel( 3 )
                }
            }
        }
    }
}
