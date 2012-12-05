import QtQuick 1.1
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 80
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
            anchors.left: parent.left
            anchors.leftMargin: 5
            verticalAlignment: Text.AlignVCenter
            height: 30
            text: localizedPlace
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
        Label {
            id: delegateSubTitle
            anchors.right: parent.right
            anchors.top: delegateTitle.bottom
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: 5
            verticalAlignment: Text.AlignVCenter
            text: localizedTown + ", " + localizedCountry
            color: "white"
            elide: Text.ElideRight
            font.pixelSize: delegateTitle.font.pixelSize - 2
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
