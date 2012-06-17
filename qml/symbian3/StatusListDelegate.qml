import QtQuick 1.1
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 60
        Rectangle {
            id: background
            anchors.fill: parent
            color: "blue"
            visible: selected == true
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
                    return abbrev + " (" + engname + ")"
                }
                else if (currentLanguage == "sv") {
                    return abbrev + " (" + swename + ")"
                }
                else {
                    return abbrev + " (" + finname + ")"
                }

            }
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {

                if (listView.editMode == true)
                {
                    listPage.clicked(realindex)
                }
                else
                {
                    console.log("click: " + abbrev + " (" + finname + ")")
                    //listPage.clicked(abbrev + " (" + name + ")")
                    listView.model.setData(realindex, !selected, 2)
                }
            }
            onPressAndHold: {
                listPage.showContextMenu(realindex)
            }
        }
    }
}
