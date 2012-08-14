import QtQuick 1.1
import com.nokia.meego 1.1


Page {
    id: historyPage
    property bool dateListVisible: historyListView.model == historyDateModel ? true : false
    property bool placeListVisible: historyListView.model == historyPlaceModel ? true : false

    tools: historyToolBarLayout

    ToolBarLayout {
        id: historyToolBarLayout
        ToolIcon {
            iconId: "icon-m-toolbar-back-white"
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
//            iconSource: "toolbar-share" //qml/symbian3_icons/save.svg"
            text: qsTr("Export")
            visible: historyListView.model == historyDateModel
            onClicked: {
                exportDialog.open()
            }
        }
        ToolButton {
            flat: true
//            iconSource: "toolbar-filter"
            text: qsTr("import")
            visible: historyListView.model == historyDateModel
            onClicked: {
                window.importData()
            }
        }
    }

    ContextMenu {
        id: contextMenu
        property int selectedItem: -1
        property string selectedDate: ""
        property string selectedPlace: ""
        MenuLayout {
            MenuItem {
                id: removeItem
                text: qsTr("Remove")
                onClicked: {
                    console.log("REMOVE " + contextMenu.selectedItem)
                    removeItem.enabled = false
                    historyToolBarLayout.enabled = false
                    window.deleteObs(contextMenu.selectedItem, contextMenu.selectedDate, contextMenu.selectedPlace)
                    removeItem.enabled = true
                    historyToolBarLayout.enabled = true
                    contextMenu.close()
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
        console.log("clicked")
        //pageStack.pop()
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

    function findBirdFromAbbrev(name)
    {
        console.log("findBirdFromAbbrev: " + name)
        for(var i=0;i<birdModel.rowCount();i++) {
            // TODO localized names
            if (currentLanguage == "en") {
                if(name === birdModel.data(i, 38)) {
                    return birdModel.data(i, 44)
                }
            }
            else if (currentLanguage == "sv") {
                if(name === birdModel.data(i, 38)) {
                    return birdModel.data(i, 46)
                }
            }
            else {
                if(name === birdModel.data(i, 38)) {
                    return birdModel.data(i, 36)
                }
            }
        }

        return name
    }

    Dialog {
        id: exportDialog

        property int step: 1
        property bool onlyNew: false

        title: Label {
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: qsTr("Export")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        content:Item {
            height: 100
            width: parent.width
            anchors.margins: 10
            Label {
                id: exportDialogText
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: exportDialog.step == 1 ? qsTr("Do you want to export all data, or just new data?") : qsTr("Which delimiter to use?")
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 20
            }
        }

        buttons: Item { height: exportDialogAllButton.height + 2 * 20; width: parent.width - 20
            Button {
                id: exportDialogAllButton
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 5
                width: parent.width / 2
                text: exportDialog.step == 1 ? qsTr("All") : ("#")
                onClicked: {
                    if( exportDialog.step == 1 ) {
                        exportDialog.onlyNew = false
                        exportDialog.step = 2
                        return
                    }
                    console.log("all")
                    window.exportData(exportDialog.onlyNew,"#")
                    exportDialog.close()
                    exportDialog.step = 1
                }
            }
            Button {
                id: exportDialogNewButton
                anchors.bottom: parent.bottom
                anchors.left: exportDialogAllButton.right
                anchors.margins: 5
                width: parent.width / 2
                text: exportDialog.step == 1 ? qsTr("New") : (";")
                onClicked: {
                    if( exportDialog.step == 1 ) {
                        exportDialog.onlyNew = true
                        exportDialog.step = 2
                        return
                    }

                    console.log("new")
                    window.exportData(exportDialog.onlyNew,";")
                    exportDialog.close()
                    exportDialog.step = 1
                }
            }
        }
//        onClickedOutside: exportDialog.close()

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
        section.criteria: ViewSection.FullString
        section.property: model == historyDateModel ? "section" : ""
        section.delegate: model == historyDateModel ? sectionDelegate : emptySectionDelegate
    }

    Component {
        id: emptySectionDelegate
        Item {
            width: historyListView.width
            height: 0
        }
    }

    Component {
        id: sectionDelegate
        Item {
            width: historyListView.width
            height: 40
            Label {
                id: sectionLabel
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: section
                font.bold: true

            }
            Rectangle {
                anchors.top: sectionLabel.bottom
                height: 1
                width: parent.width
            }
        }
    }
/*
    SectionScroller {
        listView: historyListView
    }
*/
    HistoryListDelegate {
        id: myTestDelegate

    }
}
