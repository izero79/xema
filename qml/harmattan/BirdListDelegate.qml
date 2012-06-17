import QtQuick 1.1
 import com.nokia.meego 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 60
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
                    return engname + ", " + latinname
                }
                else if (currentLanguage == "sv") {
                    return swename + ", " + latinname
                }
                else {
                    return finname + ", " + latinname
                }
            }

            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("clicked bird: " + finname)
                if (listView.editMode == true)
                {
                    listPage.clicked(realindex)
                }
                else
                {
                    listPage.clicked(realindex)
                }

            }
            onPressAndHold: {
                listPage.showContextMenu(realindex)
            }
        }
    }
}
