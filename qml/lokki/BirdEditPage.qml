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
                console.log("pep sivu back clicked")
                if( birdEdited == true )
                {
                    if( addingNew == true )
                    {
                        console.log("adding new")
                        var rows = birdModel.rowCount()
                        birdModel.setData( rows, name1Tf.text, 36 )
                        birdModel.setData( rows, name2Tf.text, 37 )
                        birdModel.setData( rows, name3Tf.text, 39 )
                        birdModel.setData( rows, group1Tf.text, 40 )
                        birdModel.setData( rows, group2Tf.text, 41 )
                        birdModel.setData( rows, group3Tf.text, 42 )
                        birdModel.setData( rows, abbrevTf.text, 38 )
                        birdModel.setData( rows, categoryTf.text, 43 )
                    }
                    else
                    {
                        birdModel.setData( currentIndex, name1Tf.text, 36 )
                        birdModel.setData( currentIndex, name2Tf.text, 37 )
                        birdModel.setData( currentIndex, name3Tf.text, 39 )
                        birdModel.setData( currentIndex, group1Tf.text, 40 )
                        birdModel.setData( currentIndex, group2Tf.text, 41 )
                        birdModel.setData( currentIndex, group3Tf.text, 42 )
                        birdModel.setData( currentIndex, abbrevTf.text, 38 )
                        birdModel.setData( currentIndex, categoryTf.text, 43 )
                    }
                }
                pageStack.pop()
            }
        }
        ToolButton {
            flat: true
            iconSource: "toolbar-delete"
            onClicked: {
                console.log( "tee bird delete jutut")
                birdModel.removeRow( currentIndex );
                pageStack.pop()
            }
        }
    }

    property int currentIndex: -1

    function addNew()
    {
        console.log("adding newjoo")
        addingNew = true
    }

    function selectedIndex( index )
    {
        currentIndex = index
        console.log( "valittu: " + birdModel.data( currentIndex, 36 ))
        birdEdited = false
    }

    Text {
        id: editText
        text: qsTr( "Muokkaa lajia")
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
        placeholderText: qsTr( "Lajinimi" )
        text: birdModel.data( currentIndex, 36 )
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
        placeholderText: qsTr( "namn" )
        text: birdModel.data( currentIndex, 37 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: name1Tf.bottom
        anchors.topMargin: 8
        onTextChanged: {
            birdEdited = true
        }
    }

    TextField {
        id: name3Tf
        placeholderText: qsTr( "latin name")
        text: birdModel.data( currentIndex, 39 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: name2Tf.bottom
        anchors.topMargin: 8
        onTextChanged: {
            birdEdited = true
        }
    }

    TextField {
        id: group1Tf
        placeholderText: qsTr( "ryhma" )
        text: birdModel.data( currentIndex, 40 )
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
        placeholderText: qsTr( "gruppen" )
        text: birdModel.data( currentIndex, 41 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: group1Tf.bottom
        anchors.topMargin: 8
        onTextChanged: {
            birdEdited = true
        }
    }
    TextField {
        id: group3Tf
        placeholderText: qsTr( "latin group" )
        text: birdModel.data( currentIndex, 42 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: group2Tf.bottom
        anchors.topMargin: 8
        onTextChanged: {
            birdEdited = true
        }
    }
    TextField {
        id: abbrevTf
        placeholderText: qsTr( "lyhenne" )
        text: birdModel.data( currentIndex, 38 )
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
        placeholderText: qsTr( "kategoria" )
        text: birdModel.data( currentIndex, 43 )
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
        placeholderText: qsTr( "id" )
        enabled: false
        text: addingNew ? birdModel.rowCount() + 1 : birdModel.data( currentIndex, 35 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: categoryTf.bottom
        anchors.topMargin: 8
        onTextChanged: {
            birdEdited = true
        }
    }
}
