import QtQuick 1.1
import com.nokia.meego 1.0

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        visible: true /*{
            if (listView.editMode == true)
            {
                return true
            }
/*
            if (listView.showRegistered == registered)
            {
                return true
            }
            else
            {
                return false
            }
        }*/
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
            text: name ? name : ""
            color: "white"
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            onClicked: {
                if (listView.editMode == true)
                {
                    listPage.clicked(realindex)
                    return
                }
                else
                {
                    listView.model.setData(realindex, !selected, 2)
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
                    color: "#222222"
                    visible: true
                }
            }
        ]
    }
}
