import QtQuick 1.0
import com.nokia.symbian 1.0


Page {
    id: listPage

    function clicked( name )
    {
        if( listPageType == "birds" )
        {
            window.birdChanged( name )
        }
        else if( listPageType == "places" )
        {
            window.placeChanged( name )
        }
        else if( listPageType == "regpeople")
        {
            window.regpeopleChanged( name )
        }
        else if( listPageType == "people")
        {
            window.peopleChanged( name )
        }
        pageStack.pop()
    }

    ListModel {
        id: testModelHistory
        ListElement {
            name: "1.1.2011, Kokkola, 3 lajia"
        }
        ListElement {
            name: "24.12.2010, Lohtaja, 1 laji"
        }
        ListElement {
            name: "12.11.2010, Helsinki, 2 laji"
        }
    }

    Item {
        id: item1
        width: parent.width
        height: 50
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 8

        TextField {
            id: textfield1
            height: 50
            text: listPageType
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            onTextChanged: {
                console.log("teksti muuttuu: " + text)
            }
        }
    }
    ListView {
        model: testModelHistory
        anchors.top: item1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        delegate: myTestDelegate
        clip: true
    }
    HistoryListDelegate {
        id: myTestDelegate

    }
}
