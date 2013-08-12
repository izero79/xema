import QtQuick 1.1
import com.nokia.meego 1.0

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: visible ? 60 : 0
        Rectangle {
            id: background
            anchors.fill: parent
            visible: false
            color: "#222222"
        }
        Label {
            id: delegateTitle
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 5
            verticalAlignment: Text.AlignVCenter
            text: {
                if (currentLanguage == "en") {
                    return engname
                }
                else if (currentLanguage == "sv") {
                    return swename
                }
                else {
                    return finname
                }

            }
            color: "white"
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            onClicked: {
                console.log("accuracy click, ri: " + realindex)
                listPage.clicked(realindex)
            }
            onPressAndHold: {
                listPage.showContextMenu()
            }

        }
        states: [
            State {
                name: "pressed"
                when: ma.pressed == true
                PropertyChanges {
                    target: background
                    visible: true
                }
            }
        ]
    }
}
