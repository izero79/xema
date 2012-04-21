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
                if (dateListVisible == true)
                {
                    window.backFromList()
                    pageStack.pop()
                }
                else if (placeListVisible == true)
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
        ToolButton {
            flat: true
            iconSource: "toolbar-share" //qml/s3icons/save.svg"
            onClicked: {
                exportDialog.open()
            }
        }/*
        ToolButton {
            flat: true
            iconSource: "toolbar-filter"
            onClicked: {
                window.importData()
            }
        }*/
    }

    ContextMenu {
        id: contextMenu
        property int selectedItem: -1
        property string selectedDate: ""
        property string selectedPlace: ""
        MenuLayout {
            MenuItem {
                text: qsTr("Remove")
                onClicked: {
                    console.log("REMOVE " + contextMenu.selectedItem)
                    window.deleteObs(contextMenu.selectedItem, contextMenu.selectedDate, contextMenu.selectedPlace)
                }
            }
        }
    }

    function showContextMenu(itemid, place, date)
    {
        contextMenu.selectedItem = itemid
        contextMenu.selectedPlace = place
        contextMenu.selectedDate = date
        contextMenu.open()
    }

    function clicked(name)
    {
        pageStack.pop()
        window.showObsPage(false)
        window.readObs(name)
    }

    function newObsWithData(date, place, species)
    {
        pageStack.pop()
        window.showObsPage(false)
        window.newObsWithData(date, place, species)
    }

    function showDate(pvm)
    {
        window.loadHistoryWithDate(pvm)
        //textfield1.text = ""
        console.log("showDate " + pvm)
        historyListView.model = historyPlaceModel
        historyPlaceModel.filter(textfield1.text)
    }

    function showPlace(place, pvm)
    {
        window.loadHistoryWithDateAndPlace(pvm, place)
        //textfield1.text = ""
        console.log("showDate " + place + " pvm " +pvm)
        historyListView.model = historyModel

//        var filterString = place + ", " + pvm
        historyModel.filter(textfield1.text)
    }

    function init()
    {
        historyListView.model = historyDateModel
        textfield1.text = ""
    }

    Dialog {
        id: exportDialog

        title: Text {
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: qsTr("Export")
            horizontalAlignment: Text.AlignHCenter
        }
        content:Item {
            height: exportDialogText.paintedHeight
            width: parent.width
            anchors.topMargin: 10
            Text {
                id: exportDialogText
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: qsTr("Do you want to export all data, or just new data?")
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 20
            }
        }

        buttons: Item { height: exportDialogAllButton.height + 2 * 10; width: parent.width
            Button {
                id: exportDialogAllButton
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width / 2
                text: qsTr("All")
                onClicked: {
                    console.log("all")
                    window.exportData(false)
                    exportDialog.close()
                }
            }
            Button {
                id: exportDialogNewButton
                anchors.top: parent.top
                anchors.left: exportDialogAllButton.right
                width: parent.width / 2
                text: qsTr("New")
                onClicked: {
                    console.log("new")
                    window.exportData(true)
                    exportDialog.close()
                }
            }
        }
        onClickedOutside: exportDialog.close()

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
                historyListView.model.filter(text)
            }
        }
    }

    ScrollDecorator {
        id: scrolldecorator
        flickableItem: historyListView
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
