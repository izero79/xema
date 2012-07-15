import QtQuick 1.1
 import com.nokia.meego 1.1
import "myjs.js" as MyScript



Page {
    id: statusEditPage
    tools: editToolBar
    property bool statusEdited: false
    property bool addingNew: false

    ToolBarLayout {
        id: editToolBar
        ToolIcon {
            iconId: "icon-m-toolbar-back-white"
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
                        statusModel.setData(rows, name1Tf.text, 36)
                        statusModel.setData(rows, name2Tf.text, 39)
                        statusModel.setData(rows, name3Tf.text, 40)
                        statusModel.setData(rows, abbrevTf.text, 35)
                    }
                    else
                    {
                        console.log("modifying")
                        statusModel.setData(currentIndex, name1Tf.text, 36)
                        statusModel.setData(currentIndex, name2Tf.text, 39)
                        statusModel.setData(currentIndex, name3Tf.text, 40)
                        statusModel.setData(currentIndex, abbrevTf.text, 35)
                    }
                }
                pageStack.pop()
            }
        }
        ToolIcon {
            iconId: "icon-m-toolbar-undo-white"
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
        name1Tf.text = statusModel.data(currentIndex, 36)
        name2Tf.text = statusModel.data(currentIndex, 39)
        name3Tf.text = statusModel.data(currentIndex, 40)
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
            horizontalAlignment: Text.AlignLeft
            anchors.top: parent.top
            color: "#ffffff"
        }

        TextField {
            id: name1Tf
            width: 240
            height: 50
            placeholderText: qsTr("Finnish name")
            text: statusModel.data(currentIndex, 36)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            onTextChanged: {
                statusEdited = true
            }
        }

        TextField {
            id: name2Tf
            width: 240
            height: 50
            placeholderText: qsTr("Swedish name")
            text: statusModel.data(currentIndex, 39)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: name1Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                statusEdited = true
            }
        }

        TextField {
            id: name3Tf
            placeholderText: qsTr("English name")
            text: statusModel.data(currentIndex, 40)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: name2Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                statusEdited = true
            }
        }

        TextField {
            id: abbrevTf
            placeholderText: qsTr("Abbreviation")
            text: statusModel.data(currentIndex, 35)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: name3Tf.bottom
            anchors.topMargin: 8
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
            onTextChanged: {
                statusEdited = true
            }
        }
    }
}
