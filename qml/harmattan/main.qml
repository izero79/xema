import QtQuick 1.1
import com.nokia.meego 1.0
import "myjs.js" as MyScript
import XemaEnums 1.0

PageStackWindow {
    id: window

    property string listPageType: ""
    property bool useSystematicSort: false
    property bool wpSpecies: true
    property int defaultDetailLevel: 1
    property int currentDetailLevel: 1
    property bool unsavedData: false
    property string currentLanguage: ""
    property bool compassSupported: false
    property string appName: "Xema"
    property string versionString: "0.0.0"
    property bool cppProcessing: true
    property bool needsHistoryReload: false
    property bool onlyDefaultCountry: false
    property string defaultCountry: ""
    property bool exportWgs: true

    signal writeNew(string data)
    signal readObs(string id)
    signal deleteObs(string id, string date, string place)
    signal reloadHistory()
    signal saveSystematicSorting(bool systematic)
    signal saveWPSpecies(bool wpspecies)
    signal saveDetailLevel(int level)
    signal saveDefaultCountry(string defaultCountry)
    signal saveOnlyDefaultCountry(bool onlyDefault)
    signal saveExportWgs(bool exportWgs)
    signal quit()
    signal loadHistoryWithDate(string date)
    signal loadHistoryWithDateAndPlace(string date, string place)
    signal exportData(bool onlyNew, string delimiter)
    signal restoreSpecies()
    signal restoreStatuses()
    signal restoreLocations()
    signal restoreObservers()
    signal saveLocations()
    signal savePersons()
    signal importData()
    signal exportOwnData()
    signal importOwnData()
    signal openUrl(string url)
    signal clearHistory()
    signal saveBirds()

    signal mousePressedNow;
    signal mouseReleasedNow;
    signal mouseMovedNow(variant deltax, variant deltay);

    // slots
    function doPanMap(deltaX,deltaY)
    {
        MyScript.mapPan(deltaX,deltaY);
    }

    function setSystematicSort(use)
    {
        window.useSystematicSort = use
    }

    function setDetailLevel(level)
    {
        window.defaultDetailLevel = level
        window.currentDetailLevel = level
    }

    function setWpSpecies(wpspecies)
    {
        window.wpSpecies = wpspecies
    }

    function setOnlyDefaultCountry(onlyDefault)
    {
        window.onlyDefaultCountry = onlyDefault
    }

    function setDefaultCountry(defaultCountry)
    {
        window.defaultCountry = defaultCountry
    }

    function setExportWgs(exportWgs)
    {
        window.exportWgs = exportWgs
    }

    function showHistoryPage(type)
    {
        MyScript.showHistoryPage(type)
    }

    function showListPage(type, selectedItems, itemi)
    {
        console.log("showlistpage by: " + itemi + "selecteditems: " + selectedItems)
        MyScript.showListPage(type, selectedItems, itemi)
    }

    function showSettingsPage()
    {
        //pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
        MyScript.showSettingsPage()
    }

    function birdChanged(name)
    {
        MyScript.obsObject.birdChanged(name)
    }

    function placeChanged(name)
    {
        MyScript.obsObject.placeChanged(name)
    }

    function editPeople(index)
    {
        MyScript.settingsObject.editPeople(index)
    }

    function editBird(index)
    {
        MyScript.settingsObject.editBird(index)
    }

    function editStatus(index)
    {
        MyScript.settingsObject.editStatus(index)
    }

    function editLocation(index)
    {
        MyScript.settingsObject.editLocation(index)
    }

    function regpeopleChanged(name)
    {
        MyScript.obsObject.regpeopleChanged(name)
    }

    function peopleChanged(name)
    {
        MyScript.obsObject.peopleChanged(name)
    }

    function sexChanged(name)
    {
        MyScript.fillSexBox(name)
    }

    function atlasChanged(name)
    {
        MyScript.fillAtlasBox(name)
    }

    function dressChanged(name)
    {
        MyScript.fillDressBox(name)
    }

    function directionChanged(name)
    {
        MyScript.fillDirectionBox(name)
    }

    function ageChanged(name)
    {
        MyScript.fillAgeBox(name)
    }

    function showObsPage(doNotInit)
    {
        MyScript.showObsPage(doNotInit)
    }

    function dataLoaded(data)
    {
        console.log("dataLoaded")
        MyScript.obsObject.dataLoaded(data)
    }

    function backFromList()
    {
        if (listPageType == "regpeople")
        {
            MyScript.fillRegPersonBox()
        }
        else if (listPageType == "people")
        {
            MyScript.fillNonRegPersonBox()
        }
        else if (listPageType == "status")
        {
            MyScript.fillStatusBox()
        }
        MyScript.clearListPage()
    }

    function addPeople()
    {
        console.log("add people")
        MyScript.addPeople()
    }

    function addBird()
    {
        console.log("add bird")
        MyScript.addBird()
    }

    function addLocation()
    {
        console.log("add location")
        MyScript.addLocation()
    }

    function addStatus()
    {
        console.log("add status")
        MyScript.addStatus()
    }

    function newLocationCreated( location )
    {
        MyScript.listObject.setSelected( location )
    }

    function newBirdCreated( bird )
    {
        MyScript.listObject.setSelected( bird )
    }

    function newPersonCreated( name )
    {
        MyScript.listObject.setSelected( name )
    }

    function newStatusCreated( name )
    {
        MyScript.listObject.setSelected( name )
    }

    function newObsWithData(date, place, species)
    {

        MyScript.obsObject.setData(date, place, species)
    }

    function showBirdMap(itemi, place_x, place_y, x, y) {
        cppProcessing = true
        MyScript.showMapPage(itemi, "bird", place_x, place_y, x, y)
        cppProcessing = false
    }

    function showPlaceMap(itemi, x, y) {
        cppProcessing = true
        MyScript.showMapPage(itemi, "place", x, y, null, null)
        cppProcessing = false
    }

    function setBirdCoords(coords) {
        MyScript.fillBirdCoords(coords)
    }

    function setPlaceCoords(coords) {
        MyScript.fillLocationCoords(coords)
    }

    initialPage: Qt.resolvedUrl("MainPage.qml")

    function importError(errorNo)
    {
        errorDialog.dialogText = ""
        console.log("importError: " + errorNo)
        if (errorNo == 0) {
            errorDialog.titleText = qsTr("Import complete")
            if (errorNo == XemaEnums.IMPORT_NOERRORS ) {
                errorDialog.dialogText = qsTr("No files imported")
            }
        }
        else {
            errorDialog.titleText = qsTr("Import complete")
            if (errorNo&XemaEnums.IMPORT_LOCATION_OK) {
                errorDialog.dialogText += qsTr("Imported locations") + "\n"
            }
            if (errorNo&XemaEnums.IMPORT_PERSON_OK) {
                errorDialog.dialogText += qsTr("Imported persons") + "\n"
            }
            if (errorNo&XemaEnums.IMPORT_BIRD_OK) {
                errorDialog.dialogText += qsTr("Imported birds") + "\n"
            }
            if (errorNo&XemaEnums.IMPORT_STATUS_OK) {
                errorDialog.dialogText += qsTr("Imported statuses") + "\n"
            }
            if (errorNo&XemaEnums.IMPORT_HISTORY_OK) {
                errorDialog.dialogText += qsTr("Imported history") + "\n"
            }

            if (errorNo&XemaEnums.IMPORT_LOCATIONERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with locations file") + "\n"
            }
            if (errorNo&XemaEnums.IMPORT_PERSONERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with persons file") + "\n"
            }
            if (errorNo&XemaEnums.IMPORT_BIRDERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with birds file") + "\n"
            }
            if (errorNo&XemaEnums.IMPORT_STATUSERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with status file") + "\n"
            }
            if (errorNo&XemaEnums.IMPORT_HISTORYERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with history file") + "\n"
            }
        }
        errorDialog.open()
    }

    function exportDone()
    {
        errorDialog.dialogText = ""
        console.log("exportDone")
        errorDialog.titleText = qsTr("Export complete")
        errorDialog.dialogText = qsTr("Export is complete.")
        errorDialog.open()
    }

    Rectangle {
        id: processingRec
        visible: cppProcessing
        anchors.fill: parent
        color: "#c0000000"

        BusyIndicator {
            id: indicator
            anchors.centerIn: parent
            running: cppProcessing
            platformStyle: BusyIndicatorStyle { size: "large" }
            visible: running
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: indicator.bottom
            anchors.topMargin: 5
            text: qsTr( "Please, wait...")

        }

        MouseArea {
            anchors.fill: parent
            enabled: processingRec.visible
        }
    }

    function clearTab() {
        MyScript.obsObject.clearTab()
//        unsavedData = false
    }

    function save() {
        console.log("taala")
        var success = false
        success = MyScript.readAndSaveData()
        if (success)
        {
            MyScript.obsObject.clearTab()
            MyScript.dataSaved()
            unsavedData = false
            needsHistoryReload = true
        }

    }

    function backFromObs() {
        if (pageStack.currentPage == MyScript.obsObject)
        {
            window.unsavedData = MyScript.unSavedDataExists()
            MyScript.clearObsDataSelections()
            if (window.needsHistoryReload) {
                window.reloadHistory()
                window.needsHistoryReload = false
            }

        }
        else if (pageStack.currentPage == MyScript.listObject)
        {
            window.backFromList()
        }
        pageStack.depth <= 1 ? quit() : pageStack.pop()
    }

    ToolBarLayout {
        id: toolBarLayout
        visible: false
        ToolIcon {
            //flat: true
            iconId: "icon-m-toolbar-back-white" //"toolbar-back"
            onClicked: {
                if (pageStack.currentPage == MyScript.obsObject)
                {
                    unsavedData = MyScript.unSavedDataExists()
                    MyScript.clearObsDataSelections()
                    if (needsHistoryReload) {
                        window.reloadHistory()
                        needsHistoryReload = false
                    }

                }
                else if (pageStack.currentPage == MyScript.listObject)
                {
                    backFromList()
                }
                pageStack.depth <= 1 ? quit() : pageStack.pop()
            }
        }/*
        ToolButton {
            flat: true
            iconSource: "/qml/symbian3_icons/filter.svg"
            visible: pageStack.currentPage == MyScript.obsObject
            onClicked: {
                if (window.currentDetailLevel < 3)
                {
                    window.currentDetailLevel++
                }
                else
                {
                    window.currentDetailLevel = 1
                }
            }
        }
        ToolButton {
            flat: true
            iconSource: "/qml/symbian3_icons/save.svg"
            visible: pageStack.currentPage == MyScript.obsObject && MyScript.obsObject.currentTab == 3
            onClicked: window.save()
        }*/
        ToolIcon {
            //flat: true
            iconId: "icon-m-toolbar-delete-white"
            visible: pageStack.currentPage == MyScript.obsObject //&& MyScript.obsObject.currentTab == 3
            onClicked: window.clearTab()
        }
    }

    Dialog {
        id: errorDialog
        property alias titleText: titleTextField.text
        property alias dialogText: dialogTextField.text

        title: Label {
            id: titleTextField
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: qsTr("Error")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        content:Item {
            height: 150
            width: parent.width
            anchors.margins: 10
            Label {
                id: dialogTextField
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: ""
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 20
            }
        }

        buttons: Item { height: errorDialogButton.height + 2 * 20; width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: errorDialogButton
                anchors.bottom: parent.bottom
                anchors.margins: 5
                anchors.horizontalCenter: parent.horizontalCenter
                width: 200
                text: qsTr("Ok")
                onClicked: {
                    errorDialog.close()
                }
            }
        }
//        onClickedOutside: errorDialog.close()
    }


    Component.onCompleted: {
//        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
        theme.inverted = true
    }
}
