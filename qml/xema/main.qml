import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript
import XemaEnums 1.0

PageStackWindow {
    id: window

    property string listPageType: ""
    property bool useSystematicSort: false
    property int defaultDetailLevel: 1
    property int currentDetailLevel: 1
    property bool unsavedData: false
    property string currentLanguage: ""
    property string appName: "Xema"
    property string versionString: "0.0.0"
    property bool cppProcessing: true
    property bool needsHistoryReload: false

    signal writeNew(string data)
    signal readObs(string id)
    signal deleteObs(string id, string date, string place)
    signal reloadHistory()
    signal reloadAllHistory()
    signal saveSystematicSorting(bool systematic)
    signal saveDetailLevel(int level)
    signal quit()
    signal loadHistoryWithDate(string date)
    signal loadHistoryWithDateAndPlace(string date, string place)
    signal exportData(bool onlyNew)
    signal restoreSpecies()
    signal restoreLocations()
    signal restoreObservers()
    signal saveLocations()
    signal importData()
    signal exportOwnData()
    signal importOwnData()
    signal openUrl(string url)
    function setSystematicSort(use)
    {
        window.useSystematicSort = use
    }

    function setDetailLevel(level)
    {
        window.defaultDetailLevel = level
        window.currentDetailLevel = level
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

    function newObsWithData(date, place, species)
    {

        MyScript.obsObject.setData(date, place, species)
    }
//    initialPage: Qt.resolvedUrl("MainPage.qml")

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
                errorDialog.dialogText += qsTr("Imported locations\n")
            }
            if (errorNo&XemaEnums.IMPORT_PERSON_OK) {
                errorDialog.dialogText += qsTr("Imported persons\n")
            }
            if (errorNo&XemaEnums.IMPORT_BIRD_OK) {
                errorDialog.dialogText += qsTr("Imported birds\n")
            }
            if (errorNo&XemaEnums.IMPORT_STATUS_OK) {
                errorDialog.dialogText += qsTr("Imported status\n")
            }
            if (errorNo&XemaEnums.IMPORT_HISTORY_OK) {
                errorDialog.dialogText += qsTr("Imported history\n")
            }

            if (errorNo&XemaEnums.IMPORT_LOCATIONERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with locations file\n")
            }
            if (errorNo&XemaEnums.IMPORT_PERSONERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with persons file\n")
            }
            if (errorNo&XemaEnums.IMPORT_BIRDERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with birds file\n")
            }
            if (errorNo&XemaEnums.IMPORT_STATUSERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with status file\n")
            }
            if (errorNo&XemaEnums.IMPORT_HISTORYERROR) {
                errorDialog.titleText = qsTr("Import error")
                errorDialog.dialogText += qsTr("Error with history file\n")
            }
        }
        errorDialog.open()
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: cppProcessing
        width: 64
        height: width
        visible: running
    }

    ToolBarLayout {
        id: toolBarLayout
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
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
        }
        ToolButton {
            flat: true
            iconSource: "/qml/s3icons/filter.svg"
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
            iconSource: "/qml/s3icons/save.svg"
            visible: pageStack.currentPage == MyScript.obsObject && MyScript.obsObject.currentTab == 3
            onClicked: {
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
        }
        ToolButton {
            flat: true
            iconSource: "toolbar-delete"
            visible: pageStack.currentPage == MyScript.obsObject //&& MyScript.obsObject.currentTab == 3
            onClicked: {
                MyScript.obsObject.clearTab()
                unsavedData = false
            }
        }
    }

    Dialog {
        id: errorDialog
        property alias titleText: titleTextField.text
        property alias dialogText: dialogTextField.text

        title: Text {
            id: titleTextField
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: qsTr("Error")
            horizontalAlignment: Text.AlignHCenter
        }
        content:Item {
            height: 150
            width: parent.width
            anchors.topMargin: 10
            Text {
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

        buttons: Item { height: errorDialog.height + 2 * 10;
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: errorDialogButton
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                text: qsTr("Ok")
                onClicked: {
                    errorDialog.close()
                }
            }
        }
    }

    Component.onCompleted: {
        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
    }
}
