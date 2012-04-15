import QtQuick 1.1
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        visible: {
            if( listView.editMode == true )
            {
                return true
            }

            if( listView.showRegistered == registered )
            {
                return true
            }
            else
            {
                return false
            }
        }
        height: visible ? 60 : 0
        Rectangle {
            id: background
            anchors.fill: parent
            color: "blue"
            visible: selected == true
        }
        Text {
            id: delegateTitle
            anchors.right: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            text: name ? name : ""
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if( listView.editMode == true )
                {
                    listPage.clicked( realindex )
                    return
                }
                else
                {
                    listView.model.setData( realindex, !selected, 2 )
                }
            }
            onPressAndHold: {
                listPage.showContextMenu(realindex)
            }
        }
    }
}
