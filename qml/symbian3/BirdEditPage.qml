import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript



Page {
    id: birdEditPage
    tools: editToolBar
    property bool birdEdited: false
    property bool addingNew: false

    ToolBarLayout {
        id: editToolBar
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
//                console.log("bird edit page back clicked")
                if (birdEdited == true)
                {
                    if (addingNew == true)
                    {
                        console.log("adding new bird")
                        if (!mandatoryInfoExists())
                        {
                            pageStack.pop()
                            return
                        }

                        var rows = birdModel.rowCount()
                        birdModel.setData(rows, name1Tf.text, 36)
                        birdModel.setData(rows, name2Tf.text, 37)
                        birdModel.setData(rows, name3Tf.text, 39)
                        birdModel.setData(rows, name4Tf.text, 44)
                        birdModel.setData(rows, group4Tf.text, 45)
                        birdModel.setData(rows, group1Tf.text, 40)
                        birdModel.setData(rows, group2Tf.text, 41)
                        birdModel.setData(rows, group3Tf.text, 42)
                        birdModel.setData(rows, abbrevTf.text, 38)
                        birdModel.setData(rows, categoryTf.text, 43)
                    }
                    else
                    {
                        birdModel.setData(currentIndex, name1Tf.text, 36)
                        birdModel.setData(currentIndex, name2Tf.text, 37)
                        birdModel.setData(currentIndex, name3Tf.text, 39)
                        birdModel.setData(currentIndex, group1Tf.text, 40)
                        birdModel.setData(currentIndex, group2Tf.text, 41)
                        birdModel.setData(currentIndex, group3Tf.text, 42)
                        birdModel.setData(currentIndex, abbrevTf.text, 38)
                        birdModel.setData(currentIndex, categoryTf.text, 43)
                        birdModel.setData(currentIndex, name4Tf.text, 44)
                        birdModel.setData(currentIndex, group4Tf.text, 45)
                    }
                }
                pageStack.pop()
            }
        }/*
        ToolButton {
            flat: true
            iconSource: "toolbar-delete"
            onClicked: {
                console.log("tee bird delete jutut")
                birdModel.removeRow(currentIndex);
                pageStack.pop()
            }
        }*/
    }

    property int currentIndex: -1

    function mandatoryInfoExists()
    {
        var dataOk = false;
        if (name1Tf.text.length > 0 &&
            name3Tf.text.length > 0 &&
            group1Tf.text.length > 0 &&
            group3Tf.text.length > 0 &&
            abbrevTf.text.length > 0 &&
            categoryTf.text.length > 0)
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
        birdEdited = false
    }

    Flickable {
        id: flickable1
        contentWidth: width
        contentHeight: idTf.y + idTf.height + 10
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent

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

        Text {
            id: editText
            text: qsTr("Edit species")
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
            text: birdModel.data(currentIndex, 36)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }

        TextField {
            id: name2Tf
            width: 240
            height: 50
            placeholderText: qsTr("Swedish name")
            text: birdModel.data(currentIndex, 46)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: name1Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }

        TextField {
            id: name4Tf
            width: 240
            height: 50
            placeholderText: qsTr("English name")
            text: birdModel.data(currentIndex, 48)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: name2Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }

        TextField {
            id: name3Tf
            placeholderText: qsTr("Latin name")
            text: birdModel.data(currentIndex, 39)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: name4Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }

        TextField {
            id: group1Tf
            placeholderText: qsTr("Group in finnish")
            text: birdModel.data(currentIndex, 40)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: name3Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: group2Tf
            placeholderText: qsTr("Group in swedish")
            text: birdModel.data(currentIndex, 47)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: group1Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: group4Tf
            placeholderText: qsTr("Group in english")
            text: birdModel.data(currentIndex, 49)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: group2Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: group3Tf
            placeholderText: qsTr("Group in latin")
            text: birdModel.data(currentIndex, 42)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: group4Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: abbrevTf
            placeholderText: qsTr("Abbreviation")
            text: birdModel.data(currentIndex, 38)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: group3Tf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: categoryTf
            placeholderText: qsTr("Category")
            text: birdModel.data(currentIndex, 43)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: abbrevTf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: idTf
            placeholderText: qsTr("Id number")
            enabled: false
            text: addingNew ? birdModel.rowCount() + 1 : birdModel.data(currentIndex, 35)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: categoryTf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }
    }
}
