import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript
import XemaEnums 1.0

Page {

    id: obsPage
    tools: toolBarLayout
    property int detailLevel: window.currentDetailLevel
    property int currentId: 0
    property int currentTab: 1
    property int delegateHeight: detailLevel == 3 ? 300 : detailLevel == 2 ? 150 : 75
    property variant missingInfo: null
    property bool edited: false

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

        MyScript.removeObjects();
        MyScript.createObjects();

        window.currentDetailLevel = window.defaultDetailLevel

        MyScript.changeDelegateHeight()
        for(var i=0;i<personModel.rowCount();i++)
        {
            if (personModel.data(i, 37) == true)
            {
                regPeopleTa.text = personModel.data(i, 35)
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

    function readAllData()
    {
        var missingData = new Array();
        if (startDateTf.text == "")
        {
            missingData[missingData.length] = qsTr("Start date")
        }
        if (birdNameTf.text == "")
        {
            missingData[missingData.length] = qsTr("Species")
        }
        if (regPeopleTa.text == "")
        {
            missingData[missingData.length] = qsTr("Observer")
        }
        if (locationTf.text == "")
        {
            missingData[missingData.length] = qsTr("Location")
        }

        var allData = "";
        var delimiter = "#";

        allData += currentId + delimiter
        var bird_abbrev = findBirdAbbrev(birdNameTf.text)
        allData += bird_abbrev + delimiter
        allData += startDateTf.text + delimiter
        allData += stopDateTf.text + delimiter
        allData += startTimeTf.text + delimiter
        allData += endTimeTf.text + delimiter
        // uusi, town
        var location = findLocation(locationTf.text)
        var town = location.substring(0, location.indexOf(", ") )
        var place = location.substring(location.indexOf(", ")+2 )
        allData += town + delimiter
        allData += place + delimiter
        // uusi, x-coord
        allData += delimiter
        // uusi, y-coord
        allData += delimiter
        // uusi, accuracy
        allData += delimiter
        // uusi, x-coord bird
        allData += delimiter
        // uusi, y-coord bird
        allData += delimiter
        // uusi, accuracy bird
        allData += delimiter
        // uusi, paikannettu
        allData += delimiter
        allData += moreInfoTa.text + delimiter
        var atlas_abbrev = findAtlas(atlasTf.text)
        allData += atlas_abbrev + delimiter
        // uusi, saver
        allData += delimiter
        // uusi, save time
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

    function findLocation(name)
    {
        console.log("findLocation(name)" + name)
        for(var i=0;i<locationModel.rowCount();i++) {
            // TODO localized names
            var town = name.split(", ",2);
            console.log("town[0] " +town[0])
            console.log("town[1] " +town[1])
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
        }
        if (currentTab <= 1)
        {
            startDateTf.text = ""
            stopDateTf.text = ""
            regPeopleTa.text = ""
//            otherPeopleTa.text = ""
            weatherTa.text = ""
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
                        placeholderText: "0.0.0000"
                        text: ""
                        inputMethodHints: Qt.ImhPreferNumbers
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        validator: RegExpValidator {
                            regExp: /[0-2]{0,1}[0-9]{1}[\.]{1}[0-2]{0,1}[0-9]{1}[\.]{1}[0-9]{4}/
                        }
                        onActiveFocusChanged: {
                            if (activeFocus == true && text == "")
                            {
                                text = Qt.formatDateTime(new Date(), "dd.MM.yyyy")
                            }
                        }
                        onTextChanged: obsPage.edited = true
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
                        validator: RegExpValidator {
                            regExp: /[0-2]{0,1}[0-9]{1}[\.]{1}[0-2]{0,1}[0-9]{1}[\.]{1}[0-9]{4}/
                        }
                        onActiveFocusChanged: {
                            if (activeFocus == true && text == "")
                            {
                                if (startDateTf.text != "")
                                {
                                    text = startDateTf.text
                                }
                                else
                                {
                                    text = Qt.formatDateTime(new Date(), "dd.MM.yyyy")
                                }
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
                        placeholderText: qsTr("Registered")
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
                    id: item1
                    width: parent.width
                    height: 50
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: locationText.bottom
                    anchors.topMargin: 8

                    TextField {
                        id: locationTf
                        height: 50
                        placeholderText: qsTr("Location")
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
                        }
                        onTextChanged: obsPage.edited = true
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
                    anchors.top: item1.bottom
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
                        validator: RegExpValidator {
                            regExp: /[0-2]{0,1}[0-9]{1}[\:\.]{1}[0-9]{2}/
                        }
                        onActiveFocusChanged: {
                            if (activeFocus == true && text == "")
                            {
                                text = Qt.formatDateTime(new Date(), "hh:mm")
                            }
                        }
                        onTextChanged: obsPage.edited = true
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
                        validator: RegExpValidator {
                            regExp: /[0-2]{0,1}[0-9]{1}[\:\.]{1}[0-9]{2}/
                        }
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
                contentHeight: detailLevel > 2 ? item8.y + item8.height : plus.y + plus.height

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

                    var focusChildY = focusChild.parent["y"]
                    console.log("parent: " + focusChild.parent.parent["objectName"])
                    if (focusChildY == 0)
                    {
                        console.log("focusChildY 0")
                        focusChildY = focusChild.parent.parent["y"]
                    }

                    var focusChildHeight = focusChild["height"]
                    var focusChildHeaderHeight = focusChild["headerHeight"]
                    if (focusChildHeaderHeight === undefined)
                    {
                        focusChildHeaderHeight = 0
                    }

                    if (focusChildY >= 300)
                    {
                        focusChildY = focusChildY + 97
                    }

                    console.log("fcY: " + focusChildY +", fch: " +focusChildHeight +", fchh: " +focusChildHeaderHeight)
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
                        placeholderText: qsTr("Species")
                        text: ""
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
                        }
                        onTextChanged: obsPage.edited = true

                    }
                }

                Item {
                    id: obsDelegateItem
                    objectName: "obsDelegateItem"
                    anchors.top: birdNameItem.bottom
                    anchors.topMargin: 8
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
                    width: 150
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
                    anchors.leftMargin: 0
                    width: 150
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
                    id: item8
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
                        property int headerHeight: text8.height + 8
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
                        width: 150
                        visible: detailLevel > 2
                        validator: IntValidator { bottom: 0 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        MouseArea {
                            id: atlastfmouse
                            anchors.fill: parent
                            onClicked: window.showListPage("atlas", atlasTf.text, atlasTf);
                        }
                        onTextChanged: obsPage.edited = true
                    }
                    CheckBox {
                        id: hideChkBox
                        anchors.top: moreInfoTa.bottom
                        anchors.topMargin: 0
                        anchors.left: atlasTf.right
                        anchors.leftMargin: 0
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
