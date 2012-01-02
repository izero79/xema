import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript



Page {
    id: locationEditPage
    tools: editToolBar
    property bool locationEdited: false

    ToolBarLayout {
        id: editToolBar
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
                console.log("pep sivu back clicked")
                if( locationEdited == true )
                {
                    locationModel.setData( currentIndex, townTf.text, 35 )
                    locationModel.setData( currentIndex, locationTf.text, 36 )
                    locationModel.setData( currentIndex, ykjTf.text, 38 )
                    locationModel.setData( currentIndex, wgsTf.text, 39 )
                }
                pageStack.pop()
            }
        }
    }

    property int currentIndex: -1

    function selectedIndex( index )
    {
        currentIndex = index
        console.log( "valittu: " + locationModel.data( currentIndex, 35 ))
    }

    Text {
        id: editText
        text: qsTr( "Muokkaa paikkaa")
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
        id: townTf
        width: 240
        height: 50
        placeholderText: qsTr( "Town" )
        text: locationModel.data( currentIndex, 35 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: editText.bottom
        anchors.topMargin: 8
        onTextChanged: {
            locationEdited = true
        }
    }

    TextField {
        id: locationTf
        width: 240
        height: 50
        placeholderText: qsTr( "Location" )
        text: locationModel.data( currentIndex, 36 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: townTf.bottom
        anchors.topMargin: 8
        onTextChanged: {
            locationEdited = true
        }
    }

    TextField {
        id: ykjTf
        placeholderText: qsTr( "YKJ coordinates")
        text: locationModel.data( currentIndex, 38 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: locationTf.bottom
        anchors.topMargin: 8
        onTextChanged: {
            locationEdited = true
        }
    }

    TextField {
        id: wgsTf
        placeholderText: qsTr( "WGS coordinates" )
        text: locationModel.data( currentIndex, 39 )
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: ykjTf.bottom
        anchors.topMargin: 8
        onTextChanged: {
            locationEdited = true
        }
    }
}
