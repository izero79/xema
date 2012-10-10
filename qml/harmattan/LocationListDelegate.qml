import QtQuick 1.1
import com.nokia.meego 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 60
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
            text: localizedTown + ", " + localizedPlace
            /*
            {
                if (currentLanguage == "en") {
                    return engtown + ", " + engplace
                }
                if (currentLanguage == "sv") {
                    return swetown + ", " + sweplace
                }
                else {
                    return town + ", " + place
                }

            }*/
            color: "white"
            font.italic: custom ? true : false
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            onClicked: {
                console.log("click: " + town)
                if (listView.editMode == true)
                {
                    console.log("edit")
                    listPage.clicked(realindex)
                }
                else
                {
                    listPage.clicked(realindex)
//                    listPage.clicked(town + ", " + place)
                }
            }
            onPressAndHold: {
                listPage.showContextMenu(realindex)
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