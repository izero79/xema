import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1
import "myjs.js" as MyScript
import XemaEnums 1.0

Page {

    id: obsPage
    tools: toolBarLayout
    property int detailLevel: window.currentDetailLevel
    property int currentId: 0
    property int currentTab: 1
    property int delegateHeight: detailLevel == 3 ? 330 : detailLevel == 2 ? 180 : 145
    property variant missingInfo: null
    property bool edited: false
    property string saver: ""

    height: parent.height
    width: parent.width

    onDelegateHeightChanged: {
        MyScript.changeDelegateHeight()
    }

    Component.onCompleted: {
        window.currentDetailLevel = window.defaultDetailLevel
        MyScript.createObjects();
    }

    Component.onDestruction: MyScript.removeObjects();

    property int obsCount: 0

    function init()
    {
        console.log("obs init")
        currentId = 0
        currentTab = 1
        tabGroup.currentTab = tab1content

        startDateTf.text = ""
        stopDateTf.text = ""
        regPeopleTa.text = ""
//        otherPeopleTa.text = ""
        weatherTa.text = ""

        locationTf.text = ""
        startTimeTf.text = ""
        endTimeTf.text = ""

        birdNameTf.text = ""
        moreInfoTa.text = ""
        atlasTf.text = ""
        hideChkBox.checked = false
        birdCoordinatesTf.text = ""
        directionTf.text = ""
        distanceTf.text = ""

        MyScript.removeObjects();
        MyScript.createObjects();

        window.currentDetailLevel = window.defaultDetailLevel

        MyScript.changeDelegateHeight()
        for(var i=0;i<personModel.rowCount();i++)
        {
            if (personModel.data(i, 37) == true)
            {
                if (regPeopleTa.text == "") {
                    regPeopleTa.text = personModel.data(i, 35)
                } else {
                    regPeopleTa.text += ", " + personModel.data(i, 35)
                }
            }
            if (personModel.data(i,40) == true)
            {
                saver = personModel.data(i, 40)
            }
        }

    }

    function birdChanged(i)
    {
        if (currentLanguage == "en") {
            birdNameTf.text = birdModel.data(i, 44 )
        }
        else if (currentLanguage == "sv") {
            birdNameTf.text = birdModel.data(i, 37 )
        }
        else {
            birdNameTf.text = birdModel.data(i, 36 )
        }
    }

    function placeChanged(i)
    {
        if (currentLanguage == "en") {
            locationTf.text = locationModel.data(i, 42 ) + ", " + locationModel.data(i, 43 )
        }
        else if (currentLanguage == "sv") {
            locationTf.text = locationModel.data(i, 40 ) + ", " + locationModel.data(i, 41 )
        }
        else {
            locationTf.text = locationModel.data(i, 35 ) + ", " + locationModel.data(i, 36 )
        }

    }

    function regpeopleChanged(name)
    {
        regPeopleTa.text = name
    }

    function peopleChanged(name)
    {
//        otherPeopleTa.text = name
    }

    function openMap() {
        var x = null;
        var y = null;
        var place_x = null;
        var place_y = null;
        var locationCoords = getLocationCoords()
        if (locationCoords != "") {
            var place_coords = locationCoords.split(":")
            place_x = place_coords[0]
            place_y = place_coords[1]

        }

        console.log('loc coords: '+ locationCoords)

        if (birdCoordinatesTf.text != "") {
            var coords = birdCoordinatesTf.text.split(":")
            x = coords[0]
            y = coords[1]
        }

        window.showBirdMap(birdCoordinatesTf, place_x, place_y, x, y)
    }

    function readAllData()
    {
        var missingData = new Array();
        if (startDateTf.text == "")
        {
            missingData[missingData.length] = qsTr("Start date")
        }
        if (regPeopleTa.text == "")
        {
            missingData[missingData.length] = qsTr("Observer")
        }
        if (locationTf.text == "")
        {
            missingData[missingData.length] = qsTr("Location")
        }
        if (birdNameTf.text == "")
        {
            missingData[missingData.length] = qsTr("Species")
        }

        var allData = "";
        var delimiter = "#";

        allData += currentId + delimiter
        var bird_abbrev = findBirdAbbrev(birdNameTf.text)
        allData += bird_abbrev + delimiter
        allData += startDateTf.text + delimiter
        if (stopDateTf.text != startDateTf.text) {
            allData += stopDateTf.text + delimiter
        } else {
            allData += delimiter
        }

        allData += startTimeTf.text + delimiter
        if (startTimeTf.text == endTimeTf.text && (stopDateTf.text == startDateTf.text || stopDateTf.text == "")) {
            allData += delimiter
        } else {
            allData += endTimeTf.text + delimiter
        }

        var locationIndex = findLocationIndex(locationTf.text)
        if (locationIndex < 0) {
            console.log("location index < 0")
            var town = locationTf.text.substring(0, locationTf.text.indexOf(", ") )
            var place = locationTf.text.substring(locationTf.text.indexOf(", ")+2 )
            allData += town + delimiter
            allData += place + delimiter
        }
        else {
            var name = locationNameByIndex(locationIndex)
            if( name == -1 )
            {
                console.log("joo, name: " +name)
                var currentLocation = locationTf.text
                var ctown = currentLocation.substring(0, currentLocation.indexOf(", ") )
                var cplace = currentLocation.substring(currentLocation.indexOf(", ")+2 )
                console.log("testi: " + ctown + "#" + cplace)
                allData += ctown
                allData += delimiter
                allData += cplace
                allData += delimiter
            }
            else {
                allData += locationNameByIndex(locationIndex)
                allData += delimiter

            }
        }

        allData += locationCoordinantesByIndex(locationIndex)
        allData += delimiter
        // accuracy
        var locationAccuracy = findLocationAccuracyValue(locationAccuracyTf.text)

        if (locationAccuracy != -1) {
            allData += locationAccuracy
        }

        allData += delimiter

        if (birdCoordinatesTf.text != "") {
            var birdWGS = birdCoordinatesTf.text
            var birdWGSArray = birdWGS.split(":",2);

            // uusi, x-coord bird
            allData += birdWGSArray[1]
            allData += delimiter
            // uusi, y-coord bird
            allData += birdWGSArray[0]
            allData += delimiter
        } else {
            // uusi, x-coord bird
            allData += delimiter
            // uusi, y-coord bird
            allData += delimiter

        }

        // accuracy
        var birdAccuracy = findBirdAccuracyValue(birdAccuracyTf.text)

        if (birdAccuracy != -1) {
            allData += birdAccuracy
        }

        allData += delimiter

        // uusi, paikannettu
        allData += delimiter
        allData += moreInfoTa.text + delimiter
        var atlas_abbrev = findAtlas(atlasTf.text)
        allData += atlas_abbrev + delimiter
        // uusi, saver
        allData += saver + delimiter

        // uusi, save time
        allData += Qt.formatDateTime(new Date(), "dd.MM.yyyy hh:mm")
        allData += delimiter
        allData += regPeopleTa.text + delimiter
        allData += delimiter // paikka other peoplelle, ei kaytossa nyt
//        allData += otherPeopleTa.text + delimiter // tama pois
        allData += hideChkBox.checked + delimiter
        // uusi, koontihavainto
        allData += delimiter
        // uusi, kuuluu havaintoon
        allData += delimiter

        var delegateData = MyScript.readDelegateDatas()
        if (delegateData == "-1")
        {
            missingData[missingData.length] = qsTr("Count")
        }
        else
        {
            allData += delegateData
        }

        if (missingData.length > 0)
        {
            console.log("missing data: " + missingData)
            missingInfo = missingData
            return "-1"
        }

        allData += weatherTa.text
        allData += delimiter
        allData += exportedChkBox.checked
        allData += delimiter

        console.log("allData: " + allData)

        return allData;
    }

    function findBirdAbbrev(name)
    {
        console.log("findBirdAbbrev(name)")
        for(var i=0;i<birdModel.rowCount();i++) {
            // TODO localized names
            if (currentLanguage == "en") {
                if(name === birdModel.data(i, 44)) {
                    return birdModel.data(i, 38)
                }
            }
            else if (currentLanguage == "sv") {
                if(name === birdModel.data(i, 37)) {
                    return birdModel.data(i, 38)
                }
            }
            else {
                if(name === birdModel.data(i, 36)) {
                    return birdModel.data(i, 38)
                }
            }
        }

        return name
    }

    function findAtlas(name)
    {
        console.log("findAtlas(name)")
        for(var i=0;i<atlasModel.rowCount();i++) {
            if (currentLanguage == "en") {
                if(name === atlasModel.data(i, 38)) {
                    return atlasModel.data(i, 36)
                }
            } else if (currentLanguage == "sv") {
                if(name === atlasModel.data(i, 37)) {
                    return atlasModel.data(i, 36)
                }
            }
            else {
                return name
            }
        }
        return name
    }

    function findDress(name)
    {
        console.log("findDress(name)")
        for(var i=0;i<dressModel.rowCount();i++) {
            if (currentLanguage == "en") {
                if(name === dressModel.data(i, 38)) {
                    return dressModel.data(i, 36)
                }
            } else if (currentLanguage == "sv") {
                if(name === dressModel.data(i, 37)) {
                    return dressModel.data(i, 36)
                }
            }
            else {
                return name
            }
        }
        return name
    }

    function findAge(name)
    {
        console.log("findAge(name)")
        for(var i=0;i<ageModel.rowCount();i++) {
            if (currentLanguage == "en") {
                if(name === ageModel.data(i, 38)) {
                    return ageModel.data(i, 36)
                }
            } else if (currentLanguage == "sv") {
                if(name === ageModel.data(i, 37)) {
                    return ageModel.data(i, 36)
                }
            }
            else {
                return name
            }
        }
        return name
    }

    function findSex(name)
    {
        console.log("findSex(name)")
        for(var i=0;i<sexModel.rowCount();i++) {
            if (currentLanguage == "en") {
                if(name === sexModel.data(i, 38)) {
                    return sexModel.data(i, 36)
                }
            } else if (currentLanguage == "sv") {
                if(name === sexModel.data(i, 37)) {
                    return sexModel.data(i, 36)
                }
            }
            else {
                return name
            }
        }
        return name
    }

    function findLocationIndex(name)
    {
        var j = -1
        console.log("findLocationIndex(name)" + name)
        var town = name.split(", ",1);
        var place = name.slice(town[0].length + 2); // comma and space
        console.log("town[0] " +town[0])
        console.log("place " +place)
        for(var i=0;i<locationModel.rowCount();i++) {
            // TODO localized names
            if (currentLanguage == "en") {
                if(town[0] === locationModel.data(i, 42)) {
                    if(place === locationModel.data(i, 43)) {
                        j = i
                        break;
                    }
                }
            }
            else if (currentLanguage == "sv") {
                if(town[0] === locationModel.data(i, 40)) {
                    if(place === locationModel.data(i, 41)) {
                        j = i
                        break;
                    }
                }
            }
            else {
                if(town[0] === locationModel.data(i, 35)) {
                    if(place === locationModel.data(i, 36)) {
                        j = i
                        break;
                    }
                }
            }

        }
        console.log("palautetaan: " + j)
        return j
    }

    function findLocationAccuracyValue(name)
    {
        var j = -1
        console.log("findLocationAccuracyValue(name)" + name)
        for(var i=0;i<locationAccuracyModel.rowCount();i++) {
            if(name === locationAccuracyModel.data(i, 36)) {
                j = locationAccuracyModel.data(i, 35);
                break;
            }
        }
        console.log("palautetaan: " + j)
        return j
    }

    function findBirdAccuracyValue(name)
    {
        var j = -1
        console.log("findBirdAccuracyValue(name)" + name)
        for(var i=0;i<birdAccuracyModel.rowCount();i++) {
            if(name === birdAccuracyModel.data(i, 36)) {
                j = birdAccuracyModel.data(i, 35);
                break;
            }
        }
        console.log("palautetaan: " + j)
        return j
    }

    function findLocationAccuracy(value) {
        var j = ""
        console.log("findLocationAccuracy(value)" + value)
        for(var i=0;i<locationAccuracyModel.rowCount();i++) {
            if(value === locationAccuracyModel.data(i, 35)) {
                j = locationAccuracyModel.data(i, 36);
                break;
            }
        }
        console.log("palautetaan: " + j)
        return j
    }

    function findBirdAccuracy(value) {
        var j = ""
        console.log("findBirdAccuracy(value)" + value)
        for(var i=0;i<birdAccuracyModel.rowCount();i++) {
            if(value === birdAccuracyModel.data(i, 35)) {
                j = birdAccuracyModel.data(i, 36);
                break;
            }
        }
        console.log("palautetaan: " + j)
        return j
    }

    function locationNameByIndex(index)
    {
        if( !locationModel.data(index, 35) || !locationModel.data(index, 36) )
        {
            return -1
        }

        return locationModel.data(index, 35) + "#" + locationModel.data(index, 36)
    }

    function locationCoordinantesByIndex(index)
    {
        if(!locationModel.data(index, 38)) {
            return "#"
        }

        var ykjs = locationModel.data(index, 38)
        var ykj = ykjs.split(":",2);
        return ykj[1] + "#" + ykj[0]
    }

    function findLocation(name)
    {
        console.log("findLocation(name)" + name)
        var town = name.split(", ",2);
        console.log("town[0] " +town[0])
        console.log("town[1] " +town[1])
        for(var i=0;i<locationModel.rowCount();i++) {
            // TODO localized names
            if (currentLanguage == "en") {
                if(town[0] === locationModel.data(i, 42)) {
                    if(town[1] === locationModel.data(i, 43)) {
                        return locationModel.data(i, 35) + ", " + locationModel.data(i, 36)
                    }
                }
            }
            else if (currentLanguage == "sv") {
                if(town[0] === locationModel.data(i, 40)) {
                    if(town[1] === locationModel.data(i, 41)) {
                        return locationModel.data(i, 35) + ", " + locationModel.data(i, 36)
                    }
                }
            }
            else {
                return name
            }

        }

        return name
    }

    function getLocationCoords() {
        var locationIndex = findLocationIndex(locationTf.text)
        if (locationIndex < 0) {
            return "";
        }
        if(!locationModel.data(locationIndex, 39)) {
            return ""
        } else {
            console.log("location coords: " +locationModel.data(locationIndex, 39))
            return locationModel.data(locationIndex, 39)
        }
    }

    function dataLoaded(data)
    {
        console.log("data loaded now: " + data)
        var fields;
        fields = data.split("#");
        console.log("id: " +fields[XemaEnums.OBS_ID])
        currentId = fields[XemaEnums.OBS_ID]
        birdNameTf.text = fields[XemaEnums.OBS_SPECIES]
        startDateTf.text = fields[XemaEnums.OBS_DATE1]
        stopDateTf.text = fields[XemaEnums.OBS_DATE2]
        startTimeTf.text = fields[XemaEnums.OBS_TIME1]
        endTimeTf.text = fields[XemaEnums.OBS_TIME2]
        locationTf.text = fields[XemaEnums.OBS_TOWN] + ", " + fields[XemaEnums.OBS_LOCATION]
        moreInfoTa.text = fields[XemaEnums.OBS_INFO]
        atlasTf.text = fields[XemaEnums.OBS_ATLAS]
        regPeopleTa.text = fields[XemaEnums.OBS_REGPERSON]
//        otherPeopleTa.text = fields[XemaEnums.OBS_OTHERPERSON]
        hideChkBox.checked = fields[XemaEnums.OBS_HIDDEN]
        locationAccuracyTf.text = findLocationAccuracy(fields[XemaEnums.OBS_ACCURACY])
        birdAccuracyTf.text = findBirdAccuracy(fields[XemaEnums.OBS_BIRD_ACCURACY])

        var rows = fields[XemaEnums.OBS_ROWCOUNT]
        console.log("rows: " + rows)
        var currentField = 0;
        while (rows > obsCount)
        {
            MyScript.createObjects();
        }
        for(var j = 0; j < rows; j++)
        {
            var rowData = ""
            for(var k = XemaEnums.OBS_BIRDCOUNT+j*12; k < XemaEnums.OBS_WEATHER+j*12; k++)
            {
                rowData += fields[k] + "#"
                currentField = k
            }

            MyScript.setDelegateData(j, rowData)
        }
        currentField++
        weatherTa.text = fields[currentField]
        var birdCoords = fields[XemaEnums.OBS_BIRD_YCOORD] + ":" + fields[XemaEnums.OBS_BIRD_XCOORD]
        console.log("bird-koordinaatit: " + birdCoords )
        if (birdCoords.trim().length < 3) {
            birdCoordinatesTf.text = ""
            return
        }

        if (fields[XemaEnums.OBS_BIRD_YCOORD] < 400) {
            birdCoordinatesTf.text = birdCoords
        } else {
            birdCoordinatesTf.text = CoordinateConverter.ykjToWgsString(birdCoords)
        }
    }

    function setData(date, place, species)
    {
        startDateTf.text = date
        locationTf.text = place
        if (species != "")
        {
            if (species.indexOf(" (") != -1)
            {
                console.log("loyty sulku")
                species = species.substring(0, (species.indexOf(" (")))
            }
        }

        birdNameTf.text = species
    }

    function clearTab()
    {
        console.log("clear tab: " + currentTab)
        if (currentTab <= 3)
        {
            birdNameTf.text = ""
            birdCoordinatesTf.text = ""
            birdAccuracyTf.text = ""
            directionTf.text = ""
            distanceTf.text = ""
            moreInfoTa.text = ""
            atlasTf.text = ""
            hideChkBox.checked = false

            MyScript.removeObjects();
            MyScript.createObjects();

            MyScript.changeDelegateHeight()
        }
        if (currentTab <= 2)
        {
            locationTf.text = ""
            startTimeTf.text = ""
            endTimeTf.text = ""
            locationAccuracyTf.text = ""
        }
        if (currentTab <= 1)
        {
            startDateTf.text = ""
            stopDateTf.text = ""
            regPeopleTa.text = ""
//            otherPeopleTa.text = ""
            weatherTa.text = ""
            unsavedData = false
        }
        MyScript.clearObsDataSelections()
    }

    function showErrorDialog()
    {
        saveErrorDialog.open()
    }

    function dataChanged()
    {
        if (edited == false)
        {
            edited = MyScript.delegateDataChanged()
        }
        return edited
    }

    function initDataChanged()
    {
        console.log("initDataChanged")
        //MyScript.initDelegateDataChanged()
        edited = false
        currentId = 0
    }

    function dateSelected(object, d, m, y) {
        console.log('parametrit: ' +y +", "+m+", "+d)
        d = d + ""
        m = m + ""
        if (d.length < 2) {
            d = "0" + d
        }
        if (m.length < 2) {
            m = "0" + m
        }

        object.text = d + "." + m + "." + y
    }

    function dateRejected(object) {
        object.text = ""
    }

    function timeRejected(object) {
        object.text = ""
    }

    function timeSelected(object, h, m) {
        console.log('parametrit: ' +h +", "+m)
        h = h + ""
        m = m + ""
        if (h.length < 2) {
            h = "0" + h
        }
        if (m.length < 2) {
            m = "0" + m
        }

        object.text = h + ":" + m
    }

    function setDateToPicker(text) {
        var fields = text.split(".")
        datePickerDialog.year = fields[2]
        datePickerDialog.month = fields[1]
        datePickerDialog.day = fields[0]
    }

    function setTimeToPicker(text) {
        var fields = text.split(":")
        timePickerDialog.hour = fields[0]
        timePickerDialog.minute = fields[1]
    }

    IntValidator {
        id: notZero
        bottom: 0
    }

    DatePickerDialog{
        id: datePickerDialog

        property variant targetObject: null
        minimumYear: 1950

        titleText: qsTr("Select Date")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        onAccepted: {dateSelected(targetObject, datePickerDialog.day, datePickerDialog.month, datePickerDialog.year)}
        onRejected: {dateRejected(targetObject)}
    }

    TimePickerDialog{
        id: timePickerDialog

        property variant targetObject: null

        fields: DateTime.Hours | DateTime.Minutes;
        titleText: qsTr("Select Time")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        onAccepted: {timeSelected(targetObject, timePickerDialog.hour, timePickerDialog.minute)}
        onRejected: {timeRejected(targetObject)}
    }


    Dialog {
        id: saveErrorDialog

        title: Label {
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
            height: 150 //saveErrorDialogText.paintedHeight
            width: parent.width
            anchors.topMargin: 10
            Label {
                id: saveErrorDialogText
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: {
                    var dialogText = qsTr("Mandatory information missing:")
                    if(missingInfo) {
                        for(var i = 0; i < missingInfo.length; i++)
                        {
                            dialogText += "\n" + missingInfo[i];
                        }
                        return dialogText;
                    }
                    return "";
                }
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 20
            }
        }

        buttons: Item { height: saveErrorDialogButton.height + 2 * 20; width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: saveErrorDialogButton
                anchors.bottom: parent.bottom
                anchors.margins: 5
                anchors.horizontalCenter: parent.horizontalCenter
                width: 200
                text: qsTr("Ok")
                onClicked: {
                    missingInfo = ""
                    saveErrorDialog.close()
                }
            }
        }
    }

    TabBarLayout {
        id: tabBarLayout
        anchors { left: parent.left; right: parent.right; top: parent.top }
        TabButton { tab: tab1content; text: qsTr("Day") }
        TabButton { tab: tab2content; text: qsTr("Place") }
        TabButton { tab: tab3content; text: qsTr("Observation") }
    }

    TabGroup {
        id: tabGroup
        currentTab: tab1content
        anchors { left: parent.left; right: parent.right; top: tabBarLayout.bottom; bottom: parent.bottom }
        onCurrentTabChanged: {
            if (currentTab == tab1content)
            {
                obsPage.currentTab = 1
            }
            else if (currentTab == tab2content)
            {
                obsPage.currentTab = 2
            }
            else if (currentTab == tab3content)
            {
                obsPage.currentTab = 3
            }
        }

        // define the content for tab 1
        Page {
            id: tab1content
            clip: true

            Flickable {
                id: flickable2
                clip: false
                flickableDirection: Flickable.VerticalFlick
                anchors.fill: parent
                anchors.margins: 5
                contentWidth: width
                contentHeight: detailLevel > 1 ? item3.y + item3.height : item4.y + item4.height

                Timer {
                  id: adjuster2
                  interval: 200
                  onTriggered: parent.adjust()
                }

                Component.onCompleted: {
                  inputContext.visibleChanged.connect(adjuster2.restart)
                }

                function adjust() {
                    if (!inputContext.visible) {
                        return
                    }

                    var focusChild = null
                    function findFocusChild(p) {
                        if (p["activeFocus"] === true) {
                            focusChild = p
                        }
                        else {
                            for (var i = 0; i < p["children"].length; i++) {
                                findFocusChild(p["children"][i])
                                if (focusChild !== null) {
                                    break
                                }
                            }
                        }
                    }
                    findFocusChild(flickable2)

                    if (focusChild === null) {
                        return
                    }

                    var focusChildY = focusChild.parent["y"]
                    var focusChildHeight = focusChild["height"]
                    var focusChildHeaderHeight = focusChild["headerHeight"]
                    flickable2.contentY = focusChildY - focusChildHeaderHeight
                }

                Label {
                    id: text2
                    color: "#ffffff"
                    text: qsTr("Date")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    font.pixelSize: 18
                }

                Item {
                    id: item2
                    height: 50
                    anchors.top: text2.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    TextField {
                        id: startDateTf
                        width: 160
                        height: 50
                        placeholderText: "0.0.0000 *"
                        text: ""
                        inputMethodHints: Qt.ImhPreferNumbers
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        onTextChanged: obsPage.edited = true
                        MouseArea {
                            id: startDateMouse
                            anchors.fill: parent
                            z: startDateTf.z + 1
                            onClicked: {
                                if (startDateTf.text == "")
                                {
                                    startDateTf.text = Qt.formatDateTime(new Date(), "dd.MM.yyyy")
                                }
                                setDateToPicker(startDateTf.text);
                                datePickerDialog.targetObject = startDateTf
                                datePickerDialog.open()
                            }
                        }
                    }

                    Label {
                        id: text3
                        height: 50
                        color: "#ffffff"
                        text: qsTr("-")
                        anchors.right: stopDateTf.left
                        anchors.rightMargin: 0
                        anchors.left: startDateTf.right
                        anchors.leftMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        font.pixelSize: 18
                        visible: detailLevel > 2
                    }

                    TextField {
                        id: stopDateTf
                        width: 160
                        height: 50
                        placeholderText: "0.0.0000"
                        text: ""
                        inputMethodHints: Qt.ImhPreferNumbers
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        visible: detailLevel > 2
                        MouseArea {
                            id: stopDateMouse
                            anchors.fill: parent
                            z: stopDateTf.z + 1
                            onClicked: {
                                if (stopDateTf.text == "")
                                {
                                    if (startDateTf.text != "")
                                    {
                                        stopDateTf.text = startDateTf.text
                                    } else {
                                        stopDateTf.text = Qt.formatDateTime(new Date(), "dd.MM.yyyy")
                                    }
                                }
                                setDateToPicker(stopDateTf.text);
                                datePickerDialog.targetObject = stopDateTf
                                datePickerDialog.open()
                            }
                        }
                        onTextChanged: obsPage.edited = true
                    }
                }
                Label {
                    id: text6
                    color: "#ffffff"
                    text: qsTr("Observers")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: item2.bottom
                    anchors.topMargin: 8
                    font.pixelSize: 18
                }

                Item {
                    id: item4
                    height: 100
                    anchors.top: text6.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    TextArea {
                        id: regPeopleTa
                        height: 100
                        placeholderText: qsTr("Observers *")
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        MouseArea {
                            id: ta1mouse
                            anchors.fill: parent
                            onClicked: window.showListPage("regpeople", regPeopleTa.text);
                            z: regPeopleTa.z + 1
                        }
                        onTextChanged: obsPage.edited = true

                    }
                }
                Label {
                    id: text5
                    color: "#ffffff"
                    text: qsTr("Weather")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: item4.bottom
                    anchors.topMargin: 16
                    font.pixelSize: 18
                    visible: detailLevel > 1

                }

                Item {
                    id: item3
                    height: 100
                    anchors.top: text5.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    visible: detailLevel > 1


                    TextArea {
                        id: weatherTa
                        property int headerHeight: text5.height + 8
                        height: 100
                        placeholderText: qsTr("Weather")
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        onTextChanged: obsPage.edited = true
                    }
                }
            }
        }

        // define the content for tab 2
        Page {
            id: tab2content
            clip: true
            Flickable {
                id: flickable1
                clip: false
                flickableDirection: Flickable.VerticalFlick
                anchors.margins: 5
                anchors.fill: parent
                contentWidth: width
                contentHeight: obsTimeItem.y + obsTimeItem.height

                Timer {
                  id: adjuster1
                  interval: 200
                  onTriggered: parent.adjust()
                }

                Component.onCompleted: {
                  inputContext.visibleChanged.connect(adjuster1.restart)
                }

                function adjust() {
                    if (!inputContext.visible) {
                        return
                    }

                    var focusChild = null
                    function findFocusChild(p) {
                        if (p["activeFocus"] === true) {
                            focusChild = p
                        }
                        else {
                            for (var i = 0; i < p["children"].length; i++) {
                                findFocusChild(p["children"][i])
                                if (focusChild !== null) {
                                    break
                                }
                            }
                        }
                    }
                    findFocusChild(flickable1)

                    if (focusChild === null) {
                        return
                    }

                    var focusChildY = focusChild.parent["y"]
                    var focusChildHeight = focusChild["height"]
                    var focusChildHeaderHeight = focusChild["headerHeight"]
                    flickable1.contentY = focusChildY - focusChildHeaderHeight
                }

                Label {
                    id: locationText
                    color: "#ffffff"
                    text: qsTr("Location")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    font.pixelSize: 18
                }

                Item {
                    id: locationItem
                    width: parent.width
                    height: detailLevel > 1 ? 140 : 50
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: locationText.bottom
                    anchors.topMargin: 8

                    TextField {
                        id: locationTf
                        height: 50
                        placeholderText: qsTr("Location *")
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        MouseArea {
                            id: tf1mouse
                            anchors.fill: parent
                            onClicked: window.showListPage("places");
                            z: locationTf.z + 1
                        }
                        onTextChanged: {
                            obsPage.edited = true
                            var locationCoords = getLocationCoords()
                            if (locationCoords != "" && distanceTf.text != "" && directionTf.text != "") {
                                var coordinates = CoordinateConverter.countCoordinates(locationCoords, distanceTf.text, directionTf.text)
                                birdCoordinatesTf.text = coordinates
                            }
                        }
                        validator: RegExpValidator{ regExp: /.{1,}/ }
                    }

                    Label {
                        id: locationAccuracyLabel
                        color: "#ffffff"
                        text: qsTr("Accuracy")
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        anchors.top: locationTf.bottom
                        anchors.topMargin: 8
                        font.pixelSize: 18
                        visible: detailLevel > 1
                    }

                    TextField {
                        id: locationAccuracyTf
                        property int headerHeight: 0
                        height: 50
                        placeholderText: qsTr("Accuracy")
                        text: ""
                        anchors.top: locationAccuracyLabel.bottom
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        visible: detailLevel > 1
                        width: 200
                        MouseArea {
                            id: locationAccuracyMouse
                            anchors.fill: parent
                            onClicked: window.showListPage("locationaccuracy", locationAccuracyTf.text, locationAccuracyTf);
                            z: directionTf.z + 1
                        }
                        onTextChanged: {
                            obsPage.edited = true
                        }
                    }

                }

                Label {
                    id: obsTimeText
                    color: "#ffffff"
                    text: qsTr("Time")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: locationItem.bottom
                    anchors.topMargin: 8
                    font.pixelSize: 18
                    visible: detailLevel > 1

                }

                Item {
                    id: obsTimeItem
                    height: 50
                    anchors.top: obsTimeText.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    visible: detailLevel > 1


                    TextField {
                        id: startTimeTf
                        property int headerHeight: obsTimeText.height + 8
                        width: 160
                        height: 50
                        placeholderText: "0:00"
                        text: ""
                        inputMethodHints: Qt.ImhPreferNumbers

                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        onTextChanged: obsPage.edited = true
                        MouseArea {
                            id: startTimeMouse
                            anchors.fill: parent
                            z: startTimeTf.z + 1
                            onClicked: {
                                if (startTimeTf.text == "")
                                {
                                    startTimeTf.text = Qt.formatDateTime(new Date(), "hh:mm")
                                }
                                setTimeToPicker(startTimeTf.text);
                                timePickerDialog.targetObject = startTimeTf
                                timePickerDialog.open()
                            }
                        }
                    }

                    Label {
                        id: timeDashText
                        height: 50
                        color: "#ffffff"
                        text: qsTr("-")
                        anchors.left: startTimeTf.right
                        anchors.leftMargin: 0
                        anchors.right: endTimeTf.left
                        anchors.rightMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        font.pixelSize: 18
                    }

                    TextField {
                        id: endTimeTf
                        property int headerHeight: obsTimeText.height + 8

                        width: 160
                        height: 50
                        placeholderText: "0:00"
                        text: ""
                        inputMethodHints: Qt.ImhPreferNumbers

                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        onActiveFocusChanged: {
                            if (activeFocus == true && text == "")
                            {
                                if (startTimeTf.text != "")
                                {
                                    text = startTimeTf.text
                                }
                                else
                                {
                                    text = Qt.formatDateTime(new Date(), "hh:mm")
                                }
                            }
                        }
                        onTextChanged: obsPage.edited = true
                        MouseArea {
                            id: endTimeMouse
                            anchors.fill: parent
                            z: endTimeTf.z + 1
                            onClicked: {
                                if (endTimeTf.text == "")
                                {
                                    if (startTimeTf.text != "") {
                                        endTimeTf.text = startTimeTf.text
                                    } else {
                                        endTimeTf.text = Qt.formatDateTime(new Date(), "hh:mm")
                                    }
                                }
                                setTimeToPicker(endTimeTf.text);
                                timePickerDialog.targetObject = endTimeTf
                                timePickerDialog.open()
                            }
                        }
                    }
                }
            }
        }

        // define content for tab 3
        Page {
            id: tab3content
            clip: true
            Flickable {
                id: flickable3
                clip: false
                flickableDirection: Flickable.VerticalFlick
                anchors.fill: parent
                anchors.margins: 5
                contentWidth: width
                contentHeight: detailLevel > 2 ? obsExtraInfoItem.y + obsExtraInfoItem.height : plus.y + plus.height

                Timer {
                  id: adjuster3
                  interval: 200
                  onTriggered: parent.adjust()
                }

                Component.onCompleted: {
                  inputContext.visibleChanged.connect(adjuster3.restart)
                }

                function adjust() {
                    if (!inputContext.visible) {
                        return
                    }

                    var focusChild = null
                    function findFocusChild(p) {
                        if (p["activeFocus"] === true) {
                            focusChild = p
                        }
                        else {
                            for (var i = 0; i < p["children"].length; i++) {
                                findFocusChild(p["children"][i])
                                if (focusChild !== null) {
                                    break
                                }
                            }
                        }
                    }
                    findFocusChild(flickable3)

                    if (focusChild === null) {
                        return
                    }

                    var delegate = false;
                    var focusChildY = focusChild.parent["y"]
                    console.log("parent: " + focusChild.parent.parent["objectName"])
                    if ( focusChild.parent.parent["objectName"] == "obsDelegateItem" ) {
                        delegate = true;
                    }

                    var focusChildHeight = focusChild["height"]
                    var focusChildHeaderHeight = focusChild["headerHeight"]
                    if (focusChildHeaderHeight === undefined)
                    {
                        focusChildHeaderHeight = 0
                    }

                    if (delegate) {
                        focusChildY += focusChild.parent.parent["y"]
                        console.log("delegaatti, focusChildY: " + focusChildY)
                    }

                    console.log("focusChildY: " + focusChildY +", focusChildHeight: " +focusChildHeight +", focusChildHeaderHeight: " +focusChildHeaderHeight)
                    console.log("new contentY: " + (focusChildY - focusChildHeaderHeight))
                    flickable3.contentY = focusChildY - focusChildHeaderHeight
                }

                Label {
                    id: text7
                    color: "#ffffff"
                    text: qsTr("Observation")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    font.pixelSize: 18
                }

                Item {
                    id: birdNameItem
                    height: 50
                    anchors.top: text7.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    TextField {
                        id: birdNameTf
                        height: 50
                        placeholderText: qsTr("Species *")
                        text: ""
                        validator: RegExpValidator{ regExp: /.{1,}/ }
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        MouseArea {
                            id: tf7mouse
                            anchors.fill: parent
                            onClicked: window.showListPage("birds");
                            z: birdNameTf.z + 1
                        }
                        onTextChanged: obsPage.edited = true

                    }
                }
                Label {
                    id: birdPlaceLabel
                    color: "#ffffff"
                    text: qsTr("Location of bird")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: birdNameItem.bottom
                    anchors.topMargin: 8
                    font.pixelSize: 18
                    visible: detailLevel > 1
                }

                Item {
                    id: birdPlaceItem
                    height: detailLevel > 2 ? 220 : 140
                    anchors.top: birdPlaceLabel.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    visible: detailLevel > 1

                    TextField {
                        id: birdCoordinatesTf
                        property int headerHeight: 0
                        placeholderText: qsTr("Coordinates")
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        anchors.right: mapButton.left
                        anchors.rightMargin: 8
                        height: 50
                        onTextChanged: obsPage.edited = true
                        enabled: false;
                    }

                    Button {
                        id: mapButton
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        width: 150
                        text: qsTr("Map")
                        onClicked: obsPage.openMap()
                    }

                    Label {
                        id: birdAccuracyLabel
                        color: "#ffffff"
                        text: qsTr("Accuracy")
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        anchors.top: birdCoordinatesTf.bottom
                        anchors.topMargin: 8
                        font.pixelSize: 18
                        visible: detailLevel > 1
                    }

                    TextField {
                        id: birdAccuracyTf
                        property int headerHeight: 0
                        height: 50
                        placeholderText: qsTr("Accuracy")
                        text: ""
                        anchors.top: birdAccuracyLabel.bottom
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        visible: detailLevel > 1
                        width: 200
                        MouseArea {
                            id: birdAccuracyMouse
                            anchors.fill: parent
                            onClicked: window.showListPage("birdaccuracy", birdAccuracyTf.text, birdAccuracyTf);
                            z: directionTf.z + 1
                        }
                        onTextChanged: {
                            obsPage.edited = true
                        }
                    }

                    Label {
                        id: fromPlaceToBirdLabel
                        color: "#ffffff"
                        text: qsTr("From place to bird")
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        anchors.top: birdAccuracyTf.bottom
                        anchors.topMargin: 8
                        font.pixelSize: 18
                        visible: detailLevel > 2
                    }

                    TextField {
                        id: directionTf
                        property int headerHeight: -y
                        height: 50
                        placeholderText: qsTr("Direction")
                        text: ""
                        anchors.top: fromPlaceToBirdLabel.bottom
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        visible: detailLevel > 2
                        width: 200
                        MouseArea {
                            id: directionMouse
                            anchors.fill: parent
                            onClicked: window.showListPage("direction", directionTf.text, directionTf);
                            z: directionTf.z + 1
                        }
                        onTextChanged: {
                            obsPage.edited = true
                            console.log("directionTf: " + directionTf.text)
                            var locationCoords = getLocationCoords()
                            if (locationCoords != "") {
                                var coordinates = CoordinateConverter.countCoordinates(locationCoords, distanceTf.text, directionTf.text)
                                birdCoordinatesTf.text = coordinates
                            }
                        }
                    }
                    TextField {
                        id: distanceTf
                        property int headerHeight: -y
                        height: 50
                        placeholderText: qsTr("Distance (m)")
                        text: ""
                        anchors.left: directionTf.right
                        anchors.leftMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: fromPlaceToBirdLabel.bottom
                        anchors.topMargin: 8
                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: text != "" ? notZero : null
                        visible: detailLevel > 2
                        onTextChanged: {
                            obsPage.edited = true
                            console.log("distanceTf: " + distanceTf.text)
                            var locationCoords = getLocationCoords()
                            if (locationCoords != "") {
                                var coordinates = CoordinateConverter.countCoordinates(locationCoords, distanceTf.text, directionTf.text)
                                birdCoordinatesTf.text = coordinates
                            }
                        }
                    }
                }

                Item {
                    id: obsDelegateItem
                    objectName: "obsDelegateItem"
                    anchors.top: detailLevel > 2 ? birdPlaceItem.bottom : detailLevel > 1 ? birdPlaceItem.bottom : birdNameItem.bottom;
                    anchors.topMargin: 16
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    height: childrenRect.height
                }
                Button {
                    id: plus
                    text: qsTr("Add row")
                    anchors.top: obsDelegateItem.bottom
                    anchors.topMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    width: 170
                    onClicked: {
                        MyScript.createObjects();
                    }
                }
                Button {
                    id: minus
                    text: qsTr("Remove row")
                    anchors.top: obsDelegateItem.bottom
                    anchors.topMargin: 8
                    anchors.left: plus.right
                    anchors.leftMargin: 8
                    width: 170
                    visible: obsCount > 1
                    onClicked: {
                        MyScript.removeObject();
                    }
                }

                Label {
                    id: text8
                    color: "#ffffff"
                    text: qsTr("Additional information")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: plus.bottom
                    anchors.topMargin: 8
                    font.pixelSize: 18
                    visible: detailLevel > 2

                }
                Item {
                    id: obsExtraInfoItem
                    height: 200
                    anchors.top: text8.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    visible: detailLevel > 2


                    TextArea {
                        id: moreInfoTa
                        property int headerHeight: 0
                        height: 150
                        placeholderText: qsTr("More information")
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        visible: detailLevel > 2
                        onTextChanged: obsPage.edited = true

                    }

                    TextField {
                        id: atlasTf
                        height: 50
                        placeholderText: qsTr("Atlas index")
                        text: ""
                        anchors.top: moreInfoTa.bottom
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        width: 125
                        visible: detailLevel > 2
                        validator: text != "" ? notZero : null
                        inputMethodHints: Qt.ImhDigitsOnly
                        MouseArea {
                            id: atlastfmouse
                            anchors.fill: parent
                            onClicked: window.showListPage("atlas", atlasTf.text, atlasTf);
                            z: atlasTf.z + 1
                        }
                        onTextChanged: obsPage.edited = true
                    }
                    CheckBox {
                        id: hideChkBox
                        anchors.top: moreInfoTa.bottom
                        anchors.topMargin: 2
                        anchors.left: atlasTf.right
                        anchors.leftMargin: 5
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        text: qsTr("Hidden")
                        visible: detailLevel > 2
                        onCheckedChanged: obsPage.edited = true
                    }
                    CheckBox {
                        id: exportedChkBox
                        anchors.top: hideChkBox.bottom
                        anchors.topMargin: 0
                        anchors.left: atlasTf.right
                        anchors.leftMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        text: ""
                        visible: false
                    }
                }
            }
        }
    }
}
