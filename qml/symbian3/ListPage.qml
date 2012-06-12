import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.location 1.1

Page {
    id: listPage
    tools: listToolBarLayout

    ToolBarLayout {
        id: listToolBarLayout
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
                window.backFromList()
                positionSource.stop()
                pageStack.pop()
            }
        }
        ToolButton {
            flat: false
            text: positionSource.active ? qsTr("Stop GPS") : qsTr("Start GPS")
//            iconSource: "toolbar-search"
            visible: listView.model == locationModel && listView.editMode == false
            onClicked: {
                console.log("start gps")
                if(positionSource.active)
                {
                    positionSource.stop()
                }
                else
                {
                    positionSource.start()
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
        listView.model.filter("")
        filterTf.text = ""
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
/*
    ListModel {
        id: sexModel
        ListElement {
            name: ""
            engname: ""
            swename: ""
            selected: false
        }
        ListElement {
            name: "koiras"
            engname: "male"
            swename: "hane"
            selected: false
        }
        ListElement {
            name: "naaras"
            engname: "female"
            swename: "hona"
            selected: false
        }
        ListElement {
            name: "pariutuneet"
            engname: "couple"
            swename: "i par"
            selected: false
        }
    }

    ListModel {
        id: ageModel
        ListElement {    engname: ""; swename: ""; name: "" }
        ListElement {    engname: "fl"; swename: "fl"; name: "fl" }
        ListElement {    engname: "pm"; swename: "pm"; name: "pm" }
        ListElement {    engname: "pp"; swename: "pp"; name: "pp" }
        ListElement {    engname: "+1kv"; swename: "+1kv"; name: "+1kv" }
        ListElement {    engname: "1kv"; swename: "1kv"; name: "1kv" }
        ListElement {    engname: "+2kv"; swename: "+2kv"; name: "+2kv" }
        ListElement {    engname: "2kv"; swename: "2kv"; name: "2kv" }
        ListElement {    engname: "+3kv"; swename: "+3kv"; name: "+3kv" }
        ListElement {    engname: "3kv"; swename: "3kv"; name: "3kv" }
        ListElement {    engname: "+4kv"; swename: "+4kv"; name: "+4kv" }
        ListElement {    engname: "4kv"; swename: "4kv"; name: "4kv" }
        ListElement {    engname: "+5kv"; swename: "+5kv"; name: "+5kv" }
        ListElement {    engname: "5kv"; swename: "5kv"; name: "5kv" }
        ListElement {    engname: "+6kv"; swename: "+6kv"; name: "+6kv" }
        ListElement {    engname: "6kv"; swename: "6kv"; name: "6kv" }
        ListElement {    engname: "+7kv"; swename: "+7kv"; name: "+7kv" }
        ListElement {    engname: "7kv"; swename: "7kv"; name: "7kv" }
        ListElement {    engname: "+8kv"; swename: "+8kv"; name: "+8kv" }
        ListElement {    engname: "8kv"; swename: "8kv"; name: "8kv" }
    }

    ListModel {
        id: dressModel
        ListElement { engname: ""; swename: ""; name: "" }
        ListElement { engname: "ad"; swename: "ad"; name: "ad" }
        ListElement { engname: "eijp"; swename: "eijp"; name: "eijp" }
        ListElement { engname: "imm"; swename: "imm"; name: "imm" }
        ListElement { engname: "jp"; swename: "jp"; name: "jp" }
        ListElement { engname: "juv"; swename: "juv"; name: "juv" }
        ListElement { engname: "n-puk"; swename: "n-puk"; name: "n-puk" }
        ListElement { engname: "pep"; swename: "pep"; name: "pep" }
        ListElement { engname: "pull"; swename: "pull"; name: "pull" }
        ListElement { engname: "ss"; swename: "ss"; name: "ss" }
        ListElement { engname: "subad"; swename: "subad"; name: "subad" }
        ListElement { engname: "tp"; swename: "tp"; name: "tp" }
        ListElement { engname: "vp"; swename: "vp"; name: "vp" }
    }
*/
    Component.onCompleted: {
        console.log("ListPage loaded")
        listView.model.filter("")
        for(var i = 0; i < listView.model.rowCount(); i++)
        {
            listView.model.setData(i, false, 2)
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
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            onTextChanged: {
                console.log("teksti muuttuu: " + text)
                listView.model.filter(text)
            }
            BusyIndicator {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                width: height
                running: positionSource.active
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
        model: {
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
        }
        anchors.top: item1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        delegate: {
            if (listPageType == "birds")
            {
                if (window.useSystematicSort == true)
                {
                    model.setSorting(0, true)
                }
                else
                {
                    model.setSorting(1, true)
                }
                editMode = false
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
        }
        clip: true
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
                    var latitude = positionSource.position.coordinate.latitude.toFixed(1)
                    var longitude = positionSource.position.coordinate.longitude.toFixed(1)
                    filterTf.text = latitude + ":" + longitude
                    console.log("accuracy now: " + positionSource.accuracy)
                }
            }
        }
    }
}
