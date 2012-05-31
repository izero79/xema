import QtQuick 1.1
 import com.nokia.meego 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: visible ? 60 : 0
        Label {
            id: delegateTitle
            anchors.right: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
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
            anchors.fill: parent
            onClicked: {
                // tstest listPage.clicked(name)
                listPage.clicked(index)
            }
            onPressAndHold: {
                listPage.showContextMenu()
            }

        }
    }
}
