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
            text: town + ", " + place
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("click: " + town)
                if( listView.editMode == true )
                {
                    console.log("edit")
                    listPage.clicked( realindex )
                }
                else
                {
                    listPage.clicked( town + ", " + place )
                }
            }
        }
    }
}
