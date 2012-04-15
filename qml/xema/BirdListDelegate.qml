import QtQuick 1.1
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 60
        Text {
            id: delegateTitle
            anchors.right: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            text: finname + ", " + latinname
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("clicked bird: " + finname)
                if( listView.editMode == true )
                {
                    listPage.clicked( realindex )
                }
                else
                {
                    listPage.clicked( finname )
                }

            }
            onPressAndHold: {
                listPage.showContextMenu(realindex)
            }
        }
    }
}
