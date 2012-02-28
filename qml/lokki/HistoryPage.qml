import QtQuick 1.1
import com.nokia.symbian 1.1


Page {
    id: historyPage
    property bool dateListVisible: historyListView.model == historyDateModel ? true : false
    property bool placeListVisible: historyListView.model == historyPlaceModel ? true : false

    tools: historyToolBarLayout

    ToolBarLayout {
        id: historyToolBarLayout
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
                if( dateListVisible == true )
                {
                    window.backFromList()
                    pageStack.pop()
                }
                else if( placeListVisible == true )
                {
                    historyListView.model = historyDateModel
                    textfield1.text = ""
                }
                else
                {
                    historyListView.model = historyPlaceModel
                    textfield1.text = ""
                }
            }
        }
    }


    function clicked( name )
    {
        pageStack.pop()
        window.showObsPage()
        window.readObs( name )
    }

    function showDate( pvm )
    {
        window.loadHistoryWithDate( pvm )
        //textfield1.text = ""
        console.log("showDate " + pvm)
        historyListView.model = historyPlaceModel
        historyPlaceModel.filter( textfield1.text )
    }

    function showPlace( place, pvm )
    {
        window.loadHistoryWithDateAndPlace( pvm, place )
        //textfield1.text = ""
        console.log("showDate " + place + " pvm " +pvm)
        historyListView.model = historyModel

//        var filterString = place + ", " + pvm
        historyModel.filter( textfield1.text )
    }

    function init()
    {
        historyListView.model = historyDateModel
        textfield1.text = ""
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
                historyListView.model.filter( text )
            }
        }
    }
    ListView {
        id: historyListView
        model: historyDateModel
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
