import QtQuick 1.1
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: visible ? 60 : 0
        Text {
            id: delegateTitle
            anchors.right: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            text: name
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("simple delefate click: " + name)
                listPage.clicked(name)
            }
            onPressAndHold: {
                listPage.showContextMenu()
            }

        }
    }
}
