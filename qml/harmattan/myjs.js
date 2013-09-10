var component;
var sprite = new Array() ;
var compCount = 0;
var listPageComponent;
var listObject;
var obsPageComponent;
var obsObject;
var historyPageComponent;
var historyObject;
var settingsPageComponent;
var settingsObject;
var mapPageComponent;
var mapObject;
var currentStatusBox;

function createObjects() {
    component = Qt.createComponent("ObservationDelegate.qml");
    sprite[compCount] = component.createObject(obsDelegateItem, {"x": 0, "y": compCount*delegateHeight});
    compCount++;
    obsCount = compCount

    if (sprite == null) {
        // Error Handling
        console.log("Error creating object");
    }

    for(var i = 0; i < sprite.length; i++)
    {
        sprite[i].setRows(compCount)
    }
}

function removeObject() {
    if (compCount > 1)
    {
        compCount--;
        sprite[compCount].destroy()
        sprite.pop()
        obsCount = compCount
    }
}

function removeObjects() {
    while (compCount > 0)
    {
        compCount--;
        sprite[compCount].destroy()
        obsCount = compCount
        sprite.pop()
    }
}

function showListPage(type, selectedItems, itemi)
{
    if (!listObject)
    {
        console.log("creating component")
        listPageComponent = Qt.createComponent(Qt.resolvedUrl("ListPage.qml"))
        listObject = listPageComponent.createObject(window)
    }
    console.log("set type")
    listPageType = type

    pageStack.push(listObject)
    if (type == "regpeople" || type == "people")
    {
        listObject.selectNames(selectedItems)
    }
    else if (type == "status")
    {
        currentStatusBox = itemi
        listObject.selectStatus(selectedItems)
    }
    else if (type == "associations")
    {
        currentStatusBox = itemi
        listObject.selectAssociations(selectedItems)
    }
    else if (type == "sex" || type == "age" || type == "dress" || type == "atlas" || type == "direction"
             || type == "locationaccuracy" || type == "birdaccuracy")
    {
        currentStatusBox = itemi
    }
    console.log("call init")
    listObject.init()
}

function showObsPage(doNotInit)
{
    console.log("showObsPage, doNotInit: " + doNotInit)

    if (obsObject)
    {
        console.log("obs exists, push")/*
        if(settingsPageComponent) {
            settingsObject.destroy()
            settingsPageComponent.destroy()
            settingsObject = null
            settingsPageComponent = null
        }
        if(historyPageComponent) {
            historyObject.destroy()
            historyPageComponent.destroy()
            historyObject = null
            historyPageComponent = null
        }*/

        pageStack.push(obsObject)
        if (doNotInit == false)
        {
            obsObject.init()
        }
        return
    }
    console.log("create obs")
    obsPageComponent = Qt.createComponent(Qt.resolvedUrl("ObservationPage.qml"))
    obsObject = obsPageComponent.createObject(window)/*
    if(settingsPageComponent) {
        settingsObject.destroy()
        settingsPageComponent.destroy()
        settingsObject = null
        settingsPageComponent = null
    }
    if(historyPageComponent) {
        historyObject.destroy()
        historyPageComponent.destroy()
        historyObject = null
        historyPageComponent = null
    }*/

    pageStack.push(obsObject)
    if (doNotInit == false)
    {
        console.log("create obs, init")
        obsObject.init()
    }
}

function showSettingsPage()
{
    if (settingsObject)
    {
        console.log("settings exists, push")/*
        if(obsPageComponent) {
            obsObject.destroy()
            obsPageComponent.destroy()
            obsObject = null
            obsPageComponent = null
        }
        if(historyPageComponent) {
            historyObject.destroy()
            historyPageComponent.destroy()
            historyObject = null
            historyPageComponent = null
        }*/
        pageStack.push(settingsObject)
        return
    }
    settingsPageComponent = Qt.createComponent(Qt.resolvedUrl("SettingsPage.qml"))
    settingsObject = settingsPageComponent.createObject(window)/*
    if(obsPageComponent) {
        obsObject.destroy()
        obsPageComponent.destroy()
        obsObject = null
        obsPageComponent = null
    }
    if(historyPageComponent) {
        historyObject.destroy()
        historyPageComponent.destroy()
        historyObject = null
        historyPageComponent = null
    }*/

    pageStack.push(settingsObject)
}

function showHistoryPage(type)
{
    if (historyObject)
    {/*
        if(obsPageComponent) {
            obsObject.destroy()
            obsPageComponent.destroy()
            obsObject = null
            obsPageComponent = null
        }
        if(settingsPageComponent) {
            settingsObject.destroy()
            settingsPageComponent.destroy()
            settingsObject = null
            settingsPageComponent = null
        }*/
        pageStack.push(historyObject)
        historyObject.init()
        return
    }
    console.log("create history")

    historyPageComponent = Qt.createComponent(Qt.resolvedUrl("HistoryPage.qml"))
    historyObject = historyPageComponent.createObject(window)
    historyObject.init()/*
    if(obsPageComponent) {
        obsObject.destroy()
        obsPageComponent.destroy()
        obsObject = null
        obsPageComponent = null
    }
    if(settingsPageComponent) {
        settingsObject.destroy()
        settingsPageComponent.destroy()
        settingsObject = null
        settingsPageComponent = null
    }*/
    pageStack.push(historyObject)
}

function showMapPage(itemi, mode, place_x, place_y, bird_x, bird_y)
{
    currentStatusBox = itemi
    if (mapObject)
    {
        console.log("map exists, push")/*
        if(obsPageComponent) {
            obsObject.destroy()
            obsPageComponent.destroy()
            obsObject = null
            obsPageComponent = null
        }
        if(historyPageComponent) {
            historyObject.destroy()
            historyPageComponent.destroy()
            historyObject = null
            historyPageComponent = null
        }*/
    } else {
        mapPageComponent = Qt.createComponent(Qt.resolvedUrl("MapPage.qml"))
        mapObject = mapPageComponent.createObject(window)
    }
    mapObject.setType(mode)
    if (mode == "place") {
        mapObject.setBirdPos(null, null)
        mapObject.setPlacePos(place_x, place_y)
        if (place_x != null && place_y != null) {
            mapObject.setPos(place_x, place_y)
        }
    } else {
        mapObject.setPlacePos(place_x, place_y)
        mapObject.setBirdPos(bird_x, bird_y)
        if (bird_x != null && bird_y != null) {
            mapObject.setPos(bird_x, bird_y)
        }
        else if (place_x != null && place_y != null) {
            mapObject.setPos(place_x, place_y)
        }
    }
    mapObject.startGps()

    /*
    if(obsPageComponent) {
        obsObject.destroy()
        obsPageComponent.destroy()
        obsObject = null
        obsPageComponent = null
    }
    if(historyPageComponent) {
        historyObject.destroy()
        historyPageComponent.destroy()
        historyObject = null
        historyPageComponent = null
    }*/

    pageStack.push(mapObject)
}

function fillRegPersonBox()
{
    var selectedNames = new Array();
    var j = 0;
    for(var i = 0; i < personModel.rowCount(); i++)
    {
        if (personModel.data(i, 2) == true)//&& personModel.data(i, 36) == true)
        {
            selectedNames[j] = personModel.data(i, 35)
            j++;
        }
    }

    var names = "";
    for(var i = 0; i < selectedNames.length; i++)
    {
        if (names != "")
        {
            names += ","
        }
        names += selectedNames[i]
    }

    obsObject.regpeopleChanged(names)
}

function fillNonRegPersonBox()
{
    var selectedNames = new Array();
    var j = 0;
    for(var i = 0; i < personModel.rowCount(); i++)
    {
        if (personModel.data(i, 2) == true && personModel.data(i, 36) == false)
        {
            selectedNames[j] = personModel.data(i, 35)
            j++;
        }
    }

    var names = "";
    for(var i = 0; i < selectedNames.length; i++)
    {
        if (names != "")
        {
            names += ","
        }
        names += selectedNames[i]
    }

    obsObject.peopleChanged(names)
}

function fillStatusBox()
{
    var selectedStatuses = new Array();
    var j = 0;
    for(var i = 0; i < statusModel.rowCount(); i++)
    {
        if (statusModel.data(i, 2) == true)
        {
            selectedStatuses[j] = statusModel.data(i, 35)
            j++;
        }
    }
    var names = "";
    for(var k = 0; k < selectedStatuses.length; k++)
    {
        if (names != "")
        {
            names += ","
        }
        names += selectedStatuses[k]
    }

    currentStatusBox.text = names
}

function fillAssociationBox()
{
    var selectedItems = new Array();
    var j = 0;
    for(var i = 0; i < associationModel.rowCount(); i++)
    {
        if (associationModel.data(i, 2) == true)
        {
            selectedItems[j] = associationModel.data(i, 35)
            j++;
        }
    }
    var names = "";
    for(var k = 0; k < selectedItems.length; k++)
    {
        if (names != "")
        {
            names += "#"
        }
        names += selectedItems[k]
    }

    //currentStatusBox.text = names
    if (names.length > 0) {
        console.log("onlyDefaultAssiciation: " + true)
        window.onlyDefaultAssiciation = true
        window.saveOnlyDefaultAssociation(true)
    } else {
        console.log("onlyDefaultAssiciation: " + false)
        window.onlyDefaultAssiciation = false
        window.saveOnlyDefaultAssociation(false)
    }
    console.log("defaultAssociation: " + defaultAssociation)
    window.saveDefaultAssociation(names)
    window.defaultAssociation = names
}

function fillSexBox(index)
{
    if (currentLanguage == "en") {
        currentStatusBox.text = sexModel.data(index, 37)
    }
    else if (currentLanguage == "sv") {
        currentStatusBox.text = sexModel.data(index, 36)
    }
    else {
        currentStatusBox.text = sexModel.data(index, 35)
    }
}

function fillDressBox(index)
{
    if (currentLanguage == "en") {
        currentStatusBox.text = dressModel.data(index, 35)
    }
    else if (currentLanguage == "sv") {
        currentStatusBox.text = dressModel.data(index, 35)
    }
    else {
        currentStatusBox.text = dressModel.data(index, 35)
    }
}

function fillDirectionBox(index)
{
    if (currentLanguage == "en") {
        currentStatusBox.text = directionModel.data(index, 35)
    }
    else if (currentLanguage == "sv") {
        currentStatusBox.text = directionModel.data(index, 35)
    }
    else {
        currentStatusBox.text = directionModel.data(index, 35)
    }
}

function fillLocationAccuracyBox(index)
{
    console.log('fillLocationAccuracyBox')
    if (currentLanguage == "en") {
        currentStatusBox.text = locationAccuracyModel.data(index, 36)
    }
    else if (currentLanguage == "sv") {
        currentStatusBox.text = locationAccuracyModel.data(index, 36)
    }
    else {
        currentStatusBox.text = locationAccuracyModel.data(index, 36)
    }
}

function fillBirdAccuracyBox(index)
{
    console.log('fillBirdAccuracyBox')
    if (currentLanguage == "en") {
        currentStatusBox.text = birdAccuracyModel.data(index, 36)
    }
    else if (currentLanguage == "sv") {
        currentStatusBox.text = birdAccuracyModel.data(index, 36)
    }
    else {
        currentStatusBox.text = birdAccuracyModel.data(index, 36)
    }
}

function fillAgeBox(index)
{
    if (currentLanguage == "en") {
        currentStatusBox.text = ageModel.data(index, 35)
    }
    else if (currentLanguage == "sv") {
        currentStatusBox.text = ageModel.data(index, 35)
    }
    else {
        currentStatusBox.text = ageModel.data(index, 35)
    }
}

function fillAtlasBox(index)
{
    if (currentLanguage == "en") {
        currentStatusBox.text = atlasModel.data(index, 35)
    }
    else if (currentLanguage == "sv") {
        currentStatusBox.text = atlasModel.data(index, 35)
    }
    else {
        currentStatusBox.text = atlasModel.data(index, 35)
    }
}

function dataEdited()
{
    return obsObject.dataChanged()
}

function dataSaved()
{
    console.log("dataSaved")
    return obsObject.initDataChanged()
}

function unSavedDataExists()
{
    return dataEdited()
}

function readAndSaveData()
{
    var allData = "";
    var delimiter = "#";
    allData = obsObject.readAllData()
    if (allData == "-1")
    {
        console.log("Pakollinen tieto puuttuu")
        obsObject.showErrorDialog()
        unsavedData = true
        return false
    }

    if (allData != "")
    {
        console.log("really allData: " + allData)
        window.writeNew(allData)
        dataSaved()
        unsavedData = false
        return true
    }
}

function delegateDataChanged()
{
    for(var i = 0; i < sprite.length; i++)
    {
        if (sprite[i].edited == true)
        {
            return true
        }
    }
    return false
}

function initDelegateDataChanged()
{
    for(var i = 0; i < sprite.length; i++)
    {
        sprite[i].edited = false
    }
}


function readDelegateDatas()
{
    var allData = "";
    var delimiter = "#";

    allData += sprite.length + delimiter
    for(var i = 0; i < sprite.length; i++)
    {
        var delegateData = "";
        delegateData += sprite[i].delegateData()
        if (delegateData != "-1")
        {
            delegateData += delimiter
            allData += delegateData
        }
        else
        {
            allData = "-1"
            break;
        }
    }
    return allData
}

function setDelegateData(row, data)
{
    sprite[row].setDelegateData(data)
}

function changeDelegateHeight()
{
    for(var i = 0; i < sprite.length; i++)
    {
        sprite[i].y = i * delegateHeight
    }
}

function clearObsDataSelections()
{
    personModel.filter("")
    for(var i = 0; i < personModel.rowCount(); i++)
    {
        personModel.setData(i, false, 2)
    }
    statusModel.filter("")
    for(var i = 0; i < statusModel.rowCount(); i++)
    {
        statusModel.setData(i, false, 2)
    }
}

function checkAndSetDefaultSaver(index)
{
    for(var i = 0; i < personModel.rowCount(); i++)
    {
        if (i == index)
        {
            console.log("person match - default")
            personModel.setData(i, true, 40)
        }
        else
        {
            console.log("person match - nondefault")
            personModel.setData(i, false, 40)
        }
    }
}

function addPeople()
{
    var editorCompnent = Qt.createComponent(Qt.resolvedUrl("PersonEditPage.qml"))
    var editorObject = editorCompnent.createObject(window)
    editorObject.addNew()
    pageStack.push(editorObject)
}

function addLocation()
{
    var editorCompnent = Qt.createComponent(Qt.resolvedUrl("LocationEditPage.qml"))
    var editorObject = editorCompnent.createObject(window)
    editorObject.addNew()
    pageStack.push(editorObject)
}

function addBird()
{
    var editorCompnent = Qt.createComponent(Qt.resolvedUrl("BirdEditPage.qml"))
    var editorObject = editorCompnent.createObject(window)
    editorObject.addNew()
    pageStack.push(editorObject)
}

function addStatus()
{
    var editorCompnent = Qt.createComponent(Qt.resolvedUrl("StatusEditPage.qml"))
    var editorObject = editorCompnent.createObject(window)
    editorObject.addNew()
    pageStack.push(editorObject)
}

function clearListPage() {
    if( listObject ) {
        listObject.selectModel()
        listObject.selectDelegate()
    }
}

function mapPan(deltaX,deltaY) {
    if (mapObject) {
        mapObject.pan(deltaX,deltaY);
    }
}

function fillBirdCoords(coords) {
    currentStatusBox.text = coords
}

function fillLocationCoords(coords) {
    currentStatusBox.text = coords
}

function editPeople(indexi)
{
    var editorCompnent = Qt.createComponent(Qt.resolvedUrl("PersonEditPage.qml"))
    var editorObject = editorCompnent.createObject(window)
    pageStack.push(editorObject)
    editorObject.selectedIndex(indexi)
}

function editBird(indexi)
{
    var editorCompnent = Qt.createComponent(Qt.resolvedUrl("BirdEditPage.qml"))
    var editorObject = editorCompnent.createObject(window)
    pageStack.push(editorObject)
    editorObject.selectedIndex(indexi)
}

function editStatus(indexi)
{
    var editorCompnent = Qt.createComponent(Qt.resolvedUrl("StatusEditPage.qml"))
    var editorObject = editorCompnent.createObject(window)
    pageStack.push(editorObject)
    editorObject.selectedIndex(indexi)
}

function editLocation(indexi)
{
    var editorCompnent = Qt.createComponent(Qt.resolvedUrl("LocationEditPage.qml"))
    var editorObject = editorCompnent.createObject(window)
    pageStack.push(editorObject)
    editorObject.selectedIndex(indexi)
}

