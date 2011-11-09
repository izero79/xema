import QtQuick 1.0
import com.nokia.symbian 1.0

Page {
    id: mainPage

    Item {
        id: item1
        anchors.centerIn: parent

        width: 200
        height: 200

        Button {
            id: settingsButton
            x: 13
            y: -73
            text: "Asetukset"
            width: newButton.width
            height: newButton.height
            anchors.top: historyButton.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            iconSource: ""
            onClicked: {
                window.showSettingsPage()
            }
        }

        Button {
            id: historyButton
            x: 13
            y: -145
            width: newButton.width
            height: newButton.height
            text: "Havainnot"
            anchors.top: newButton.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                window.showHistoryPage()
            }
        }

        Button {
            id: newButton
            x: 13
            width: 196
            height: 42
            text: "Uusi havanto"
            anchors.top: parent.top
            anchors.topMargin: 0
            checked: false
            checkable: false
            pressed: false
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                window.showObsPage()
            }
        }
    }
}
