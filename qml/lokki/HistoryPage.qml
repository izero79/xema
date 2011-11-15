import QtQuick 1.0
import com.nokia.symbian 1.1


Page {
    id: historyPage

    function clicked( name )
    {
        pageStack.pop()
        window.showObsPage()
        window.readObs( name )
    }

    ListModel {
        id: testModelHistory
        ListElement {
            name: "1.1.2011, Kokkola, 1 laji"
            obsId: 1
        }
        ListElement {
            name: "24.12.2010, Lohtaja, 1 laji"
            obsId: 2
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
