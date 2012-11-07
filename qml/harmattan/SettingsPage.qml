import QtQuick 1.1
import com.nokia.meego 1.0
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

    function editStatus(indexi)
    {
        var editorCompnent = Qt.createComponent(Qt.resolvedUrl("StatusEditPage.qml"))
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

    Dialog {
        id: areYouSureDialog
        property alias titleText: titleTextField.text
        property alias dialogText: dialogTextField.text
        signal oksignal()
        signal cancelsignal()

        title: Label {
            id: titleTextField
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: ""
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        content:Item {
            height: 150
            width: parent.width
            anchors.topMargin: 10
            Label {
                id: dialogTextField
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: ""
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 20
            }
        }

        buttons: Item { height: dialogYes.height + 2 * 20; width: parent.width - 20
            Button {
                id: dialogYes
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 5
                width: parent.width / 2
                text: qsTr("Yes")
                onClicked: {
                    areYouSureDialog.oksignal()
                    areYouSureDialog.close()
                }
            }
            Button {
                id: dialogNo
                anchors.bottom: parent.bottom
                anchors.left: dialogYes.right
                anchors.margins: 5
                width: parent.width / 2
                text: qsTr("No")
                onClicked: {
                    areYouSureDialog.cancelsignal()
                    areYouSureDialog.close()
                }
            }
        }
//        onClickedOutside: {areYouSureDialog.cancelsignal(); areYouSureDialog.close()}
    }

    Flickable {
        contentWidth: width
        contentHeight: restoreHistory.y + restoreHistory.height + 10
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        anchors.margins: 5

        Label {
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
        ButtonColumn {
            id: row1
            spacing: 10//platformStyle.paddingMedium
            anchors.top: systematicSortText.bottom
            anchors.left: parent.left
            RadioButton {
                id: alphaButton
                text: qsTr("Alphabetical")
//                platformExclusiveGroup: orderGroup
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
//                platformExclusiveGroup: orderGroup
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
        Label {
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

        ButtonColumn {
            id: row2
            spacing: 10// platformStyle.paddingMedium
            anchors.top: groupHeader.bottom
            anchors.left: parent.left
            RadioButton {
                id: button1
                text: qsTr("Minimum")
//                platformExclusiveGroup: group
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
//                platformExclusiveGroup: group
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
//                platformExclusiveGroup: group
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
        Label {
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
        Button {
            id: editStatus
            anchors.top: editBird.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Statuses")
            onClicked: window.showListPage("editstatuses", "");
        }

        Label {
            id: backupText
            text: qsTr("Export/Import own lists")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: editStatus.bottom
            anchors.topMargin: 30
            color: "#ffffff"
        }
        Button {
            id: backupButton
            anchors.top: backupText.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Export")
            onClicked: window.exportOwnData()
        }
        Button {
            id: restoreButton
            anchors.top: backupButton.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Import")
            onClicked: window.importOwnData()
        }

        Label {
            id: restoreText
            text: qsTr("Restore default lists")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: restoreButton.bottom
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
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Clear observers?")
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.open()
            }
            function callback() {
                window.restoreObservers()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }

        }
        Button {
            id: restoreLocation
            anchors.top: restorePeople.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Locations")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Restore locations?")
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.open()
            }
            function callback() {
                window.restoreLocations()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }

        }
        Button {
            id: restoreBird
            anchors.top: restoreLocation.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Species")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Restore species?")
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.open()
            }
            function callback() {
                window.restoreSpecies()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }

        }
        Button {
            id: restoreStatus
            anchors.top: restoreBird.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Statuses")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Restore statuses?")
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.open()
            }
            function callback() {
                window.restoreStatuses()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
        }
        Button {
            id: restoreHistory
            anchors.top: restoreStatus.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Observations")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Clear all history?")
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.open()
            }
            function callback() {
                window.clearHistory()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
        }
    }
}
