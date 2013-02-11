import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.location 1.1
import QtMobility.sensors 1.2

Page {
    id: listPage
    tools: listToolBarLayout
    property bool useGPS: listView.model == locationModel
    property bool useCompass: listView.model == directionModel && window.compassSupported
    property bool activeGPS: positionSource.active
    property bool activeCompass: compass.active

    ToolBarLayout {
        id: listToolBarLayout
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
                window.backFromList()
                positionSource.stop()
                compass.stop()
                pageStack.pop()
            }
        }
        ToolButton {
            flat: false
            text: useGPS ? (activeGPS ? qsTr("Stop GPS") : qsTr("Start GPS")) : (useCompass ? (activeCompass ? qsTr("Stop Compass") : qsTr("Start Compass")) : "")
            visible: useGPS || useCompass
            onClicked: {
                if (useGPS) {
                    console.log("start gps")
                    if(activeGPS)
                    {
                        positionSource.stop()
                    }
                    else
                    {
                        positionSource.start()
                    }
                } else if (useCompass) {
                    console.log("start compass")
                    if(activeCompass)
                    {
                        compass.stop()
                    }
                    else
                    {
                        compass.start()
                    }
                }

            }
        }
        ToolButton {
            flat: true
            iconSource: "toolbar-add"
            visible: listView.model == personModel || listView.model == locationModel || listView.model == birdModel || listView.model == statusModel
            onClicked: {
                console.log("Add new")
                addNewClicked()

            }
        }
    }

    function init()
    {
        console.log("listpage init")
        selectModel()
        selectDelegate()
        listView.model.filter("")
        filterTf.text = ""
        console.log("listpage init - done")
    }

    function addNewClicked()
    {
        if(positionSource.active)
        {
            positionSource.stop()
        }
        if (listPageType == "status" || listPageType == "editstatuses")
        {
            window.addStatus()
            return
        }
        if (listPageType == "birds" || listPageType == "editbirds")
        {
            window.addBird()
            return
        }
        else if (listPageType == "places" ||  listPageType == "editplaces")
        {
            window.addLocation()
            return
        }
        else if (listPageType == "editallpeople" || listPageType == "regpeople" || listPageType == "people")
        {
            window.addPeople()
            return
        }
        else if (listPageType == "atlas")
        {
            return
        }
    }

    ContextMenu {
        id: contextMenu
        property int selectedIndex: -1
        MenuLayout {
            MenuItem {
                text: qsTr("Remove")
                onClicked: {
                    console.log("REMOVE " + contextMenu.selectedIndex)
                    if (listPageType == "editbirds")
                    {
                        birdModel.removeRow(contextMenu.selectedIndex)
                    }

                    if (listPageType == "editplaces")
                    {
                        locationModel.removeRow(contextMenu.selectedIndex)
                    }

                    if (listPageType == "editallpeople")
                    {
                        personModel.removeRow(contextMenu.selectedIndex)
                    }

                    if (listPageType == "editstatuses")
                    {
                        statusModel.removeRow(contextMenu.selectedIndex)
                    }
                }
            }
        }
    }

    function showContextMenu(index)
    {
        if (listPageType == "editbirds" || listPageType == "editplaces" || listPageType == "editallpeople"
                || listPageType == "editstatuses" )
        {
            contextMenu.selectedIndex = index
            contextMenu.open()
        }
    }

    function clicked(name)
    {
        if(positionSource.active)
        {
            positionSource.stop()
        }
        if(compass.active)
        {
            compass.stop()
        }
        if (listPageType == "atlas")
        {
            window.atlasChanged(name)
        }
        else if (listPageType == "birds")
        {
            window.birdChanged(name)
        }
        else if (listPageType == "editbirds")
        {
            window.editBird(name)
            return
        }
        else if (listPageType == "editstatuses")
        {
            window.editStatus(name)
            return
        }
        else if (listPageType == "places")
        {
            console.log("hoo")
            window.placeChanged(name)
        }
        else if (listPageType == "editplaces")
        {
            window.editLocation(name)
            return
        }
        else if (listPageType == "editallpeople")
        {
            window.editPeople(name)
            return
        }
        else if (listPageType == "regpeople")
        {
            window.regpeopleChanged(name)
        }
        else if (listPageType == "people")
        {
            window.peopleChanged(name)
        }
        else if (listPageType == "sex")
        {
            window.sexChanged(name)
        }
        else if (listPageType == "age")
        {
            window.ageChanged(name)
        }
        else if (listPageType == "dress")
        {
            window.dressChanged(name)
        }
        else if (listPageType == "direction")
        {
            window.directionChanged(name)
        }
        pageStack.pop()
    }

    function selectNames(names)
    {
        var selNames = new Array();
        console.log("names: " + names)

        selNames = names.split(",")
        console.log("name[0]: " + selNames[0])

        for(var i = 0; i < selNames.length; i++)
        {
            for(var j = 0; j < personModel.rowCount(); j++)
            {
                if (personModel.data(j, 35) == selNames[i])
                {
                    personModel.setData(j, true, 2)
                }
            }
        }
    }

    function selectStatus(names)
    {
        var selNames = new Array();
        console.log("select status names: " + names)

        selNames = names.split(",")
        console.log("names length: " + selNames.length)

        if (selNames.length == 1 && selNames[0] == "")
        {
            for(var j = 0; j < statusModel.rowCount(); j++)
            {
                statusModel.setData(j, false, 2)
            }
            return
        }

        for(var k = 0; k < statusModel.rowCount(); k++)
        {
            for(var i = 0; i < selNames.length; i++)
            {
                console.log("tarkistetaan: " + selNames[i] )
                console.log("mallissa: " + statusModel.data(k, 35) )
                if (statusModel.data(k, 35) == selNames[i])
                {
                    statusModel.setData(k, true, 2)
                    break
                }
                else
                {
                    statusModel.setData(k, false, 2)
                }
            }
        }
    }

    function selectDelegate() {
        console.log("selecting delegate")
        if (listPageType == "birds")
        {
            console.log("selecting delegate - setting sort")
            if (window.useSystematicSort == true)
            {
                listView.model.setSorting(0, true)
            }
            else
            {
                listView.model.setSorting(1, true)
            }
            listView.editMode = false
            console.log("selecting delegate - setting sort done")
            listView.delegate = birdDelegate
        }
        else if (listPageType == "editbirds")
        {
            if (window.useSystematicSort == true)
            {
                listView.model.setSorting(0, true)
            }
            else
            {
                listView.model.setSorting(1, true)
            }
            listView.editMode = true
            listView.delegate = birdDelegate
        }
        else if (listPageType == "places")
        {
            listView.model.setSorting(1, true)
            listView.editMode = false
            listView.delegate = locationDelegate
        }
        else if (listPageType == "editplaces")
        {
            listView.model.setSorting(1, true)
            listView.editMode = true
            listView.delegate = locationDelegate
        }
        else if (listPageType == "editstatuses")
        {
            listView.editMode = true
            listView.delegate = statusDelegate
        }
        else if (listPageType == "status")
        {
            listView.editMode = false
            listView.delegate = statusDelegate
        }
        else if (listPageType == "dress" || listPageType == "age" || listPageType == "sex" || listPageType == "atlas")
        {
            listView.editMode = false
            listView.delegate = simpleDelegate
        }
        else if (listPageType == "editallpeople")
        {
            listView.model.setSorting(1, true)
            listView.editMode = true
            listView.delegate = myTestDelegate
        }
        else if (listPageType == "regpeople")
        {
            listView.model.setSorting(1, true)
            listView.editMode = false
            listView.showRegistered = true
            listView.delegate = myTestDelegate
        }
        else if (listPageType == "direction")
        {
            listView.editMode = false
            listView.delegate = simpleDelegate
        }
        else
        {
            listView.model.setSorting(1, true)
            listView.editMode = false
            listView.showRegistered = false
            listView.delegate = emptyDelegate
        }
    }

    function selectModel() {
        console.log("selecting model")
        if (listPageType == "atlas")
        {
            listView.model = atlasModel
        }
        else if (listPageType == "birds")
        {
//                listView.section.property = "section"
//                listView.section.delegate = sectionDelegate
            listView.model = birdModel
        }
        else if (listPageType == "editbirds")
        {
            listView.model = birdModel
        }
        else if (listPageType == "places")
        {
            listView.model = locationModel
        }
        else if (listPageType == "editplaces")
        {
            listView.model = locationModel
        }
        else if (listPageType == "status")
        {
            listView.model = statusModel
        }
        else if (listPageType == "editstatuses")
        {
            listView.model = statusModel
        }
        else if (listPageType == "sex")
        {
            listView.model = sexModel
        }
        else if (listPageType == "dress")
        {
            listView.model = dressModel
        }
        else if (listPageType == "age")
        {
            listView.model = ageModel
        }
        else if (listPageType == "regpeople")
        {
            listView.model = personModel
        }
        else if (listPageType == "editallpeople")
        {
            listView.model = personModel
        }
        else if (listPageType == "people")
        {
            listView.model = personModel
        }
        else if (listPageType == "direction")
        {
            listView.model = directionModel
        }
        else
        {
            listView.model = emptyModel
        }
    }

    function setSelected( string )
    {
        filterTf.text = string
/*
        for(var i = 0; i < listView.model.rowCount(); i++)
        {
            if( string == listView.model.data(i, 49 )) {
                console.log("loyty")
                listView.positionViewAtIndex( i, ListView.Center)
            }
        }
*/
    }

    function showCalibrateDialog()
    {
        calibrateDialog.dialogText = qsTr("Calibrate compass by moving the device in a figure eight pattern.")
        calibrateDialog.titleText = qsTr("Calibration")
        calibrateDialog.open()
    }

    function hideCalibrateDialog()
    {
        calibrateDialog.close()
    }


    Component.onCompleted: {
        console.log("ListPage loaded")
        listView.model.filter("")
        for(var i = 0; i < listView.model.rowCount(); i++)
        {
            listView.model.setData(i, false, 2)
        }
    }

    Dialog {
        id: calibrateDialog
        property alias titleText: titleTextField.text
        property alias dialogText: dialogTextField.text

        title: Label {
            id: titleTextField
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: qsTr("")
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

        buttons: Item { height: calibrateDialog.height + 2 * 20; width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: calibrateDialogButton
                anchors.bottom: parent.bottom
                anchors.margins: 5
                anchors.horizontalCenter: parent.horizontalCenter
                width: 200
                text: qsTr("Cancel")
                onClicked: {
                    compass.stop()
                    calibrateDialog.close()
                }
            }
        }
        onClickedOutside: {
            compass.stop()
            calibrateDialog.close()
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
            text: "" //listPageType
            placeholderText: qsTr("Search")
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            inputMethodHints: Qt.ImhNoPredictiveText
            onTextChanged: {
                //console.log("teksti muuttuu: " + text)
                listView.model.filter(text)
            }
            Image {
                anchors { top: parent.top; right: parent.right; margins: 1 }
                id: clearText
                fillMode: Image.PreserveAspectFit
                smooth: true;
                source: ":../symbian3_icons/icon-m-input-clear.png"
                height: 48 //parent.height - 8 * 2
                width: 48 //parent.height - 8 * 2
                visible: !busyIndicator.visible && filterTf.text

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
            BusyIndicator {
                id: busyIndicator
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                width: height
                running: positionSource.active || compass.active
                visible: running
            }

        }
    }

    ScrollDecorator {
        id: scrolldecorator
        flickableItem: listView
    }

    ListView {
        id: listView
        property bool showRegistered: false
        property bool editMode: false

        anchors.top: item1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        delegate: emptyDelegate/* {
            console.log("selecting delegate")
            if (listPageType == "birds")
            {
                console.log("selecting delegate - setting sort")
                if (window.useSystematicSort == true)
                {
                    model.setSorting(0, true)
                }
                else
                {
                    model.setSorting(1, true)
                }
                editMode = false
                console.log("selecting delegate - setting sort done")
                return birdDelegate
            }
            else if (listPageType == "editbirds")
            {
                if (window.useSystematicSort == true)
                {
                    model.setSorting(0, true)
                }
                else
                {
                    model.setSorting(1, true)
                }
                editMode = true
                return birdDelegate
            }
            else if (listPageType == "places")
            {
                editMode = false
                return locationDelegate
            }
            else if (listPageType == "editplaces")
            {
                editMode = true
                return locationDelegate
            }
            else if (listPageType == "editstatuses")
            {
                editMode = true
                return statusDelegate
            }
            else if (listPageType == "status")
            {
                editMode = false
                return statusDelegate
            }
            else if (listPageType == "dress" || listPageType == "age" || listPageType == "sex" || listPageType == "atlas")
            {
                editMode = false
                return simpleDelegate
            }
            else if (listPageType == "editallpeople")
            {
                editMode = true
                return myTestDelegate
            }
            else if (listPageType == "regpeople")
            {
                editMode = false
                showRegistered = true
                return myTestDelegate
            }
            else
            {
                editMode = false
                showRegistered = false
                return myTestDelegate
            }
        }*/
        clip: true
        model: emptyModel/*{
            console.log("selecting model")
            if (listPageType == "atlas")
            {
                return atlasModel
            }
            else if (listPageType == "birds")
            {
//                listView.section.property = "section"
//                listView.section.delegate = sectionDelegate
                return birdModel
            }
            else if (listPageType == "editbirds")
            {
                return birdModel
            }
            else if (listPageType == "places")
            {
                return locationModel
            }
            else if (listPageType == "editplaces")
            {
                return locationModel
            }
            else if (listPageType == "status")
            {
                return statusModel
            }
            else if (listPageType == "editstatuses")
            {
                return statusModel
            }
            else if (listPageType == "sex")
            {
                return sexModel
            }
            else if (listPageType == "dress")
            {
                return dressModel
            }
            else if (listPageType == "age")
            {
                return ageModel
            }
            else if (listPageType == "regpeople")
            {
                return personModel
            }
            else
            {
                return personModel
            }
        }*/
    }
    ListDelegate {
        id: myTestDelegate

    }
    BirdListDelegate {
        id: birdDelegate

    }
    LocationListDelegate {
        id: locationDelegate

    }
    StatusListDelegate {
        id: statusDelegate

    }
    SimpleDelegate {
        id: simpleDelegate

    }

    Component {
        id: emptyDelegate
        Item {}
    }

    ListModel {
        id: emptyModel
    }

/*
    Component {
        id: sectionDelegate
        Item {
            width: listView.width
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
*/
    PositionSource {
        id: positionSource
        property string accuracy: maxVerticalAccurary <= maxHorizontalAccurary ? parseInt(maxHorizontalAccurary) : parseInt(maxVerticalAccurary)
        property real maxHorizontalAccurary: 250
        property real maxVerticalAccurary: 250
        property bool coordinatesValid: (positionSource.position.latitudeValid && positionSource.position.longitudeValid &&
                                        positionSource.position.coordinate.latitude != "NaN" && positionSource.position.coordinate.longitude != "NaN")
        updateInterval: 2500

        onActiveChanged: {
            console.log("Active Changed called")
            if(active==false)
            {
                maxHorizontalAccurary = 250
                maxVerticalAccurary = 250
            }
        }

        onPositionChanged: {
            console.log(positionSource.position.coordinate.latitude + ", " + positionSource.position.coordinate.longitude)
            if (coordinatesValid) {
                if (positionSource.position.horizontalAccuracy < maxHorizontalAccurary ||
                    positionSource.position.verticalAccuracy < maxVerticalAccurary) {
                    console.log("Position changed and better accuracy")
                    if (positionSource.position.horizontalAccuracy < maxHorizontalAccurary)
                    {
                        maxHorizontalAccurary = positionSource.position.horizontalAccuracy
                    }
                    if (positionSource.position.verticalAccuracy < maxVerticalAccurary)
                    {
                        maxVerticalAccurary = positionSource.position.verticalAccuracy
                    }


                    maxVerticalAccurary = positionSource.position.verticalAccuracy
                    var latitude = positionSource.position.coordinate.latitude.toFixed(4)
                    var longitude = positionSource.position.coordinate.longitude.toFixed(4)
                    var indexOfDot = latitude.indexOf(".");
                    latitude = latitude.slice(0,indexOfDot+2);
                    indexOfDot = longitude.indexOf(".");
                    longitude = longitude.slice(0,indexOfDot+2);
                    filterTf.text = latitude + ":" + longitude
                    console.log("accuracy now: " + positionSource.accuracy)
                }
            }
        }
    }
    Compass {
        id: compass
        active: false

        onReadingChanged: {
            console.log("kalibrointi: " + reading.calibrationLevel);
            if (reading.calibrationLevel < 1)
            {
                if (calibrateDialog.status != DialogStatus.Open) {
                    showCalibrateDialog();
                }
                return;
            }
            if (reading.calibrationLevel >= 1 && calibrateDialog.status == DialogStatus.Open)
            {
                hideCalibrateDialog();
            }

            console.log(reading.azimuth);
            var value = reading.azimuth;
            var direction = "";
            if (value > 349 || value <= 11) {
                direction = "N";
            }
            if (value > 11 && value <= 34) {
                direction = "NNE";
            }
            if (value > 34 && value <= 56) {
                direction = "NE";
            }
            if (value > 56 && value <= 79) {
                direction = "ENE";
            }
            if (value > 79 && value <= 101) {
                direction = "E";
            }
            if (value > 101 && value <= 124) {
                direction = "ESE";
            }
            if (value > 124 && value <= 146) {
                direction = "SE";
            }
            if (value > 146 && value <= 169) {
                direction = "SSE";
            }
            if (value > 169 && value <= 191) {
                direction = "S";
            }
            if (value > 191 && value <= 214) {
                direction = "SSW";
            }
            if (value > 214 && value <= 236) {
                direction = "SW";
            }
            if (value > 236 && value <= 259) {
                direction = "WSW";
            }
            if (value > 259 && value <= 281) {
                direction = "W";
            }
            if (value > 281 && value <= 304) {
                direction = "WNW";
            }
            if (value > 304 && value <= 326) {
                direction = "NW";
            }
            if (value > 326 && value <= 349) {
                direction = "NNW";
            }
            console.log("suunta:" + direction);
            filterTf.text = "^" + direction  + ",";
        }
    }
}
