import QtQuick 1.0
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
                console.log("click: " + finname)
                listPage.clicked( finname )

            }
        }
    }
}
