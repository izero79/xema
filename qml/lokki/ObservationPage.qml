import QtQuick 1.1
import com.nokia.symbian 1.1
import "myjs.js" as MyScript

Page {

    id: obsPage
    tools: toolBarLayout
    property int detailLevel: window.currentDetailLevel
    property int currentId: 0
    property int delegateHeight: detailLevel == 3 ? 300 : detailLevel == 2 ? 150 : 75
    height: parent.height
    width: parent.width

    onDelegateHeightChanged: {
        MyScript.changeDelegateHeight()
    }

    Component.onCompleted: {
        window.currentDetailLevel = window.defaultDetailLevel
        MyScript.createObjects();
    }

    Component.onDestruction: MyScript.removeObjects();

    property int obsCount: 0

    function init()
    {
        console.log("obs init")
        currentId = 0
        birdNameTf.text = ""
        startDateTf.text = ""
        stopDateTf.text = ""
        startTimeTf.text = ""
        endTimeTf.text = ""
        locationTf.text = ""
        moreInfoTa.text = ""
        atlasTf.text = ""
        regPeopleTa.text = ""
        otherPeopleTa.text = ""
        hideChkBox.checked = false

        MyScript.removeObjects();
        MyScript.createObjects();

        weatherTa.text = ""
        window.currentDetailLevel = window.defaultDetailLevel

        MyScript.changeDelegateHeight()

    }

    function birdChanged( name )
    {
        birdNameTf.text = name
    }

    function placeChanged( name )
    {
        locationTf.text = name
    }

    function regpeopleChanged( name )
    {
        regPeopleTa.text = name
    }

    function peopleChanged( name )
    {
        otherPeopleTa.text = name
    }

    function readAllData()
    {
        if( birdNameTf.text == "" || regPeopleTa.text == "" || locationTf.text == "" )
        {
            return ""
        }

        var allData = "";
        var delimiter = "#";
        allData += currentId + delimiter
        allData += birdNameTf.text + delimiter
        allData += startDateTf.text + delimiter
        allData += stopDateTf.text + delimiter
        allData += startTimeTf.text + delimiter
        allData += endTimeTf.text + delimiter
        allData += locationTf.text + delimiter
        allData += moreInfoTa.text + delimiter
        allData += atlasTf.text + delimiter
        allData += regPeopleTa.text + delimiter
        allData += otherPeopleTa.text + delimiter
        allData += hideChkBox.checked + delimiter

        allData += MyScript.readDelegateDatas()

        allData += weatherTa.text

        console.log( "allData: " + allData)
        return allData;
    }

    function dataLoaded( data )
    {
        console.log( "data loaded now: " + data)
        var fields;
        fields = data.split( "#" );
        console.log("id: " +fields[0])
        currentId = fields[0]
        birdNameTf.text = fields[1]
        startDateTf.text = fields[2]
        stopDateTf.text = fields[3]
        startTimeTf.text = fields[4]
        endTimeTf.text = fields[5]
        locationTf.text = fields[6]
        moreInfoTa.text = fields[7]
        atlasTf.text = fields[8]
        regPeopleTa.text = fields[9]
        otherPeopleTa.text = fields[10]
        hideChkBox.checked = fields[11]

        var rows = fields[12]
        console.log("rows: " + rows)
        var currentField = 0;
        while( rows > obsCount )
        {
            MyScript.createObjects();
        }
        for(var j = 0; j < rows; j++ )
        {
            var rowData = ""
            for( var k = 13+j*11; k < 24+j*11; k++ )
            {
                rowData += fields[k] + "#"
                currentField = k
            }

            MyScript.setDelegateData( j, rowData )
        }
        currentField++
        weatherTa.text = fields[currentField]

    }



    Dialog {
        id: listDialog

        title: Text {
            id: titleField
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: qsTr( "GPS" )
            horizontalAlignment: Text.AlignHCenter
        }
        content:Item {
            id: name
            height: 100
            width: parent.width
            anchors.topMargin: 10
            Text {
                id: dialogText
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: qsTr( "Hakee GPS-koodrinaatit" )
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }

        buttons: Item { height: dialogButton1.height + 2 * 10;
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: dialogButton1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                text: qsTr( "Ok" )
                onClicked: {
                    locationTf.text = "63.7998N, 23.0095E"
                    listDialog.close()
                }
            }
        }
    }

    TabBarLayout {
        id: tabBarLayout
        anchors { left: parent.left; right: parent.right; top: parent.top }
        TabButton { tab: tab2content; text: qsTr( "Päivä" ) }
        TabButton { tab: tab1content; text: qsTr( "Paikka" ) }
        TabButton { tab: tab3content; text: qsTr( "Havainto" ) }
    }

    TabGroup {
        id: tabGroup
        currentTab: tab2content
        anchors { left: parent.left; right: parent.right; top: tabBarLayout.bottom; bottom: parent.bottom }

        // define the content for tab 1
        Page {
            id: tab1content
            clip: true
            Flickable {
                id: flickable1
                clip: false
                flickableDirection: Flickable.VerticalFlick
                anchors.fill: parent
                contentWidth: width
                contentHeight: obsTimeItem.y + obsTimeItem.height

                Text {
                    id: locationText
                    color: "#ffffff"
                    text: qsTr("Havainnointipaikka")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    font.pixelSize: 18
                }

                Item {
                    id: item1
                    width: parent.width
                    height: 50
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: locationText.bottom
                    anchors.topMargin: 8

                    TextField {
                        id: locationTf
                        height: 50
                        placeholderText: qsTr( "Havaintopaikka" )
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right //button1.left
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        MouseArea {
                            id: tf1mouse
                            anchors.fill: parent
                            onClicked: window.showListPage( "places" );
                        }
                    }
/*
                    Button {
                        id: button1
                        text: "..."
                        iconSource: ""
                        width: 50
                        height: locationTf.height
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        onClicked: {
                            listDialog.open()
                        }
                    }
*/
                    /*
                    TextField {
                        id: textfield2
                        height: 50
                        text: qsTr( "linnun paikka" )
                        anchors.top: locationTf.bottom
                        anchors.topMargin: 8
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                    }*/
                }
                Text {
                    id: obsTimeText
                    color: "#ffffff"
                    text: qsTr("Havainnointiaika")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: item1.bottom
                    anchors.topMargin: 8
                    font.pixelSize: 18
                    visible: detailLevel > 1

                }

                Item {
                    id: obsTimeItem
                    height: 50
                    anchors.top: obsTimeText.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    visible: detailLevel > 1


                    TextField {
                        id: startTimeTf
                        width: 160
                        height: 50
                        placeholderText: "0:00"
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        validator: RegExpValidator {
                            regExp: /[0-2]{0,1}[0-9]{1}[\:\.]{1}[0-9]{2}/
                        }
                        onActiveFocusChanged: {
                            if( activeFocus == true && text == "" )
                            {
                                text = Qt.formatDateTime(new Date(), "hh:mm")
                            }
                        }
                    }

                    Text {
                        id: timeDashText
                        height: 50
                        color: "#ffffff"
                        text: qsTr("-")
                        anchors.left: startTimeTf.right
                        anchors.leftMargin: 0
                        anchors.right: endTimeTf.left
                        anchors.rightMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        font.pixelSize: 18
                    }

                    TextField {
                        id: endTimeTf
                        width: 160
                        height: 50
                        placeholderText: "0:00"
                        text: ""
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 0
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
                    }
                }
            }
        }

        // define the content for tab 2
        Page {
            id: tab2content
            clip: true
            Flickable {
                id: flickable2
                clip: false
                flickableDirection: Flickable.VerticalFlick
                anchors.fill: parent
                contentWidth: width
                contentHeight: detailLevel > 1 ? item3.y + item3.height : item4.y + item4.height

                Text {
                    id: text2
                    color: "#ffffff"
                    text: qsTr("Päivämäärä")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    font.pixelSize: 18
                }

                Item {
                    id: item2
                    height: 50
                    anchors.top: text2.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    TextField {
                        id: startDateTf
                        width: 160
                        height: 50
                        placeholderText: "0.0.0000"
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        validator: RegExpValidator {
                            regExp: /[0-2]{0,1}[0-9]{1}[\.]{1}[0-2]{0,1}[0-9]{1}[\.]{1}[0-9]{4}/
                        }
                        onActiveFocusChanged: {
                            if( activeFocus == true && text == "" )
                            {
                                text = Qt.formatDateTime(new Date(), "dd.MM.yyyy")
                            }
                        }
                    }

                    Text {
                        id: text3
                        height: 50
                        color: "#ffffff"
                        text: qsTr("-")
                        anchors.right: stopDateTf.left
                        anchors.rightMargin: 0
                        anchors.left: startDateTf.right
                        anchors.leftMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        font.pixelSize: 18
                        visible: detailLevel > 2
                    }

                    TextField {
                        id: stopDateTf
                        width: 160
                        height: 50
                        placeholderText: "0.0.0000"
                        text: ""
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        visible: detailLevel > 2
                        validator: RegExpValidator {
                            regExp: /[0-2]{0,1}[0-9]{1}[\.]{1}[0-2]{0,1}[0-9]{1}[\.]{1}[0-9]{4}/
                        }
                        onActiveFocusChanged: {
                            if( activeFocus == true && text == "" )
                            {
                                if( startDateTf.text != "" )
                                {
                                    text = startDateTf.text
                                }
                                else
                                {
                                    text = Qt.formatDateTime(new Date(), "dd.MM.yyyy")
                                }
                            }
                        }
                    }
                }
                Text {
                    id: text6
                    color: "#ffffff"
                    text: qsTr("Havainnoijat")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: item2.bottom
                    anchors.topMargin: 8
                    font.pixelSize: 18
                }

                Item {
                    id: item4
                    height: 200
                    anchors.top: text6.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    TextArea {
                        id: regPeopleTa
                        height: 100
                        placeholderText: qsTr( "Rekisteröityneet" )
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        MouseArea {
                            id: ta1mouse
                            anchors.fill: parent
                            onClicked: window.showListPage( "regpeople", regPeopleTa.text );
                        }

                    }

                    TextArea {
                        id: otherPeopleTa
                        height: 100
                        placeholderText: qsTr( "Muut" )
                        text: ""
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: regPeopleTa.bottom
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        visible: detailLevel > 2

                        MouseArea {
                            id: ta2mouse
                            anchors.fill: parent
                            onClicked: window.showListPage( "people", otherPeopleTa.text );
                        }

                    }
                }
                Text {
                    id: text5
                    color: "#ffffff"
                    text: qsTr("Säätila")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: item4.bottom
                    anchors.topMargin: 16
                    font.pixelSize: 18
                    visible: detailLevel > 1

                }

                Item {
                    id: item3
                    height: 100
                    anchors.top: text5.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    visible: detailLevel > 1


                    TextArea {
                        id: weatherTa
                        height: 100
                        placeholderText: qsTr( "Sää" )
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                    }
                }
            }
        }

        // define content for tab 3
        Page {
            id: tab3content
            clip: true
            Flickable {
                id: flickable3
                clip: false
                flickableDirection: Flickable.VerticalFlick
                anchors.fill: parent
                contentWidth: width
                contentHeight: detailLevel > 2 ? item8.y + item8.height : plus.y + plus.height


                Text {
                    id: text7
                    color: "#ffffff"
                    text: qsTr("Havainto")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    font.pixelSize: 18
                }

                Item {
                    id: item5
                    height: 50
                    anchors.top: text7.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    TextField {
                        id: birdNameTf
                        height: 50
                        placeholderText: qsTr( "Laji" )
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        MouseArea {
                            id: tf7mouse
                            anchors.fill: parent
                            onClicked: window.showListPage( "birds" );
                        }

                    }
                }

                Item {
                    id: item7
                    anchors.top: item5.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    height: childrenRect.height
                }
                Button {
                    id: plus
                    text: qsTr( "lisää rivi" )
                    anchors.top: item7.bottom
                    anchors.topMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    onClicked: {
                        MyScript.createObjects();
                    }
                }
                Button {
                    id: minus
                    text: qsTr( "poista rivi" )
                    anchors.top: item7.bottom
                    anchors.topMargin: 8
                    anchors.left: plus.right
                    anchors.leftMargin: 0
                    //width: 100
                    visible: obsCount > 1
                    onClicked: {
                        MyScript.removeObject();
                    }
                }

                Text {
                    id: text8
                    color: "#ffffff"
                    text: qsTr("Lisätietoja")
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.top: plus.bottom
                    anchors.topMargin: 8
                    font.pixelSize: 18
                    visible: detailLevel > 2

                }
                Item {
                    id: item8
                    height: 200
                    anchors.top: text8.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    visible: detailLevel > 2


                    TextArea {
                        id: moreInfoTa
                        height: 150
                        placeholderText: qsTr( "Lisätietoja" )
                        text: ""
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        visible: detailLevel > 2

                    }

                    TextField {
                        id: atlasTf
                        height: 50
                        placeholderText: qsTr( "Atlasindeksi" )
                        text: ""
                        anchors.top: moreInfoTa.bottom
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        width: 150
                        visible: detailLevel > 2
                        validator: IntValidator { bottom: 0 }
                        inputMethodHints: Qt.ImhDigitsOnly
                    }
                    CheckBox {
                        id: hideChkBox
                        anchors.top: moreInfoTa.bottom
                        anchors.topMargin: 0
                        anchors.left: atlasTf.right
                        anchors.leftMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        text: qsTr( "Salaa havainto")
                        visible: detailLevel > 2
                    }
                }
            }
        }
    }
}
