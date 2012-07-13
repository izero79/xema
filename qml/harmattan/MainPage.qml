import QtQuick 1.1
 import com.nokia.meego 1.1

Page {
    id: mainPage
    tools: mainToolBarLayout

    ToolBarLayout {
        id: mainToolBarLayout
        visible: true
        ToolIcon {
            //flat: true
            visible: false
            iconId: "icon-m-toolbar-back-white"
            onClicked: {
                pageStack.depth <= 1 ? quit() : pageStack.pop()
            }
        }
        ToolButton {
            flat: true
            text: "i"
            visible: true
            onClicked: {
                aboutDialog.open()
            }
            anchors.centerIn: parent
        }
    }


    Loader {
        id: aboutDialog

        function open()
        {
            source = Qt.resolvedUrl( "AboutDialog.qml" )
            if( item != null )
            {
                item.screenX = -x
                item.screenY = -y
                item.open()
            }
        }

        function close()
        {
            if( item != null )
            {
                item.close()
            }
            source = ""
        }

        function isVisible()
        {
            if( item != null )
            {
                return item.isVisible
            }
            return false
        }

        anchors.centerIn: parent
        width: window.inPortrait ? parent.width / 3 * 2: parent.width / 5 * 2
        height: window.inPortrait ? parent.height / 6 * 2 : parent.height / 5 * 3
        source: ""
        z: 100
        onYChanged: {
            if( item != null )
            {
                item.screenX = -x
                item.screenY = -y
            }
        }
    }
    Connections {
        target: aboutDialog.item
        onButton1Clicked: {
            aboutDialog.close()
        }
        onCanceled: {
            aboutDialog.close()
        }
        onOpenHomepage: {
            window.openUrl( "http://www.iki.fi/z7/xema" )
        }
    }

    Item {
        id: item1
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.verticalCenter: parent.verticalCenter
        anchors.centerIn: parent
        height: 280

        width: 200
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom

        Button {
            id: continueButton
            width: parent.width
            height: newButton.height
            text: qsTr("Continue")
            anchors.top: parent.top
            anchors.topMargin: 0
            checked: false
            checkable: false
            anchors.horizontalCenter: parent.horizontalCenter
            visible: unsavedData
            onClicked: {
                window.showObsPage(true)
            }
        }

        Button {
            id: newButton
            height: parent.height / 6
            width: parent.width
            text: qsTr("New observation")
            anchors.top: continueButton.bottom
            anchors.topMargin: 20
            checked: false
            checkable: false
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                window.showObsPage(false)
            }
        }

        Button {
            id: settingsButton
            text: qsTr("Settings")
            width: parent.width
            height: newButton.height
            anchors.top: historyButton.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            iconSource: ""
            onClicked: {
                window.showSettingsPage()
            }
        }

        Button {
            id: historyButton
            width: parent.width
            height: newButton.height
            text: qsTr("Observations")
            anchors.top: newButton.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                window.showHistoryPage()
            }
        }    
    }
}
