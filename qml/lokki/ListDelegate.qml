import QtQuick 1.0
import com.nokia.symbian 1.0

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
            text: name
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("click: " + name)
                listPage.clicked( name )

            }
        }
    }
}
