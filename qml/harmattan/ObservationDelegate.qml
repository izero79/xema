import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Item {
    id: item1

    property bool edited: false
    property int rows: 1

    function setRows(count)
    {
       rows = count
    }

    function setStatus(joo)
    {
        statusTf.text = joo
    }

    function delegateData()
    {
        var allData = "";
        var delimiter = "#";
        if (countTf.text == "")
        {
            return "-1"
        }

        if (findSex(sexTf.text) == "pariutuneet") {
            var count = parseInt(countTf.text);
            if (count % 2) {
                return "-2"
            }
        }

        allData += countTf.text + delimiter
        allData += startTimeTf.text + delimiter
        if (startTimeTf.text != endTimeTf.text) {
            allData += endTimeTf.text + delimiter
        } else {
            allData += delimiter
        }

        allData += findSex(sexTf.text) + delimiter
        allData += dressTf.text + delimiter
        allData += ageTf.text + delimiter
        allData += statusTf.text + delimiter
        allData += moreInfoTa.text + delimiter
        allData += loftTf.text + delimiter
        allData += watchChkBox.checked + delimiter
        allData += nestChkBox.checked
        allData += delimiter// uusi, epäsuora havainto
        console.log("delegate allData: " + allData)
        return allData;
    }

    function setDelegateData(data)
    {
        console.log("set delegate data: " + data)
        var fields;
        fields = data.split("#");
        countTf.text = fields[0]
        startTimeTf.text = fields[1]
        endTimeTf.text = fields[2]
        fillSex(fields[3])
        dressTf.text = fields[4]
        ageTf.text = fields[5]
        statusTf.text = fields[6]
        moreInfoTa.text = fields[7]
        loftTf.text = fields[8]
        watchChkBox.checked = fields[9]
        nestChkBox.checked = fields[10]
    }

    function findSex(name) {
        console.log("findSex(name): " +name)
        for(var i=0;i<sexModel.rowCount();i++) {
            console.log("i: " +sexModel.data(i, 37))
            if (currentLanguage == "en") {
                if(name === sexModel.data(i, 37)) {
                    return sexModel.data(i, 35)
                }
            } else if (currentLanguage == "sv") {
                if(name === sexModel.data(i, 36)) {
                    return sexModel.data(i, 35)
                }
            }
            else {
                return name
            }
        }
        return name
    }

    function fillSex(name) {
        console.log("fillSex(name): " +name)
        for(var i=0;i<sexModel.rowCount();i++) {
            if (name === sexModel.data(i, 35)) {
                if (currentLanguage == "en") {
                    sexTf.text = sexModel.data(i, 37)
                } else if (currentLanguage == "sv") {
                    sexTf.text = sexModel.data(i, 36)
                }
                else {
                    sexTf.text = name
                }
            }

        }
        return name
    }

    function timeRejected(object) {
        object.text = ""
    }

    function timeSelected(object, h, m) {
        console.log('parametrit: ' +h +", "+m)
        h = h + ""
        m = m + ""
        if (h.length < 2) {
            h = "0" + h
        }
        if (m.length < 2) {
            m = "0" + m
        }

        object.text = h + ":" + m
    }

    function setTimeToPicker(text) {
        var fields = text.split(":")
        timePickerDialog.hour = fields[0]
        timePickerDialog.minute = fields[1]
    }

    anchors.right: parent.right
    anchors.rightMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0
    height: delegateHeight
    width: parent.width

    IntValidator {
        id: biggerThanOne
        bottom: 1
        top: item1.rows
    }

    TimePickerDialog{
        id: timePickerDialog

        property variant targetObject: null

        fields: DateTime.Hours | DateTime.Minutes;
        titleText: qsTr("Select Time")
        acceptButtonText: qsTr("Ok")
        rejectButtonText: qsTr("Cancel")
        onAccepted: {timeSelected(targetObject, timePickerDialog.hour, timePickerDialog.minute)}
        onRejected: {timeRejected(targetObject)}
    }


    Label {
        id: rowLabel
        property int headerHeight: 0
        color: "#ffffff"
        text: qsTr("Row")
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.topMargin: 8
        font.pixelSize: 18
    }

    TextField {
        id: countTf
        property int headerHeight: -y
        height: 50
        placeholderText: qsTr("Individuals *")
        text: ""
        anchors.top: rowLabel.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: 200
        validator: IntValidator { bottom: 1; }
        inputMethodHints: Qt.ImhDigitsOnly
        onTextChanged: item1.edited = true
    }
    TextField {
        id: statusTf
        height: 50
        placeholderText: qsTr("Status")
        text: ""
        anchors.left: countTf.right
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: rowLabel.bottom
        anchors.topMargin: 8
        MouseArea {
            id: tf5mouse
            anchors.fill: parent
            onClicked: window.showListPage("status", statusTf.text, statusTf);
            z: statusTf.z + 1
        }
        onTextChanged: item1.edited = true

    }
    TextField {
        id: sexTf
        height: 50
        placeholderText: qsTr("Sex")
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
            onClicked: window.showListPage("sex", sexTf.text, sexTf);
            z: sexTf.z + 1
        }
        onTextChanged: item1.edited = true
    }
    TextField {
        id: ageTf
        height: 50
        placeholderText: qsTr("Age")
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
            onClicked: window.showListPage("age", ageTf.text, ageTf);
            z: ageTf.z + 1
        }
        onTextChanged: item1.edited = true

    }
    TextField {
        id: dressTf
        height: 50
        placeholderText: qsTr("Dress")
        text: ""
        y: sexTf.y
        anchors.right: parent.right
        anchors.rightMargin: 0
        width: 150
        visible: detailLevel > 1
        MouseArea {
            id: dressTfmouse
            anchors.fill: parent
            onClicked: window.showListPage("dress", dressTf.text, dressTf);
            z: dressTf.z + 1
        }
        onTextChanged: item1.edited = true

    }
    TextField {
        id: startTimeTf
        property int headerHeight: -y// 0 //-item2.y + 16

        width: 100
        height: 50
        placeholderText: qsTr("0:00")
        text: ""
        inputMethodHints: Qt.ImhPreferNumbers

        anchors.top: sexTf.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        visible: detailLevel > 2
        onTextChanged: item1.edited = true
        MouseArea {
            id: startTimeMouse
            anchors.fill: parent
            z: startTimeTf.z + 1
            onClicked: {
                if (startTimeTf.text == "")
                {
                    startTimeTf.text = Qt.formatDateTime(new Date(), "hh:mm")
                }
                setTimeToPicker(startTimeTf.text);
                timePickerDialog.targetObject = startTimeTf
                timePickerDialog.open()
            }
        }
    }

    Label {
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
        property int headerHeight: -y // -item2.y + 16


        width: 100
        height: 50
        placeholderText: qsTr("0:00")
        text: ""
        inputMethodHints: Qt.ImhPreferNumbers

        anchors.right: loftTf.left
        anchors.rightMargin: 0
        y: startTimeTf.y
        visible: detailLevel > 2
        MouseArea {
            id: endTimeMouse
            anchors.fill: parent
            z: endTimeTf.z + 1
            onClicked: {
                if (endTimeTf.text == "")
                {
                    if (startTimeTf.text != "") {
                        endTimeTf.text = startTimeTf.text
                    } else {
                        endTimeTf.text = Qt.formatDateTime(new Date(), "hh:mm")
                    }
                }
                setTimeToPicker(endTimeTf.text);
                timePickerDialog.targetObject = endTimeTf
                timePickerDialog.open()
            }
        }
        onTextChanged: item1.edited = true

    }
    TextField {
        id: loftTf
        property int headerHeight: -y // -item2.y + 16


        height: 50
        width: 100
        placeholderText: qsTr("Loft")
        text: ""
        anchors.right: parent.right
        anchors.rightMargin: 0
        y: startTimeTf.y
        visible: detailLevel > 2
        validator: text != "" ? biggerThanOne : null
        inputMethodHints: Qt.ImhDigitsOnly
        onTextChanged: item1.edited = true

    }

    CheckBox {
        id: watchChkBox
        anchors.top: startTimeTf.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: parent.width / 2
        text: qsTr("Bird-watch")
        visible: detailLevel > 2
        onCheckedChanged: item1.edited = true

    }

    CheckBox {
        id: nestChkBox
        anchors.left: watchChkBox.right
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        text: qsTr("Nesting")
        y: watchChkBox.y
        visible: detailLevel > 2
        onCheckedChanged: item1.edited = true
    }

    TextArea {
        id: moreInfoTa
        property int headerHeight: -y//itemi.height * 2 + 16
        anchors.top: watchChkBox.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        height: 50

        placeholderText: qsTr("More information")
        text: ""
        visible: detailLevel > 2
        onTextChanged: item1.edited = true

    }
}
