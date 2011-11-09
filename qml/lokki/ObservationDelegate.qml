import QtQuick 1.0
import com.nokia.symbian 1.0

Item {
    id: item1
    anchors.right: parent.right
    anchors.rightMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0
    height: 200
    width: parent.width
    TextField {
        id: textfield1
        height: 50
        text: "lukum"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: 75

    }
    TextField {
        id: textfield2
        height: 50
        text: "sukup"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: textfield1.right
        anchors.leftMargin: 0
        width: 75

    }
    TextField {
        id: textfield3
        height: 50
        text: "ikä"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: textfield2.right
        anchors.leftMargin: 0
        width: 75
        visible: detailLevel > 2
    }
    TextField {
        id: textfield4
        height: 50
        text: "puku"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: textfield3.right
        anchors.leftMargin: 0
        width: 75
        visible: detailLevel > 2
    }
    TextField {
        id: textfield5
        height: 50
        text: "tila"
        anchors.top: textfield1.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: 150
    }
    TextField {
        id: textfield6
        width: 50
        height: 50
        text: "klo"
        anchors.left: textfield5.right
        anchors.leftMargin: 0
        y: textfield5.y
        visible: detailLevel > 2
    }

    Text {
        id: text1
        height: 50
        color: "#ffffff"
        text: qsTr("-")
        anchors.left: textfield6.right
        anchors.leftMargin: 0
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
        y: textfield5.y
        width: 20
        visible: detailLevel > 2
    }

    TextField {
        id: textfield7
        width: 50
        height: 50
        text: "klo"
        anchors.left: text1.right
        anchors.leftMargin: 0
        y: textfield5.y
        visible: detailLevel > 2
    }
    TextField {
        id: textfield8
        height: 50
        text: "parvi"
        anchors.left: textfield7.right
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        y: textfield5.y
        visible: detailLevel > 2
    }
    CheckBox {
        id: checkBox1
        anchors.top: textfield5.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: parent.width / 2
        text: qsTr( "Bongaus")
        visible: detailLevel > 1
    }

    CheckBox {
        id: checkBox2
        anchors.left: checkBox1.right
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        text: qsTr( "Pesintä")
        y: checkBox1.y
        visible: detailLevel > 1
    }
    TextArea {
        id: textarea1
        anchors.top: checkBox1.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        text: qsTr( "Lisätietoja")
        height: 50
    }

}
