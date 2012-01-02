import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: item1

    property bool edited: false
    property int rows: 1

    function setRows( count )
    {
       rows = count
    }

    function setStatus( joo )
    {
        statusTf.text = joo
    }

    function delegateData()
    {
        var allData = "";
        var delimiter = "#";
        if( countTf.text == "" )
        {
            return "-1"
        }

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
    height: delegateHeight
    width: parent.width
    TextField {
        id: countTf
        height: 50
        placeholderText: qsTr( "lukumäärä" )
        text: ""
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: 75
        validator: IntValidator { bottom: 1; }
        inputMethodHints: Qt.ImhDigitsOnly
        onTextChanged: item1.edited = true

    }
    TextField {
        id: sexTf
        height: 50
        placeholderText: qsTr( "sukupuoli" )
        text: ""
        anchors.top: countTf.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: 125
        visible: detailLevel > 1
        MouseArea {
            id: sexTfmouse
            anchors.fill: parent
            onClicked: window.showListPage( "sex", sexTf.text, sexTf );
        }
        onTextChanged: item1.edited = true
    }
    TextField {
        id: ageTf
        height: 50
        placeholderText: qsTr( "ikä" )
        text: ""
        y: sexTf.y
        anchors.left: sexTf.right
        anchors.leftMargin: 0
        anchors.right: dressTf.left
        anchors.rightMargin: 0
        width: 100
        visible: detailLevel > 1
        MouseArea {
            id: ageTfmouse
            anchors.fill: parent
            onClicked: window.showListPage( "age", ageTf.text, ageTf );
        }
        onTextChanged: item1.edited = true

    }
    TextField {
        id: dressTf
        height: 50
        placeholderText: qsTr( "puku" )
        text: ""
        y: sexTf.y
        anchors.right: parent.right
        anchors.rightMargin: 0
        width: 100
        visible: detailLevel > 1
        MouseArea {
            id: dressTfmouse
            anchors.fill: parent
            onClicked: window.showListPage( "dress", dressTf.text, dressTf );
        }
        onTextChanged: item1.edited = true

    }
    TextField {
        id: statusTf
        height: 50
        placeholderText: qsTr( "tila" )
        text: ""
        anchors.left: countTf.right
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 8
        MouseArea {
            id: tf5mouse
            anchors.fill: parent
            onClicked: window.showListPage( "status", statusTf.text, statusTf );
        }
        onTextChanged: item1.edited = true

    }
    Item {
        id: item2
        height: 50
        anchors.top: sexTf.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        visible: detailLevel > 2

        TextField {
            id: startTimeTf
            width: 100
            height: 50
            placeholderText: qsTr( "0:00" )
            text: ""
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 0
            visible: detailLevel > 2
            validator: RegExpValidator {
                regExp: /[0-2]{0,1}[0-9]{1}[\:\.]{1}[0-9]{2}/
            }
            onActiveFocusChanged: {
                if( activeFocus == true && text == "" )
                {
                    text = Qt.formatDateTime(new Date(), "hh:mm")
                }
            }
            onTextChanged: item1.edited = true

        }

        Text {
            id: text1
            height: 50
            color: "#ffffff"
            text: qsTr("-")
            anchors.left: startTimeTf.right
            anchors.leftMargin: 0
            anchors.right: endTimeTf.left
            anchors.rightMargin: 0
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
            y: startTimeTf.y
            visible: detailLevel > 2
        }

        TextField {
            id: endTimeTf
            width: 100
            height: 50
            placeholderText: qsTr( "0:00" )
            text: ""
            anchors.right: loftTf.left
            anchors.rightMargin: 0
            y: startTimeTf.y
            visible: detailLevel > 2
            validator: RegExpValidator {
                regExp: /[0-2]{0,1}[0-9]{1}[\:\.]{1}[0-9]{2}/
            }
            onActiveFocusChanged: {
                if( activeFocus == true && text == "" )
                {
                    if( startTimeTf.text != "" )
                    {
                        text = startTimeTf.text
                    }
                    else
                    {
                        text = Qt.formatDateTime(new Date(), "hh:mm")
                    }
                }
            }
            onTextChanged: item1.edited = true

        }
        TextField {
            id: loftTf
            height: 50
            width: 100
            placeholderText: qsTr( "parvi" )
            text: ""
            anchors.right: parent.right
            anchors.rightMargin: 0
            y: statusTf.y
            visible: detailLevel > 2
            validator: IntValidator {
                bottom: 1
                top: item1.rows
            }
            inputMethodHints: Qt.ImhDigitsOnly
            onTextChanged: item1.edited = true

        }
    }
    CheckBox {
        id: watchChkBox
        anchors.top: item2.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: parent.width / 2
        text: qsTr( "Bongaus")
        visible: detailLevel > 2
        onCheckedChanged: item1.edited = true

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
        onCheckedChanged: item1.edited = true
    }
    TextArea {
        id: moreInfoTa
        anchors.top: watchChkBox.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        placeholderText: qsTr( "Lisätietoja" )
        text: ""
        height: 50
        visible: detailLevel > 2
        onTextChanged: item1.edited = true

    }

}
