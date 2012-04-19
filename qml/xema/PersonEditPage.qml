import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript



Page {
    id: personEditPage
    tools: editToolBar
    property bool personEdited: false
    property bool addingNew: false

    ToolBarLayout {
        id: editToolBar
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
                console.log("pep sivu back clicked")
                if( personEdited == true )
                {
                    if( addingNew == true )
                    {
                        console.log("adding new")
                        if( !mandatoryInfoExists() )
                        {
                            pageStack.pop()
                            return
                        }
                        var rows = personModel.rowCount()
                        personModel.setData( rows, registeredChkBox.checked, 36 )
                        personModel.setData( rows, firstName.text, 38 )
                        personModel.setData( rows, lastName.text, 39 )
                        if( defaultChkBox.checked == true || rows == 0 )
                        {
                            MyScript.checkAndSetDefaultPerson( rows )
                        }
                    }
                    else
                    {
                        if( defaultChkBox.checked == true )
                        {
                            console.log("asetetaan defaultiksi: " + personModel.data( currentIndex, 35 ))
                            MyScript.checkAndSetDefaultPerson( currentIndex )
                        }
                        personModel.setData( currentIndex, registeredChkBox.checked, 36 )
                        personModel.setData( currentIndex, firstName.text, 38 )
                        personModel.setData( currentIndex, lastName.text, 39 )
                    }
                }
                pageStack.pop()
            }
        }/*
        ToolButton {
            flat: true
            iconSource: "toolbar-delete"
            onClicked: {
                console.log( "tee person delete jutut")
                personModel.removeRow( currentIndex );
                pageStack.pop()
            }
        }*/
    }

    property int currentIndex: -1

    function mandatoryInfoExists()
    {
        var dataOk = false;
        if( firstName.text.length > 0 )
        {
            dataOk = true;
        }
        return dataOk;
    }

    function addNew()
    {
        console.log("adding newjoo")
        addingNew = true
    }

    function selectedIndex( index )
    {
        currentIndex = index
        console.log( "valittu: " + personModel.data( currentIndex, 35 ))
        personEdited = false
    }

    Flickable {
        contentWidth: width
        contentHeight: defaultChkBox.y + defaultChkBox.height + 10
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent

        Text {
            id: editText
            text: qsTr( "Edit person")
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
            id: firstName
            width: 240
            height: 50
            placeholderText: qsTr( "First name" )
            text: personModel.data( currentIndex, 38 )
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            onTextChanged: {
                personEdited = true
            }
        }

        TextField {
            id: lastName
            width: 240
            height: 50
            placeholderText: qsTr( "Last name" )
            text: personModel.data( currentIndex, 39 )
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: firstName.bottom
            anchors.topMargin: 8
            onTextChanged: {
                personEdited = true
            }
        }

        CheckBox {
            id: registeredChkBox
            text: qsTr( "Registered")
            checked: personModel.data( currentIndex, 36 )
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: lastName.bottom
            anchors.topMargin: 8
            onCheckedChanged: {
                personEdited = true
            }
        }

        CheckBox {
            id: defaultChkBox
            text: qsTr( "Default" )
            checked: personModel.data( currentIndex, 37 ) || personModel.rowCount() == 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: registeredChkBox.bottom
            anchors.topMargin: 8
            enabled: personModel.rowCount() !== 0
            onCheckedChanged: {
                personEdited = true
            }
        }
    }
}
