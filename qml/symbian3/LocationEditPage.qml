import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript
import QtMobility.location 1.1



Page {
    id: locationEditPage
    tools: editToolBar
    property bool locationEdited: false
    property bool addingNew: false

    ToolBarLayout {
        id: editToolBar
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
                console.log("pep sivu back clicked")
                positionSource.stop()
                if (locationEdited == true)
                {
                    if (addingNew == true)
                    {
                        console.log("adding new")
                        if (!mandatoryInfoExists())
                        {
                            pageStack.pop()
                            return
                        }

                        var rows = locationModel.rowCount()
                        locationModel.setData(rows, townTf.text, 35)
                        locationModel.setData(rows, locationTf.text, 36)
                        locationModel.setData(rows, sweTownTf.text, 44)
                        locationModel.setData(rows, sweLocationTf.text, 45)
                        locationModel.setData(rows, engTownTf.text, 46)
                        locationModel.setData(rows, engLocationTf.text, 47)
                        locationModel.setData(rows, ykjTf.text, 38)
                        locationModel.setData(rows, wgsTf.text, 39)
                        locationModel.setData(rows, true, 48)
                        locationModel.setData(rows, countryTf.text, 52)
                        locationModel.setData(rows, sweCountryTf.text, 56)
                        locationModel.setData(rows, engCountryTf.text, 57)
                        locationModel.setData(rows, organizationTf.text, 58)
                        locationModel.setData(rows, orgAbbrevTf.text, 59)
                    }
                    else
                    {
                        locationModel.setData(currentIndex, townTf.text, 35)
                        locationModel.setData(currentIndex, locationTf.text, 36)
                        locationModel.setData(currentIndex, sweTownTf.text, 44)
                        locationModel.setData(currentIndex, sweLocationTf.text, 45)
                        locationModel.setData(currentIndex, engTownTf.text, 46)
                        locationModel.setData(currentIndex, engLocationTf.text, 47)
                        locationModel.setData(currentIndex, ykjTf.text, 38)
                        locationModel.setData(currentIndex, wgsTf.text, 39)
                        locationModel.setData(currentIndex, true, 48)
                        locationModel.setData(currentIndex, countryTf.text, 52)
                        locationModel.setData(currentIndex, sweCountryTf.text, 56)
                        locationModel.setData(currentIndex, engCountryTf.text, 57)
                        locationModel.setData(currentIndex, organizationTf.text, 58)
                        locationModel.setData(currentIndex, orgAbbrevTf.text, 59)
                    }
                    window.saveLocations()
                    if ( currentLanguage == "sv") {
                        window.newLocationCreated( sweLocationTf.text + ", " + sweTownTf.text )
                    }
                    else if ( currentLanguage == "en") {
                        window.newLocationCreated( engLocationTf.text + ", " + engTownTf.text )
                    }
                    else {
                        window.newLocationCreated( locationTf.text + ", " + townTf.text )
                    }

                }
                pageStack.pop()
            }
        }
        ToolButton {
            flat: true
            iconSource: "/qml/symbian3_icons/undo.svg"
            visible: !addingNew
            onClicked: {
                console.log("location edit page undo clicked")
                undo()
            }
        }
    }

    property int currentIndex: -1

    function mandatoryInfoExists()
    {
        var dataOk = false;
        if (townTf.text.length > 0 &&
            locationTf.text.length)
        {
            dataOk = true;
        }
        else if (sweTownTf.text.length > 0 &&
            sweLocationTf.text.length)
        {
            dataOk = true;
        }
        else if (engTownTf.text.length > 0 &&
            engLocationTf.text.length)
        {
            dataOk = true;
        }
        return dataOk;
    }
    function addNew()
    {
        console.log("adding newjoo")
        addingNew = true
        if ( currentLanguage == "sv") {
            sweCountryTf.text = window.defaultCountry
        }
        else if ( currentLanguage == "en") {
            engCountryTf.text = window.defaultCountry
        }
        else {
            countryTf.text = window.defaultCountry
        }
    }

    function selectedIndex(index)
    {
        currentIndex = index
        console.log("valittu: " + locationModel.data(currentIndex, 35))
        locationEdited = false
    }

    function undo()
    {
        townTf.text = locationModel.data(currentIndex, 35)
        locationTf.text = locationModel.data(currentIndex, 36)
        sweTownTf.text = locationModel.data(currentIndex, 44)
        sweLocationTf.text = locationModel.data(currentIndex, 45)
        engTownTf.text = locationModel.data(currentIndex, 46)
        engLocationTf.text = locationModel.data(currentIndex, 47)
        ykjTf.text = locationModel.data(currentIndex, 38)
        wgsTf.text = locationModel.data(currentIndex, 39)
        countryTf.text = locationModel.data(currentIndex, 52)
        sweCountryTf.text = locationModel.data(currentIndex, 56)
        engCountryTf.text = locationModel.data(currentIndex, 57)
        organizationTf.text = locationModel.data(currentIndex, 58)
        orgAbbrevTf.text = locationModel.data(currentIndex, 59)
    }

    function openMap() {
        var x;
        var y;
        if (wgsTf.text != "") {
            var coords = wgsTf.text.split(":")
            x = coords[0]
            y = coords[1]
        }

        window.showPlaceMap(wgsTf, x, y)
    }

    Flickable {
        id: flickable1
        contentWidth: width
        contentHeight: gpsButton.y + gpsButton.height + 10
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        anchors.leftMargin: 5

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

            var focusChildY = focusChild["y"]
            var focusChildHeight = focusChild["height"]
            var focusChildHeaderHeight = focusChild["headerHeight"]
            flickable1.contentY = focusChildY
        }


        Label {
            id: editText
            text: qsTr("Edit location")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: parent.top
            color: "#ffffff"
        }

        TextField {
            id: townTf
            width: 240
            height: 50
            placeholderText: qsTr("Town in finnish")
            text: locationModel.data(currentIndex, 35)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "fi"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: locationTf
            width: 240
            height: 50
            placeholderText: qsTr("Location in finnish")
            text: locationModel.data(currentIndex, 36)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: townTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "fi"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: countryTf
            width: 240
            height: 50
            placeholderText: qsTr("Country in finnish")
            text: locationModel.data(currentIndex, 52)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: locationTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "fi"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: sweTownTf
            width: 240
            height: 50
            placeholderText: qsTr("Town in swedish")
            text: locationModel.data(currentIndex, 44)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "sv"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: sweLocationTf
            width: 240
            height: 50
            placeholderText: qsTr("Location in swedish")
            text: locationModel.data(currentIndex, 45)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: sweTownTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "sv"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: sweCountryTf
            width: 240
            height: 50
            placeholderText: qsTr("Country in swedish")
            text: locationModel.data(currentIndex, 56)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: sweLocationTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "sv"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: engTownTf
            width: 240
            height: 50
            placeholderText: qsTr("Town in english *")
            text: locationModel.data(currentIndex, 46)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: editText.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "en"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: engLocationTf
            width: 240
            height: 50
            placeholderText: qsTr("Location in english *")
            text: locationModel.data(currentIndex, 47)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: engTownTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "en"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: engCountryTf
            width: 240
            height: 50
            placeholderText: qsTr("Country in english")
            text: locationModel.data(currentIndex, 57)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: engLocationTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            visible: window.currentLanguage == "en"
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: organizationTf
            width: 240
            height: 50
            placeholderText: qsTr("Organization")
            text: locationModel.data(currentIndex, 58)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: engCountryTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: orgAbbrevTf
            width: 240
            height: 50
            placeholderText: qsTr("Org abbreviation")
            text: locationModel.data(currentIndex, 59)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: organizationTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 0
            onTextChanged: {
                locationEdited = true
            }
        }

        TextField {
            id: ykjTf
            property bool settingValue: false
            placeholderText: qsTr("YKJ coordinates")
            text: locationModel.data(currentIndex, 38)
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: orgAbbrevTf.bottom
            anchors.topMargin: 8
            width: parent.width / 2
            onTextChanged: {
                if (ykjTf.settingValue == false) {
                    locationEdited = true
                    console.log("ykjTf: " + ykjTf.text)
                    var wgsString = CoordinateConverter.ykjToWgsString(ykjTf.text)
                    wgsTf.settingValue = true
                    wgsTf.text = wgsString
                    wgsTf.settingValue = false
                }
            }
        }

        TextField {
            id: wgsTf
            property bool settingValue: false
            placeholderText: qsTr("WGS coordinates")
            text: locationModel.data(currentIndex, 39)
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: orgAbbrevTf.bottom
            anchors.topMargin: 8
            width: parent.width / 2
            onTextChanged: {
                if (wgsTf.settingValue == false) {
                    locationEdited = true
                    console.log("wgsTf: " + wgsTf.text)
                    var ykjString = CoordinateConverter.wgsToYkjString(wgsTf.text)
                    ykjTf.settingValue = true
                    ykjTf.text = ykjString
                    ykjTf.settingValue = false
                }
            }
        }

        Button {
            id: mapButton
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: wgsTf.bottom
            anchors.topMargin: 8
            width: 200
            text: qsTr("Map")
            onClicked: locationEditPage.openMap()
        }
        Button {
            id: gpsButton
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: mapButton.bottom
            anchors.topMargin: 8
            width: 200
            text: qsTr("Start GPS")
            visible: positionSource.positioningMethod == PositionSource.SatellitePositioningMethod || positionSource.positioningMethod == PositionSource.AllPositioningMethods
            onClicked: {
                if (gpsButton.state != "gpsInUse")
                {
                    console.log("start")
                    positionSource.start()
                }
                else
                {
                    console.log("stop")
                    positionSource.stop()
                }
            }
            states: [
                State {
                    name: "gpsInUse"
                    when: positionSource.active
                    PropertyChanges {
                        target: gpsButton
                        text: qsTr("Stop GPS")
                    }
                }

            ]
        }
        Label {
            anchors.left: busyIndicator.right
            anchors.top: gpsButton.top
            height: wgsTf.height
            visible: positionSource.active && positionSource.coordinatesValid
            text: qsTr("Accuracy: %1 m").arg(positionSource.accuracy)
            color: "white"
            verticalAlignment: Text.AlignVCenter
        }

        BusyIndicator {
            id: busyIndicator
            anchors.left: gpsButton.right
            anchors.top: gpsButton.top
            height: gpsButton.height
            width: height
            running: positionSource.active
            visible: running
        }

        PositionSource {
            id: positionSource
            property string accuracy: maxVerticalAccurary <= maxHorizontalAccurary ? parseInt(maxHorizontalAccurary) : parseInt(maxVerticalAccurary)
            property real maxHorizontalAccurary: 999999
            property real maxVerticalAccurary: 999999
            property bool coordinatesValid: (positionSource.position.latitudeValid && positionSource.position.longitudeValid &&
                                            positionSource.position.coordinate.latitude != "NaN" && positionSource.position.coordinate.longitude != "NaN")
            updateInterval: 2500

            onActiveChanged: {
                console.log("Active Changed called")
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
                        wgsTf.text = latitude + ":" + longitude
                        console.log("accuracy now: " + positionSource.accuracy)
                    }
                }
            }
        }
/*
        Component.onCompleted: {
            console.log("posmethods: " + positionSource.positioningMethod +","+ PositionSource.SatellitePositioningMethod)
            console.log("pos 1: NoPositioningMethod: " + PositionSource.NoPositioningMethod)
            console.log("pos 2: SatellitePositioningMethod: " + PositionSource.SatellitePositioningMethod)
            console.log("pos 3: NonSatellitePositioningMethod: " + PositionSource.NonSatellitePositioningMethod)
            console.log("pos 4: AllPositioningMethods: " + PositionSource.AllPositioningMethods)
        }*/
    }
}
