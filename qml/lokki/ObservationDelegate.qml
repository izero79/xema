import QtQuick 1.0
import com.nokia.symbian 1.1

Item {
    id: item1

    function setStatus( joo )
    {
        statusTf.text = joo
    }

    function delegateData()
    {
        var allData = "";
        var delimiter = "#";
        allData += countTf.text + delimiter
        allData += startTimeTf.text + delimiter
        allData += endTimeTf.text + delimiter
        allData += sexTf.text + delimiter
        allData += dressTf.text + delimiter
        allData += ageTf.text + delimiter
        allData += statusTf.text + delimiter
        allData += moreInfoTa.text + delimiter
        allData += loftTf.text + delimiter
        allData += watchChkBox.checked + delimiter
        allData += nestChkBox.checked
        console.log( "delegate allData: " + allData)
        return allData;
    }

    function setDelegateData( data )
    {
        console.log( "set delegate data: " + data)
        var fields;
        fields = data.split( "#" );
        countTf.text = fields[0]
        startTimeTf.text = fields[1]
        endTimeTf.text = fields[2]
        sexTf.text = fields[3]
        dressTf.text = fields[4]
        ageTf.text = fields[5]
        statusTf.text = fields[6]
        moreInfoTa.text = fields[7]
        loftTf.text = fields[8]
        watchChkBox.checked = fields[9]
        nestChkBox.checked = fields[10]
    }

    anchors.right: parent.right
    anchors.rightMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0
    height: 200
    width: parent.width
    TextField {
        id: countTf
        height: 50
        text: "lukum"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: 75
    }
    TextField {
        id: sexTf
        height: 50
        text: "sukup"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: countTf.right
        anchors.leftMargin: 0
        width: 75
        visible: detailLevel > 1
        MouseArea {
            id: sexTfmouse
            anchors.fill: parent
            onClicked: window.showListPage( "sex", sexTf.text, sexTf );
        }
    }
    TextField {
        id: ageTf
        height: 50
        text: "ikä"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: sexTf.right
        anchors.leftMargin: 0
        width: 75
        visible: detailLevel > 1
        MouseArea {
            id: ageTfmouse
            anchors.fill: parent
            onClicked: window.showListPage( "age", ageTf.text, ageTf );
        }
    }
    TextField {
        id: dressTf
        height: 50
        text: "puku"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: ageTf.right
        anchors.leftMargin: 0
        width: 75
        visible: detailLevel > 1
        MouseArea {
            id: dressTfmouse
            anchors.fill: parent
            onClicked: window.showListPage( "dress", dressTf.text, dressTf );
        }
    }
    TextField {
        id: statusTf
        height: 50
        text: "tila"
        anchors.top: countTf.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: 150
        MouseArea {
            id: tf5mouse
            anchors.fill: parent
            onClicked: window.showListPage( "status", statusTf.text, statusTf );
        }
    }
    TextField {
        id: startTimeTf
        width: 50
        height: 50
        text: "klo"
        anchors.left: statusTf.right
        anchors.leftMargin: 0
        y: statusTf.y
        visible: detailLevel > 2
    }

    Text {
        id: text1
        height: 50
        color: "#ffffff"
        text: qsTr("-")
        anchors.left: startTimeTf.right
        anchors.leftMargin: 0
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
        y: statusTf.y
        width: 20
        visible: detailLevel > 2
    }

    TextField {
        id: endTimeTf
        width: 50
        height: 50
        text: "klo"
        anchors.left: text1.right
        anchors.leftMargin: 0
        y: statusTf.y
        visible: detailLevel > 2
    }
    TextField {
        id: loftTf
        height: 50
        text: "parvi"
        anchors.left: endTimeTf.right
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        y: statusTf.y
        visible: detailLevel > 2
    }
    CheckBox {
        id: watchChkBox
        anchors.top: statusTf.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: parent.width / 2
        text: qsTr( "Bongaus")
        visible: detailLevel > 2
    }

    CheckBox {
        id: nestChkBox
        anchors.left: watchChkBox.right
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        text: qsTr( "Pesintä")
        y: watchChkBox.y
        visible: detailLevel > 2
    }
    TextArea {
        id: moreInfoTa
        anchors.top: watchChkBox.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        text: qsTr( "Lisätietoja")
        height: 50
        visible: detailLevel > 2
    }

}
