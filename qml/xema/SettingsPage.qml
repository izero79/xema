import QtQuick 1.1
import com.nokia.symbian 1.1
import Qt.labs.components 1.1


Page {
    id: settingsPage
    tools: toolBarLayout

    function editPeople(indexi)
    {
        var editorCompnent = Qt.createComponent(Qt.resolvedUrl("PersonEditPage.qml"))
        var editorObject = editorCompnent.createObject(window)
        pageStack.push(editorObject)
        editorObject.selectedIndex(indexi)
    }

    function editBird(indexi)
    {
        var editorCompnent = Qt.createComponent(Qt.resolvedUrl("BirdEditPage.qml"))
        var editorObject = editorCompnent.createObject(window)
        pageStack.push(editorObject)
        editorObject.selectedIndex(indexi)
    }

    function editLocation(indexi)
    {
        var editorCompnent = Qt.createComponent(Qt.resolvedUrl("LocationEditPage.qml"))
        var editorObject = editorCompnent.createObject(window)
        pageStack.push(editorObject)
        editorObject.selectedIndex(indexi)
    }

    Flickable {
        contentWidth: width
        contentHeight: restoreBird.y + restoreBird.height + 10
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent

        Text {
            id: systematicSortText
            text: qsTr("Species list sorting")
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
                text: qsTr("Alphabetical")
                platformExclusiveGroup: orderGroup
                checked: window.useSystematicSort == false
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.useSystematicSort = false
                        window.saveSystematicSorting(!checked)
                    }
                }
            }
            RadioButton {
                id: systemButton
                text: qsTr("Systematic")
                platformExclusiveGroup: orderGroup
                checked: window.useSystematicSort == true
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.useSystematicSort = true
                        window.saveSystematicSorting(checked)
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
            anchors.topMargin: 30
            text: qsTr("Default amount of fields")
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
                text: qsTr("Minimum")
                platformExclusiveGroup: group
                checked: window.defaultDetailLevel == 1
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.defaultDetailLevel = 1
                        window.saveDetailLevel(1)
                    }
                }
            }
            RadioButton {
                id: button2
                text: qsTr("Expanded")
                platformExclusiveGroup: group
                checked: window.defaultDetailLevel == 2
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.defaultDetailLevel = 2
                        window.saveDetailLevel(2)
                    }
                }
            }
            RadioButton {
                id: button3
                text: qsTr("All")
                platformExclusiveGroup: group
                checked: window.defaultDetailLevel == 3
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.defaultDetailLevel = 3
                        window.saveDetailLevel(3)
                    }
                }
            }
        }
        Text {
            id: editText
            text: qsTr("Manage lists")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: row2.bottom
            anchors.topMargin: 30
            color: "#ffffff"
        }
        Button {
            id: editPeople
            anchors.top: editText.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Observers")
            onClicked: window.showListPage("editallpeople", "");
        }
        Button {
            id: editLocation
            anchors.top: editPeople.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Locations")
            onClicked: window.showListPage("editplaces", "");
        }
        Button {
            id: editBird
            anchors.top: editLocation.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Species")
            onClicked: window.showListPage("editbirds", "");
        }

        Text {
            id: restoreText
            text: qsTr("Restore default lists")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: editBird.bottom
            anchors.topMargin: 30
            color: "#ffffff"
        }
        Button {
            id: restorePeople
            anchors.top: restoreText.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Observers")
            onClicked: window.restoreObservers()
        }
        Button {
            id: restoreLocation
            anchors.top: restorePeople.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Locations")
            onClicked: window.restoreLocations()
        }
        Button {
            id: restoreBird
            anchors.top: restoreLocation.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Species")
            onClicked: window.restoreSpecies()
        }
    }
}
