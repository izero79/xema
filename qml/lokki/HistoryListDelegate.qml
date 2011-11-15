import QtQuick 1.0
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 60
        Text {
            id: delegateTitle
            anchors.right: delegateAdd.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            text: name
            color: "white"
        }
        Text {
            id: delegateAdd
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            verticalAlignment: Text.AlignVCenter
            width: 60
            text: "+"
            font.pixelSize: 40
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("click: " + obsId)
                historyPage.clicked( obsId )

            }
        }
    }
}
