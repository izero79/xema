import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript



Page {
    id: statusEditPage
    tools: editToolBar
    property bool statusEdited: false
    property bool addingNew: false

    ToolBarLayout {
        id: editToolBar
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
                console.log("status edit page back clicked")
                if (statusEdited == true)
                {
                    if (addingNew == true)
                    {
                        console.log("adding new status")
                        if (!mandatoryInfoExists())
                        {
                            pageStack.pop()
                            return
                        }

                        var rows = statusModel.rowCount()
                        statusModel.setData(rows, finNameTf.text, 36)
                        statusModel.setData(rows, sweNameTf.text, 39)
                        statusModel.setData(rows, engNameTf.text, 40)
                        statusModel.setData(rows, abbrevTf.text, 35)
                        statusModel.setData(rows, true, 42)
                    }
                    else
                    {
                        console.log("modifying")
                        statusModel.setData(currentIndex, finNameTf.text, 36)
                        statusModel.setData(currentIndex, sweNameTf.text, 39)
                        statusModel.setData(currentIndex, engNameTf.text, 40)
                        statusModel.setData(currentIndex, abbrevTf.text, 35)
                        statusModel.setData(currentIndex, true, 42)
                    }
                    if ( currentLanguage == "sv") {
                        window.newStatusCreated( sweNameTf.text + ", " + abbrevTf.text )
                    }
                    else if ( currentLanguage == "en") {
                        window.newStatusCreated( engNameTf.text + ", " + abbrevTf.text )
                    }
                    else {
                        window.newStatusCreated( finNameTf.text + ", " + abbrevTf.text )
                    }
                }
                pageStack.pop()
            }
        }
        ToolButton {
            flat: true
            iconSource: "/qml/symbian3_icons/undo.svg"
            visible: !addingNew
            onClicked: {
                console.log("status edit page undo clicked")
                undo()
            }
        }
    }

    property int currentIndex: -1

    function mandatoryInfoExists()
    {
        var dataOk = false;
        if (abbrevTf.text.length > 0)
        {
            dataOk = true;
        }
        return dataOk;
    }

    function addNew()
    {
        addingNew = true
    }

    function selectedIndex(index)
    {
        currentIndex = index
        statusEdited = false
    }

    function undo()
    {
        finNameTf.text = statusModel.data(currentIndex, 36)
        sweNameTf.text = statusModel.data(currentIndex, 39)
        engNameTf.text = statusModel.data(currentIndex, 40)
        abbrevTf.text = statusModel.data(currentIndex, 35)
    }


    Flickable {
        id: flickable1
        contentWidth: width
        contentHeight: idTf.y + idTf.height + 10
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        anchors.leftMargin: 5

        Timer {
          id: adjuster1
          interval: 200
          onTriggered: parent.adjust()
        }

        Component.onCompleted: {
          inputContext.visibleChanged.connect(adjuster1.restart)
        }

        function adjust() {
            if (!inputContext.visible) {
                return
            }

            var focusChild = null
            function findFocusChild(p) {
                if (p["activeFocus"] === true) {
                    focusChild = p
                }
                else {
                    for (var i = 0; i < p["children"].length; i++) {
                        findFocusChild(p["children"][i])
                        if (focusChild !== null) {
                            break
                        }
                    }
                }
            }
            findFocusChild(flickable1)

            if (focusChild === null) {
                return
            }

            var focusChildY = focusChild["y"]
            var focusChildHeight = focusChild["height"]
            var focusChildHeaderHeight = focusChild["headerHeight"]
            flickable1.contentY = focusChildY
        }

        Label {
            id: editText
            text: qsTr("Edit statuses")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            width: parent.width
            horizontalAlignment: Text.AlignLeft
            anchors.top: parent.top
            color: "#ffffff"
        }

        TextField {
            id: finNameTf
            height: 50
            placeholderText: qsTr("Finnish name")
            text: statusModel.data(currentIndex, 36)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            width: parent.width
            visible: window.currentLanguage == "fi"
            onTextChanged: {
                statusEdited = true
            }
        }

        TextField {
            id: sweNameTf
            width: parent.width
            height: 50
            placeholderText: qsTr("Swedish name")
            text: statusModel.data(currentIndex, 39)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            visible: window.currentLanguage == "sv"
            onTextChanged: {
                statusEdited = true
            }
        }

        TextField {
            id: engNameTf
            placeholderText: qsTr("English name")
            text: statusModel.data(currentIndex, 40)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            width: parent.width
            visible: window.currentLanguage == "en"
            onTextChanged: {
                statusEdited = true
            }
        }

        TextField {
            id: abbrevTf
            placeholderText: qsTr("Abbreviation *")
            text: statusModel.data(currentIndex, 35)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: engNameTf.bottom
            anchors.topMargin: 8
            width: parent.width
            validator: RegExpValidator{ regExp: /.{1,}/ }
            onTextChanged: {
                statusEdited = true
            }
        }

        TextField {
            id: idTf
            placeholderText: qsTr("Id number")
            enabled: false
            text: addingNew ? statusModel.rowCount() + 1 : statusModel.data(currentIndex, 34)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: abbrevTf.bottom
            anchors.topMargin: 8
            width: parent.width
            onTextChanged: {
                statusEdited = true
            }
            visible: false
        }
    }
}
