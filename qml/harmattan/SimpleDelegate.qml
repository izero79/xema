import QtQuick 1.1
import com.nokia.meego 1.1

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
                    if (value == "") {
                        return engname
                    }
                    else if (engname == "") {
                        return value
                    }
                    else {
                        return value + " (" + engname + ")"
                    }
                }
                else if (currentLanguage == "sv") {
                    if (value == "") {
                        return swename
                    }
                    else if (swename == "") {
                        return value
                    }
                    else {
                        return value + " (" + swename + ")"
                    }
                }
                else {
                    if (value == "") {
                        return finname
                    }
                    else if (finname == "") {
                        return value
                    }
                    else {
                        return value + " (" + finname + ")"
                    }
                }

            }
            color: "white"
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            onClicked: {
                console.log("simple click, ri: " + realindex)
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
