import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript
//import My.testi 1.0

PageStackWindow {
    id: window

    property string listPageType: ""
    property bool useSystematicSort: false
    property int defaultDetailLevel: 1
    property int currentDetailLevel: 1
    property bool unsavedData: false

    signal writeNew( string data )
    signal readObs( string id )
    signal deleteObs( string id, string date, string place )
    signal reloadHistory()
    signal saveSystematicSorting( bool systematic )
    signal saveDetailLevel( int level )
    signal quit()
    signal loadHistoryWithDate( string date )
    signal loadHistoryWithDateAndPlace( string date, string place)
    signal exportData( bool onlyNew )
    signal restoreSpecies()
    signal restoreLocations()
    signal restoreObservers()

    function setSystematicSort( use )
    {
        window.useSystematicSort = use
    }

    function setDetailLevel( level )
    {
        window.defaultDetailLevel = level
        window.currentDetailLevel = level
    }

    function showHistoryPage( type )
    {
        MyScript.showHistoryPage( type )
    }

    function showListPage( type, selectedItems, itemi )
    {
        console.log("showlistpage by: " + itemi)
        MyScript.showListPage( type, selectedItems, itemi )
    }

    function showSettingsPage()
    {
        //pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
        MyScript.showSettingsPage()
    }

    function birdChanged( name )
    {
        MyScript.obsObject.birdChanged( name )
    }

    function placeChanged( name )
    {
        MyScript.obsObject.placeChanged( name )
    }

    function editPeople( index )
    {
        MyScript.settingsObject.editPeople( index )
    }

    function editBird( index )
    {
        MyScript.settingsObject.editBird( index )
    }

    function editLocation( index )
    {
        MyScript.settingsObject.editLocation( index )
    }

    function regpeopleChanged( name )
    {
        MyScript.obsObject.regpeopleChanged( name )
    }

    function peopleChanged( name )
    {
        MyScript.obsObject.peopleChanged( name )
    }

    function sexChanged( name )
    {
        MyScript.fillCurrentBox( name )
    }

    function atlasChanged( name )
    {
        MyScript.fillCurrentBox( name )
    }

    function dressChanged( name )
    {
        MyScript.fillCurrentBox( name )
    }

    function ageChanged( name )
    {
        MyScript.fillCurrentBox( name )
    }

    function showObsPage( doNotInit )
    {
        MyScript.showObsPage( doNotInit )
    }

    function dataLoaded( data )
    {
        MyScript.obsObject.dataLoaded( data )
    }

    function backFromList()
    {
        console.log( "tee list jutut")
        if( listPageType == "regpeople" )
        {
            MyScript.fillRegPersonBox()
        }
        else if( listPageType == "people" )
        {
            MyScript.fillNonRegPersonBox()
        }
        else if( listPageType == "status" )
        {
            MyScript.fillStatusBox()
        }
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

    function newObsWithData( date, place, species )
    {

        MyScript.obsObject.setData( date, place, species )
    }
//    initialPage: Qt.resolvedUrl("MainPage.qml")

    ToolBarLayout {
        id: toolBarLayout
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: {
                if( pageStack.currentPage == MyScript.obsObject )
                {
                    console.log( "tee obs jutut")
                    console.log( "DATA EDITED: " + MyScript.dataEdited() )
                    unsavedData = MyScript.unSavedDataExists()
                    MyScript.clearObsDataSelections()
                    window.reloadHistory()
                }
                else if( pageStack.currentPage == MyScript.listObject )
                {
                    backFromList()
                }
                pageStack.depth <= 1 ? quit() : pageStack.pop()
            }
        }
        ToolButton {
            flat: true
            iconSource: "/qml/s3icons/filter.svg"
            visible: pageStack.currentPage == MyScript.obsObject
            onClicked: {
                if( window.currentDetailLevel < 3 )
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
            iconSource: "/qml/s3icons/save.svg"
            visible: pageStack.currentPage == MyScript.obsObject && MyScript.obsObject.currentTab == 3
            onClicked: {
                console.log( "tee save jutut")
                var success = false
                success = MyScript.readAndSaveData()
                if( success )
                {
                    window.reloadHistory()
                    MyScript.obsObject.clearTab()
                    MyScript.dataSaved()
                    console.log( "tee delete jutut")
                    unsavedData = false
                }

            }
        }
        ToolButton {
            flat: true
            iconSource: "toolbar-delete"
            visible: pageStack.currentPage == MyScript.obsObject //&& MyScript.obsObject.currentTab == 3
            onClicked: {
                console.log( "tee delete jutut")
                MyScript.obsObject.clearTab()
                unsavedData = false
            }
        }
    }

    Component.onCompleted: {
        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
    }
}
