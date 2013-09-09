import QtQuick 1.1
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: visible ? 60 : 0
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
            text: abbrev + ", " + name
            color: "white"
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            onClicked: {
                console.log("accuracy click, ri: " + realindex)
                if (listView.editMode == true)
                {
                    listPage.clicked(realindex)
                }
                else
                {
                    //listPage.clicked(abbrev + " (" + name + ")")
                    listView.model.setData(realindex, !selected, 2)
                }
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
