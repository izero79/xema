import QtQuick 1.0
import com.nokia.symbian 1.1
import "myjs.js" as MyScript

Window {
    id: window

    property string listPageType: ""
    property bool useSystematicSort: false
    property int defaultDetailLevel: 1
    property int currentDetailLevel: 1

    signal writeNew( string data )
    signal readObs( string id )

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
        pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
    }

    function birdChanged( name )
    {
        MyScript.obsObject.birdChanged( name )
    }

    function placeChanged( name )
    {
        MyScript.obsObject.placeChanged( name )
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

    function dressChanged( name )
    {
        MyScript.fillCurrentBox( name )
    }

    function ageChanged( name )
    {
        MyScript.fillCurrentBox( name )
    }

    function showObsPage()
    {
        MyScript.showObsPage()
    }

    function dataLoaded( data )
    {
        MyScript.obsObject.dataLoaded( data )
    }

    StatusBar {
        id: statusBar
        anchors.top: window.top
    }

    PageStack {
        id: pageStack
        anchors { left: parent.left; right: parent.right; top: statusBar.bottom; bottom: toolBar.top }
    }

    ToolBar {
        id: toolBar
        anchors.bottom: window.bottom
        tools: ToolBarLayout {
            id: toolBarLayout
            ToolButton {
                flat: true
                iconSource: "/qml/s3icons/back.svg"
                onClicked: {
                    if( pageStack.currentPage == MyScript.obsObject )
                    {
                        console.log( "tee obs jutut")
                        MyScript.readAllData()
                    }
                    else if( pageStack.currentPage == MyScript.listObject )
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
                        else if( listPageType == "sex" )
                        {
                            MyScript.fillSexBox()
                        }
                    }


                    pageStack.depth <= 1 ? Qt.quit() : pageStack.pop()
                }
            }
            ToolButton {
                text: window.currentDetailLevel
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
        }
    }

    Component.onCompleted: {
        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
    }
}
