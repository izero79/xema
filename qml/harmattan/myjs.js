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
        listPageComponent = Qt.createComponent(Qt.resolvedUrl("ListPage.qml"))
        listObject = listPageComponent.createObject(window)
    }
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
    else if (type == "sex" || type == "age" || type == "dress" || type == "atlas")
    {
        currentStatusBox = itemi
    }
    listObject.init()
}

function showObsPage(doNotInit)
{
    if (obsObject)
    {
        console.log("obs exists, push")
        pageStack.push(obsObject)
        if (doNotInit == false)
        {
            obsObject.init()
        }
        return
    }
    obsPageComponent = Qt.createComponent(Qt.resolvedUrl("ObservationPage.qml"))
    obsObject = obsPageComponent.createObject(window)
    pageStack.push(obsObject)
    if (doNotInit == false)
    {
        obsObject.init()
    }
}

function showSettingsPage()
{
    if (settingsObject)
    {
        console.log("settings exists, push")
        pageStack.push(settingsObject)
        return
    }
    settingsPageComponent = Qt.createComponent(Qt.resolvedUrl("SettingsPage.qml"))
    settingsObject = settingsPageComponent.createObject(window)
    pageStack.push(settingsObject)
}

function showHistoryPage(type)
{
    if (historyObject)
    {
        pageStack.push(historyObject)
        historyObject.init()
        return
    }
    historyPageComponent = Qt.createComponent(Qt.resolvedUrl("HistoryPage.qml"))
    historyObject = historyPageComponent.createObject(window)
    historyObject.init()
    pageStack.push(historyObject)
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

function checkAndSetDefaultPerson(index)
{
    for(var i = 0; i < personModel.rowCount(); i++)
    {
        if (i == index)
        {
            console.log("person match - default")
            personModel.setData(i, true, 37)
        }
        else
        {
            console.log("person match - nondefault")
            personModel.setData(i, false, 37)
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
