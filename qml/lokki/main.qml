import QtQuick 1.0
import com.nokia.symbian 1.0
import "myjs.js" as MyScript

Window {
    id: window

    property string listPageType: ""

    function showHistoryPage( type )
    {
        MyScript.showHistoryPage( type )
    }

    function showListPage( type )
    {
        MyScript.showListPage( type )
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

    function showObsPage()
    {
        MyScript.showObsPage()
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
                iconSource: "toolbar-back"
                onClicked: pageStack.depth <= 1 ? Qt.quit() : pageStack.pop()
            }
            ToolButton {
                property int detailNo: 1
                text: detailNo
                visible: pageStack.currentPage == MyScript.obsObject
                onClicked: {
                    if( detailNo < 3 )
                    {
                        detailNo++
                    }
                    else
                    {
                        detailNo = 1
                    }
                    MyScript.obsObject.detailLevel = detailNo
                }
            }
        }
    }

    Component.onCompleted: {
        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
    }
}
