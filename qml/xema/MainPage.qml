import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: mainPage
    tools: toolBarLayout

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
            text: qsTr( "Continue" )
            anchors.top: parent.top
            anchors.topMargin: 0
            checked: false
            checkable: false
            pressed: false
            anchors.horizontalCenter: parent.horizontalCenter
            visible: unsavedData
            onClicked: {
                window.showObsPage( true )
            }
        }

        Button {
            id: newButton
            height: parent.height / 6
            width: parent.width
            text: qsTr( "New observation" )
            anchors.top: continueButton.bottom
            anchors.topMargin: 20
            checked: false
            checkable: false
            pressed: false
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                window.showObsPage( false )
            }
        }

        Button {
            id: settingsButton
            text: qsTr( "Settings" )
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
            text: qsTr( "Observations" )
            anchors.top: newButton.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                window.showHistoryPage()
            }
        }    
    }
}
