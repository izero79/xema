import QtQuick 1.1
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 60
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
            text: abbrev + " (" + name + ")"
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("click: " + abbrev + " (" + name + ")")
                //listPage.clicked(abbrev + " (" + name + ")")
                listView.model.setData(realindex, !selected, 2)

            }
        }
    }
}
