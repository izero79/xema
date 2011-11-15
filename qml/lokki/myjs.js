var component;
var sprite = new Array() ;
var compCount = 0;
var listPageComponent;
var listObject;
var obsPageComponent;
var obsObject;
var historyPageComponent;
var historyObject;
var currentStatusBox;

function createObjects() {
    component = Qt.createComponent("ObservationDelegate.qml");
    sprite[compCount] = component.createObject(item7, {"x": 0, "y": compCount*200});
    compCount++;
    obsCount = compCount

    if (sprite == null) {
        // Error Handling
        console.log("Error creating object");
    }
}

function removeObject() {
    if( compCount > 1 )
    {
        compCount--;
        sprite[compCount].destroy()
        obsCount = compCount
    }
}

function showListPage( type, selectedItems, itemi )
{
    listPageComponent = Qt.createComponent(Qt.resolvedUrl("ListPage.qml"))
    listObject = listPageComponent.createObject( window )
    listPageType = type
    pageStack.push(listObject)
    if( type == "regpeople" || type == "people")
    {
        listObject.selectNames( selectedItems )
    }
    else if( type == "status")
    {
        currentStatusBox = itemi
        listObject.selectStatus( selectedItems )
    }
    else if( type == "sex" || type == "age" || type == "dress")
    {
        currentStatusBox = itemi
    }
}

function showObsPage()
{
    obsPageComponent = Qt.createComponent(Qt.resolvedUrl("ObservationPage.qml"))
    obsObject = obsPageComponent.createObject( window )
    pageStack.push(obsObject)
}

function showHistoryPage( type )
{
    historyPageComponent = Qt.createComponent(Qt.resolvedUrl("HistoryPage.qml"))
    historyObject = historyPageComponent.createObject( window )
    pageStack.push(historyObject)
}

function fillRegPersonBox()
{
    var selectedNames = new Array();
    var j = 0;
    for( var i = 0; i < personModel.rowCount(); i++ )
    {
        if( personModel.data( i, 2 ) == true && personModel.data( i, 36 ) == true )
        {
            selectedNames[j] = personModel.data( i, 35 )
            j++;
        }
    }
    console.log( "valutut reg nimet: " + selectedNames)

    var names = "";
    for( var i = 0; i < selectedNames.length; i++ )
    {
        if( names != "" )
        {
            names += ","
        }
        names += selectedNames[i]
    }

    obsObject.regpeopleChanged( names )
}

function fillNonRegPersonBox()
{
    var selectedNames = new Array();
    var j = 0;
    for( var i = 0; i < personModel.rowCount(); i++ )
    {
        if( personModel.data( i, 2 ) == true && personModel.data( i, 36 ) == false )
        {
            selectedNames[j] = personModel.data( i, 35 )
            j++;
        }
    }
    console.log( "valutut nonreg nimet: " + selectedNames)
    var names = "";
    for( var i = 0; i < selectedNames.length; i++ )
    {
        if( names != "" )
        {
            names += ","
        }
        names += selectedNames[i]
    }

    obsObject.peopleChanged( names )
}

function fillStatusBox()
{
    var selectedStatuses = new Array();
    var j = 0;
    for( var i = 0; i < statusModel.rowCount(); i++ )
    {
        if( statusModel.data( i, 2 ) == true )
        {
            selectedStatuses[j] = statusModel.data( i, 35 )
            j++;
        }
    }
    console.log( "valutut tilat: " + selectedStatuses)
    var names = "";
    for( var i = 0; i < selectedStatuses.length; i++ )
    {
        if( names != "" )
        {
            names += ","
        }
        names += selectedStatuses[i]
    }

    console.log( "current on: " + currentStatusBox.text )
    currentStatusBox.text = names
}

function fillCurrentBox( name )
{
    currentStatusBox.text = name
}

function readAllData()
{
    var allData = "";
    var delimiter = "#";
    allData = obsObject.readAllData()
    console.log( "really allData: " + allData)
    window.writeNew( allData )
}

function readDelegateDatas()
{
    var allData = "";
    var delimiter = "#";
    console.log( "sprite pituus: " + sprite.length)
    allData += sprite.length + delimiter
    for( var i = 0; i < sprite.length; i++ )
    {
        allData += sprite[i].delegateData() + delimiter
    }
    return allData
}

function setDelegateData( row, data )
{
    sprite[row].setDelegateData( data )
}
