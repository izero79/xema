import QtQuick 1.0
import com.nokia.symbian 1.1
import "myjs.js" as MyScript

Item {

    property int detailLevel: 1

    height: parent.height
    width: parent.width

    Component.onCompleted: {
        MyScript.createObjects();
    }

    property int obsCount: 0

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

    Dialog {
        id: listDialog

        title: Text {
            id: titleField
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: "GPS"
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
                text: "Hakee GPS-koodrinaatit"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }

        buttons: Item { height: button1.height + 2 * 10;
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: dialogButton1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                text: "Ok";
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
        TabButton { tab: tab2content; text: "Päivä" }
        TabButton { tab: tab1content; text: "Paikka" }
        TabButton { tab: tab3content; text: "Havainto" }
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

                Item {
                    id: item1
                    width: 360
                    height: 143
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.top: parent.top
                    anchors.topMargin: 8

                    TextField {
                        id: locationTf
                        height: 50
                        text: "havaintopaikka"
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: button1.left
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        MouseArea {
                            id: tf1mouse
                            anchors.fill: parent
                            onClicked: window.showListPage( "places" );
                        }
                    }

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



                    /*
                    TextField {
                        id: textfield2
                        height: 50
                        text: "linnun paikka"
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

                    TextField {
                        id: startTimeTf
                        width: 160
                        height: 50
                        text: "alkuaika"
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
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
                        text: "loppuaika"
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 0
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
                contentHeight: item3.y + item3.height

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
                        id: textfield3
                        width: 160
                        height: 50
                        text: "alkupäivä"
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                    }

                    TextField {
                        id: textfield4
                        width: 160
                        height: 50
                        text: "loppupäivä"
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 0
                    }

                    Text {
                        id: text3
                        height: 50
                        color: "#ffffff"
                        text: qsTr("-")
                        anchors.right: textfield4.left
                        anchors.rightMargin: 0
                        anchors.left: textfield3.right
                        anchors.leftMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        font.pixelSize: 18
                    }
                }
/*
                Text {
                    id: text4
                    color: "#ffffff"
                    text: qsTr("Havainnointiaika")
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
                    id: item3
                    height: 50
                    anchors.top: text4.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    TextField {
                        id: textfield5
                        width: 160
                        height: 50
                        text: "alkuaika"
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                    }

                    TextField {
                        id: textfield6
                        width: 160
                        height: 50
                        text: "loppuaika"
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: parent.top
                        anchors.topMargin: 0
                    }

                    Text {
                        id: text5
                        height: 50
                        color: "#ffffff"
                        text: qsTr("-")
                        anchors.right: textfield6.left
                        anchors.rightMargin: 0
                        anchors.left: textfield5.right
                        anchors.leftMargin: 0
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        font.pixelSize: 18
                    }
                }
                */
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
                        text: "Rekisteröityneet"
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        MouseArea {
                            id: ta1mouse
                            anchors.fill: parent
                            onClicked: window.showListPage( "regpeople" );
                        }

                    }

                    TextArea {
                        id: otherPeopleTa
                        height: 100
                        text: "Muut"
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.top: regPeopleTa.bottom
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        MouseArea {
                            id: ta2mouse
                            anchors.fill: parent
                            onClicked: window.showListPage( "people" );
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
                    anchors.topMargin: 8
                    font.pixelSize: 18
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

                    TextArea {
                        id: weatherTa
                        height: 100
                        text: "Sää"
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
                contentHeight: item8.y + item8.height


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
                    height: 100
                    anchors.top: text7.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    TextField {
                        id: birdNameTf
                        height: 50
                        text: "Laji"
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
                    CheckBox {
                        id: radiobutton1
                        anchors.top: birdNameTf.bottom
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        text: qsTr( "Salaa havainto")
                        visible: detailLevel > 2
                    }
                }

                Item {
                    id: item6
                    height: childrenRect.height
                    anchors.top: item5.bottom
                    anchors.topMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0

                    Item {
                        id: item7
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        height: childrenRect.height

                    }
                    Button {
                        id: plus
                        text: "+"
                        anchors.top: item7.bottom
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        width: 50
                        onClicked: {
                            MyScript.createObjects();
                        }
                    }
                    Button {
                        id: minus
                        text: "-"
                        anchors.top: item7.bottom
                        anchors.topMargin: 8
                        anchors.left: plus.right
                        anchors.leftMargin: 0
                        width: 50
                        visible: obsCount > 1
                        onClicked: {
                            MyScript.removeObject();
                        }
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
                    anchors.top: item6.bottom
                    anchors.topMargin: 8
                    font.pixelSize: 18
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

                    TextArea {
                        id: textarea3
                        height: 150
                        text: "Lisätietoja"
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                    }

                    TextArea {
                        id: textfield8
                        height: 50
                        text: "Atlas"
                        anchors.top: textarea3.bottom
                        anchors.topMargin: 0
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        width: 100
                        visible: detailLevel > 2
                    }
                }
            }
        }
    }
}
