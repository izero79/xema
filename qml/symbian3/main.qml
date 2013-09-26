import QtQuick 1.1
import com.nokia.symbian 1.1
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
    property string pendingOnlineTask: ""
    property variant pendingOnlineTaskParams: []

    property string defaultAssociation: ""
    property bool onlyDefaultAssiciation: false
    property bool alwaysShowOwn: false

    property bool tiiraLoginOk: false
    property bool useTiira: false
    property string tiiraUsername: ""
    property string tiiraPwdHash: ""
    property bool tiiraAutosave: false
    property int tiiraServer: 1
    property string tiiraServerUsername: ""
    property string tiiraServerPassword: ""

    property string adUrl: ""

    signal writeNew(string data)
    signal readObs(string id)
    signal deleteObs(string id, string date, string place)
    signal reloadHistory()
    signal saveSystematicSorting(bool systematic)
    signal saveWPSpecies(bool wpspecies)
    signal saveDetailLevel(int level)
    signal saveDefaultCountry(string defaultCountry)
    signal saveOnlyDefaultCountry(bool onlyDefault)
    signal saveDefaultAssociation(string defaultAssociation)
    signal saveOnlyDefaultAssociation(bool onlyDefault)
    signal saveAlwaysShowOwn(bool alwaysShowOwn)
    signal saveExportWgs(bool exportWgs)
    signal quit()
    signal loadHistoryWithDate(string date)
    signal loadHistoryWithDateAndPlace(string date, string place)
    signal exportData(bool onlyNew, bool allCountries, string date, string place, string delimiter)
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
    signal exportObsToTiira(string id);
    signal exportToTiira(string date, string place);

    signal saveUseTiira(bool useTiira)
    signal saveTiiraUsername(string username)
    signal saveTiiraPwdHash(string pwdHash)
    signal saveTiiraAutosave(bool autosave)
    signal saveTiiraServer(int server)
    signal saveTiiraServerUsername(string username)
    signal saveTiiraServerPassword(string password)
    signal tiiraLogin()

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

    function setOnlyDefaultAssociation(onlyDefault)
    {
        window.onlyDefaultAssiciation = onlyDefault
    }

    function setDefaultAssociation(defaultAssociation)
    {
        window.defaultAssociation = defaultAssociation
    }

    function setAlwaysShowOwn(showOwn)
    {
        window.alwaysShowOwn = showOwn
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
        MyScript.editPeople(index)
    }

    function editBird(index)
    {
        MyScript.editBird(index)
    }

    function editStatus(index)
    {
        MyScript.editStatus(index)
    }

    function editLocation(index)
    {
        MyScript.editLocation(index)
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

    function locationAccuracyChanged(name)
    {
        MyScript.fillLocationAccuracyBox(name)
    }

    function birdAccuracyChanged(name)
    {
        MyScript.fillBirdAccuracyBox(name)
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
        else if (listPageType == "associations")
        {
            MyScript.fillAssociationBox()
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
        if (hasInternetConnection() == false) {
            pendingOnlineTask = "birdmap"
            pendingOnlineTaskParams = [itemi, place_x, place_y, x, y]
            openInternetConnection()
        } else {
            cppProcessing = true
            MyScript.showMapPage(itemi, "bird", place_x, place_y, x, y)
            cppProcessing = false
        }
    }

    function showPlaceMap(itemi, x, y) {
        if (hasInternetConnection() == false) {
            pendingOnlineTask = "placemap"
            pendingOnlineTaskParams = [itemi, x, y]
            openInternetConnection()
            return
        } else {
            cppProcessing = true
            MyScript.showMapPage(itemi, "place", x, y, null, null)
            cppProcessing = false
        }
    }

    function setBirdCoords(coords) {
        MyScript.fillBirdCoords(coords)
    }

    function setPlaceCoords(coords) {
        MyScript.fillLocationCoords(coords)
    }

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

    function hasInternetConnection() {
        var connected = NetworkController.isConnected()
        console.log('connected: ' + connected)
        return connected
    }

    function openInternetConnection() {
        var ask = true //XemaSettings.askForConnection()
        if (ask) {
            connectionDialog.open()
        } else {
            NetworkController.openAnyConnection()
        }

    }

    function setUseTiira(use) {
        window.useTiira = use
    }

    function setTiiraUsername(value) {
        window.tiiraUsername = value
    }

    function setTiiraPwdHash(value) {
        window.tiiraPwdHash = value
    }

    function setTiiraAutosave(value) {
        window.tiiraAutosave = value
    }

    function setTiiraServer(value) {
        window.tiiraServer = value
    }

    function setTiiraServerUsername(value) {
        window.tiiraServerUsername = value
    }

    function setTiiraServerPassword(value) {
        window.tiiraServerPassword = value
    }

    function loginOk(name) {
        window.tiiraLoginOk = true
        MyScript.settingsObject.loginOk(name)
    }

    function loginFailedCredientals() {
        window.tiiraLoginOk = false
        MyScript.settingsObject.loginFailedCredientals()
    }

    function loginFailedNoRights() {
        window.tiiraLoginOk = false
        MyScript.settingsObject.loginFailedNoRights()
    }

    function serverLoginFailed() {
        window.tiiraLoginOk = false
        MyScript.settingsObject.serverLoginFailed()
    }

    function loginFailedUnknown() {
        window.tiiraLoginOk = false
        MyScript.settingsObject.loginFailedUnknown()
    }

    function tiiraExportDone() {
        console.log("tiira exportDone")
        if (pageStack.currentPage != MyScript.obsObject) {

            errorDialog.dialogText = ""
            errorDialog.titleText = qsTr("Tiira export complete")
            errorDialog.dialogText = qsTr("Export to Tiira is complete.")
            errorDialog.open()
        }
    }

    function setTiiraLoginOk(ok) {
        console.log("setTiiraLoginOk: " + ok)
        window.tiiraLoginOk = ok
    }

    function tiiraExport_obs(id) {
        if (hasInternetConnection() == false) {
            pendingOnlineTask = "obsexport"
            pendingOnlineTaskParams = id
            openInternetConnection()
        } else {
            exportObsToTiira(id)
        }
    }

    function tiiraExport_many(date, place) {
        if (hasInternetConnection() == false) {
            pendingOnlineTask = "obsexportmany"
            pendingOnlineTaskParams = [date,place]
            openInternetConnection()
        } else {
            exportToTiira(date, place)
        }
    }

    function tiira_login() {
        if (hasInternetConnection() == false) {
            pendingOnlineTask = "login"
            openInternetConnection()
        } else {
            tiiraLogin();
        }
    }

    function adReady(iconUrl, url) {
        console.log('adReady: ' + iconUrl)
        adUrl = url;
        MyScript.historyObject.showAd(iconUrl)
        window.tiira_login()
    }

    function tiiraClosed()
    {
        errorDialog.dialogText = ""
        console.log("Tiira closed")
        errorDialog.titleText = qsTr("Tiira not available")
        errorDialog.dialogText = qsTr("Tiira seems to be unavailable at the moment. Tiira functions are now disabled.")
        tiiraLoginOk = false;
        errorDialog.open()
    }


    Connections {
        target: NetworkController
        onConnectionReady: {
            console.log('connection ready!!!!!!')
            if (pendingOnlineTask == "login") {
                tiiraLogin();
            }
            else if (pendingOnlineTask == "obsexport") {
                exportObsToTiira(pendingOnlineTaskParams);
            }
            else if (pendingOnlineTask == "obsexportmany") {
                exportToTiira(pendingOnlineTaskParams[0],pendingOnlineTaskParams[1])
            }
            else if (pendingOnlineTask == "birdmap") {
                showBirdMap(pendingOnlineTaskParams[0],pendingOnlineTaskParams[1],pendingOnlineTaskParams[2],pendingOnlineTaskParams[3],pendingOnlineTaskParams[4])
            }
            else if (pendingOnlineTask == "placemap") {
                showPlaceMap(pendingOnlineTaskParams[0],pendingOnlineTaskParams[1],pendingOnlineTaskParams[2])
            }

            pendingOnlineTask = ""
            pendingOnlineTaskParams = ""
        }
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
            width: 64
            height: width
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
    }

    function save() {
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
        }
        ToolButton {
            flat: true
            iconSource: "toolbar-delete"
            visible: pageStack.currentPage == MyScript.obsObject //&& MyScript.obsObject.currentTab == 3
            onClicked: window.clearTab()
        }
    }

    Dialog {
        id: connectionDialog

        title: Label {
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: qsTr("Connection needed")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        content:Item {
            height: 100
            width: parent.width
            anchors.margins: 10
            Label {
                id: connectionDialogText
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: qsTr("Internet connection is needed, connect now?")
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 20
            }
        }

        buttons: Item { height: connectionDialogNoButton.height + connectionDialogDoNotAsk.height + 40; width: parent.width - 20
            CheckBox {
                id: connectionDialogDoNotAsk
                anchors.bottom: connectionDialogNoButton.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                text: qsTr("Connect automatically when needed")
                checked: false

            }

            Button {
                id: connectionDialogNoButton
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 5
                width: parent.width / 2
                text: qsTr("No")
                onClicked: {
                    pendingOnlineTask = ""
                    pendingOnlineTaskParams = ""
                    connectionDialog.close()
                }
            }
            Button {
                id: connectionDialogYesButton
                anchors.bottom: parent.bottom
                anchors.left: connectionDialogNoButton.right
                anchors.margins: 5
                width: parent.width / 2
                text: qsTr("Yes")
                onClicked: {
                    if (connectionDialogDoNotAsk.checked) {
                        XemaSettings.askForConnection(false);
                    }

                    NetworkController.openAnyConnection()
                    connectionDialog.close()
                }
            }
        }
        onClickedOutside: {
            pendingOnlineTask = ""
            pendingOnlineTaskParams = ""
            connectionDialogDoNotAsk = false
            connectionDialog.close()
        }
        onRejected: {
            pendingOnlineTask = ""
            pendingOnlineTaskParams = ""
            connectionDialogDoNotAsk = false
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
        onClickedOutside: errorDialog.close()
    }

    Component.onCompleted: {
        MyScript.historyPageComponent = Qt.createComponent(Qt.resolvedUrl("HistoryPage.qml"))
        MyScript.historyObject = MyScript.historyPageComponent.createObject(window)
        pageStack.push(MyScript.historyObject)
    }
}
