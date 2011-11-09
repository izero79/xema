var component;
var sprite = new Array() ;
var compCount = 0;
var listPageComponent;
var listObject;
var obsPageComponent;
var obsObject;
var historyPageComponent;
var historyObject;

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

function showListPage( type )
{
    listPageComponent = Qt.createComponent(Qt.resolvedUrl("ListPage.qml"))
    listObject = listPageComponent.createObject( window )
    listPageType = type
    pageStack.push(listObject)
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
