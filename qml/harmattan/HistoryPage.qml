import QtQuick 1.1
import com.nokia.meego 1.0


Page {
    id: historyPage
    property bool dateListVisible: historyListView.model == historyDateModel ? true : false
    property bool placeListVisible: historyListView.model == historyPlaceModel ? true : false
    property string currentDate: ""
    property string currentPlace: ""

    tools: historyToolBarLayout

    ToolBarLayout {
        id: historyToolBarLayout
        ToolIcon {

            platformIconId: "toolbar-back"
            visible: dateListVisible != true
            onClicked: {
                if (dateListVisible == true)
                {
                    currentPlace = ""
                    currentDate = ""
                    window.backFromList()
                    pageStack.depth <= 1 ? quit() : pageStack.pop()
                }
                else if (placeListVisible == true)
                {
                    currentDate = ""
                    currentPlace = ""
                    historyListView.model = historyDateModel
                    filterTf.text = ""
                }
                else
                {
                    currentPlace = ""
                    historyListView.model = historyPlaceModel
                    filterTf.text = ""
                }
            }
        }
        ToolIcon {

            platformIconId: "toolbar-add"
            onClicked: {
                window.showObsPage(false)
            }
        }
        ToolButton {
            flat: true
            text: qsTr("Export")
            onClicked: {
                exportDialog.open()
            }
        }
        ToolIcon {

            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (viewMenu.status === DialogStatus.Closed) ? viewMenu.open() : viewMenu.close()
        }
    }

    Menu {
        id: viewMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Export to Tiira")
                visible: window.useTiira && window.tiiraLoginOk
                onClicked: {
                    tiiraExportDialog.open()
                }
            }
            MenuItem {
                text: qsTr("Import")
                visible: true
                onClicked: {
                    window.importData()
                }
            }
            MenuItem {
                text: qsTr("Settings")
                visible: true
                onClicked: {
                    window.showSettingsPage()
                }
            }
            MenuItem {
                text: qsTr("About Xema")
                visible: true
                onClicked: {
                    aboutDialog.open()
                }
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
                id: tiiraExportItem
                text: qsTr("Export to Tiira")
                onClicked: {
                    console.log("EXPORT TO TIIRA " + contextMenu.selectedItem)
                    tiiraExportItem.enabled = false
                    historyToolBarLayout.enabled = false
                    window.exportObsToTiira(contextMenu.selectedItem)
                    tiiraExportItem.enabled = true
                    historyToolBarLayout.enabled = true
                    contextMenu.close()
                }
                visible: window.useTiira && window.tiiraLoginOk
            }
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
        window.showObsPage(false)
        window.readObs(name)
    }

    function newObsWithData(date, place, species)
    {
        window.showObsPage(false)
        window.newObsWithData(date, place, species)
    }

    function showDate(pvm)
    {
        window.loadHistoryWithDate(pvm)
        currentDate = pvm
        historyListView.model = historyPlaceModel
        historyPlaceModel.filter(filterTf.text)
    }

    function showPlace(place, pvm)
    {
        currentDate = pvm
        currentPlace = place
        window.loadHistoryWithDateAndPlace(pvm, place)
        historyListView.model = historyModel

        historyModel.filter(filterTf.text)
    }

    function init()
    {
        historyListView.model = historyDateModel
        filterTf.text = ""
    }

    function findBirdFromAbbrev(name)
    {
        console.log("findBirdFromAbbrev: " + name)
        for(var i=0;i<birdModel.rowCount();i++) {
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


    Loader {
        id: aboutDialog

        function open()
        {
            source = Qt.resolvedUrl( "AboutDialog.qml" )
            if( item != null )
            {
                item.screenX = -x
                item.screenY = -y
                item.open()
            }
        }

        function close()
        {
            if( item != null )
            {
                item.close()
            }
            source = ""
        }

        function isVisible()
        {
            if( item != null )
            {
                return item.isVisible
            }
            return false
        }

        anchors.centerIn: parent
        width: window.inPortrait ? parent.width / 3 * 2: parent.width / 5 * 2
        height: window.inPortrait ? parent.height / 6 * 2 : parent.height / 5 * 3
        source: ""
        z: 100
        onYChanged: {
            if( item != null )
            {
                item.screenX = -x
                item.screenY = -y
            }
        }
    }
    Connections {
        target: aboutDialog.item
        onButton1Clicked: {
            aboutDialog.close()
        }
        onCanceled: {
            aboutDialog.close()
        }
        onOpenHomepage: {
            window.openUrl( "http://www.iki.fi/z7/xema" )
        }
    }

    Dialog {
        id: exportDialog

        property int step: 1
        property bool onlyNew: false
        property bool allCountries: true

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
            height: 150
            width: parent.width
            anchors.margins: 10
            Label {
                id: exportDialogText
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: {
                    if (exportDialog.step == 1) {
                        if ( currentDate !== "" && currentPlace !== "") {
                            return qsTr("Exporting records from place %1 with date %2. Do you want to export all data, or just new data?").arg(currentPlace).arg(currentDate)
                        } else if ( currentDate !== "" && currentPlace === "") {
                            return qsTr("Exporting records with date %1. Do you want to export all data, or just new data?").arg(currentDate)
                        } else {
                            return qsTr("Do you want to export all data, or just new data?")
                        }
                    } else if (exportDialog.step == 2) {
                        return qsTr("Records from all countries, or only from default country (") + window.defaultCountry + (")?")
                    } else if (exportDialog.step == 3) {
                        return qsTr("Choose delimiter to be used")
                    }
                }
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
                text: {
                    if (exportDialog.step == 1) {
                        return qsTr("All")
                    } else if (exportDialog.step == 2) {
                        return qsTr("All")
                    } else if (exportDialog.step == 3) {
                        return qsTr("#")
                    }
                }
                onClicked: {
                    if( exportDialog.step == 1 ) {
                        exportDialog.onlyNew = false
                        exportDialog.step = 2
                        return
                    } else if( exportDialog.step == 2 ) {
                        exportDialog.allCountries = true
                        exportDialog.step = 3
                        return
                    }

                    console.log("all")
                    window.exportData(exportDialog.onlyNew, exportDialog.allCountries, currentDate, currentPlace, "#")
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
                text: {
                    if (exportDialog.step == 1) {
                        return qsTr("New")
                    } else if (exportDialog.step == 2) {
                        return qsTr("Default")
                    } else if (exportDialog.step == 3) {
                        return qsTr(";")
                    }
                }
                onClicked: {
                    if( exportDialog.step == 1 ) {
                        exportDialog.onlyNew = true
                        exportDialog.step = 2
                        return
                    } else if( exportDialog.step == 2 ) {
                        exportDialog.allCountries = false
                        exportDialog.step = 3
                        return
                    }

                    console.log("new")
                    window.exportData(exportDialog.onlyNew, exportDialog.allCountries, currentDate, currentPlace, ";")
                    exportDialog.close()
                    exportDialog.step = 1
                }
            }
        }
        onRejected: { 
            exportDialog.step = 1

        }
    }

    Dialog {
        id: tiiraExportDialog

        title: Label {
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: qsTr("Tiira export")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        content:Item {
            height: 150
            width: parent.width
            anchors.margins: 10
            Label {
                id: tiiraExportDialogText
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: {
                    if ( currentDate !== "" && currentPlace !== "") {
                        return qsTr("All the records with date %1 and place %2 that haven\'t been exported to Tiira from Xema will be now exported.").arg(currentPlace).arg(currentDate)
                    } else if ( currentDate !== "" && currentPlace === "") {
                        return qsTr("All the records with date %1 that haven't been exported to Tiira from Xema will be now exported.").arg(currentDate)
                    } else {
                        return qsTr("All the records that haven't been exported to Tiira from Xema will be now exported.")
                    }
                }
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 20
            }
        }

        buttons: Item { height: tiiraExportDialogOkButton.height + 2 * 20; width: parent.width - 20
            Button {
                id: tiiraExportDialogOkButton
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 5
                width: parent.width / 2
                text: qsTr("Ok")
                onClicked: {
                    window.exportToTiira(currentDate, currentPlace)
                    tiiraExportDialog.close()
                }
            }
        }
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
            id: filterTf
            height: 50
            placeholderText: qsTr("Search")
            text: ""
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            inputMethodHints: Qt.ImhNoPredictiveText
            onTextChanged: {
                //console.log("teksti muuttuu: " + text)
                historyListView.model.filter(text)
            }
            Image {
                anchors { top: parent.top; right: parent.right; margins: 1 }
                id: clearText
                fillMode: Image.PreserveAspectFit
                smooth: true;
                source: ":../harmattan_icons/icon-m-input-clear.png"
                height: 48 //parent.height - 8 * 2
                width: 48 //parent.height - 8 * 2
                visible: filterTf.text

                MouseArea {
                    id: clear
                    anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                    height: filterTf.height; width: filterTf.height
                    onClicked: {
                        filterTf.text = ""
                        filterTf.forceActiveFocus()
                    }
                }
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
