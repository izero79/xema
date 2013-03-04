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
                        birdModel.setData(rows, finNameTf.text, 36)
                        birdModel.setData(rows, sweNameTf.text, 37)
                        birdModel.setData(rows, latinNameTf.text, 39)
                        birdModel.setData(rows, engNameTf.text, 44)
                        birdModel.setData(rows, engGroupTf.text, 45)
                        birdModel.setData(rows, finGroupTf.text, 40)
                        birdModel.setData(rows, sweGroupTf.text, 41)
                        birdModel.setData(rows, latinGroupTf.text, 42)
                        birdModel.setData(rows, abbrevTf.text, 38)
                        birdModel.setData(rows, categoryTf.text, 43)
                        birdModel.setData(rows, true, 54)
                    }
                    else
                    {
                        birdModel.setData(currentIndex, finNameTf.text, 36)
                        birdModel.setData(currentIndex, sweNameTf.text, 37)
                        birdModel.setData(currentIndex, latinNameTf.text, 39)
                        birdModel.setData(currentIndex, finGroupTf.text, 40)
                        birdModel.setData(currentIndex, sweGroupTf.text, 41)
                        birdModel.setData(currentIndex, latinGroupTf.text, 42)
                        birdModel.setData(currentIndex, abbrevTf.text, 38)
                        birdModel.setData(currentIndex, categoryTf.text, 43)
                        birdModel.setData(currentIndex, engNameTf.text, 44)
                        birdModel.setData(currentIndex, engGroupTf.text, 45)
                        birdModel.setData(currentIndex, true, 54)
                    }
                    if ( currentLanguage == "sv") {
                        window.newBirdCreated( sweNameTf.text + ", " + latinNameTf.text )
                    }
                    else if ( currentLanguage == "en") {
                        window.newBirdCreated( engNameTf.text + ", " + latinNameTf.text )
                    }
                    else {
                        window.newBirdCreated( finNameTf.text + ", " + latinNameTf.text )
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
                console.log("bird edit page undo clicked")
                undo()
            }
        }
    }

    property int currentIndex: -1

    function mandatoryInfoExists()
    {
        var dataOk = false;
        if (currentLanguage == "en" && engNameTf.text.length > 0 &&
            abbrevTf.text.length > 0 )
        {
            dataOk = true;
        }
        else if (currentLanguage == "sv" && sweNameTf.text.length > 0 &&
            abbrevTf.text.length > 0 )
        {
            dataOk = true;
        }
        else if (finNameTf.text.length > 0 &&
            abbrevTf.text.length > 0 )
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

    function undo()
    {
        finNameTf.text = birdModel.data(currentIndex, 36)
        sweNameTf.text = birdModel.data(currentIndex, 46)
        latinNameTf.text = birdModel.data(currentIndex, 39)
        engNameTf.text = birdModel.data(currentIndex, 48)
        finGroupTf.text = birdModel.data(currentIndex, 40)
        sweGroupTf.text = birdModel.data(currentIndex, 47)
        latinGroupTf.text = birdModel.data(currentIndex, 42)
        engGroupTf.text = birdModel.data(currentIndex, 49)
        abbrevTf.text = birdModel.data(currentIndex, 38)
        categoryTf.text = birdModel.data(currentIndex, 43)
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
            id: finNameTf
            width: parent.width
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
            id: sweNameTf
            width: parent.width
            height: 50
            placeholderText: qsTr("Swedish name")
            text: birdModel.data(currentIndex, 46)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: finNameTf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }

        TextField {
            id: engNameTf
            width: parent.width
            height: 50
            placeholderText: qsTr("English name *")
            text: birdModel.data(currentIndex, 48)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: sweNameTf.bottom
            anchors.topMargin: 8
            onTextChanged: {
                birdEdited = true
            }
        }

        TextField {
            id: latinNameTf
            placeholderText: qsTr("Latin name")
            text: birdModel.data(currentIndex, 39)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: engNameTf.bottom
            anchors.topMargin: 8
            width: parent.width
            onTextChanged: {
                birdEdited = true
            }
        }

        TextField {
            id: finGroupTf
            placeholderText: qsTr("Group in finnish")
            text: birdModel.data(currentIndex, 40)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: latinNameTf.bottom
            anchors.topMargin: 8
            width: parent.width
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: sweGroupTf
            placeholderText: qsTr("Group in swedish")
            text: birdModel.data(currentIndex, 47)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: finGroupTf.bottom
            anchors.topMargin: 8
            width: parent.width
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: engGroupTf
            placeholderText: qsTr("Group in english")
            text: birdModel.data(currentIndex, 49)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: sweGroupTf.bottom
            anchors.topMargin: 8
            width: parent.width
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: latinGroupTf
            placeholderText: qsTr("Group in latin")
            text: birdModel.data(currentIndex, 42)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: engGroupTf.bottom
            anchors.topMargin: 8
            width: parent.width
            onTextChanged: {
                birdEdited = true
            }
        }
        TextField {
            id: abbrevTf
            placeholderText: qsTr("Abbreviation *")
            text: birdModel.data(currentIndex, 38)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: latinGroupTf.bottom
            anchors.topMargin: 8
            width: parent.width
            validator: RegExpValidator{ regExp: /.{1,}/ }
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
            width: parent.width
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
            width: parent.width
            visible: false
            onTextChanged: {
                birdEdited = true
            }
        }
    }
}
