import QtQuick 1.1
import com.nokia.meego 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 80
        Rectangle {
            id: background
            anchors.fill: parent
            visible: false
            color: "#222222"
        }

        Label {
            id: delegateTitle
            anchors.right: delegateTitleNumber.left
            anchors.top: parent.top
            anchors.topMargin: 10
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: 5
            verticalAlignment: Text.AlignVCenter
            text: {
                if (dateListVisible == true)
                {
                    date
                }
                else if (placeListVisible == true)
                {
                    place
                }
                else
                {
                    species
                }
            }
            color: "white"
            elide: Text.ElideRight
            font.bold: true
        }
        Label {
            id: delegateTitleNumber
            anchors.right: delegateAdd.left
            anchors.top: parent.top
            anchors.topMargin: 10
            height: delegateTitle.height
            width: 50
            verticalAlignment: Text.AlignVCenter
            text: {
                if (dateListVisible == true)
                {
                    return "(" + dateCount + ")"
                }
                else if (placeListVisible == true)
                {
                    return "(" + placeCount + ")"
                }
                else
                {
                    return ""
                }
            }
            color: "white"
            elide: Text.ElideLeft
            font.bold: false
        }
        Label {
            id: delegateSubTitle
            anchors.right: delegateSubTitleNumber.left
            anchors.top: delegateTitle.bottom
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: 5
            verticalAlignment: Text.AlignVCenter
            text: species
            color: "white"
            elide: Text.ElideRight
            font.pixelSize: delegateTitle.font.pixelSize - 2
            visible: placeListVisible == true || dateListVisible == true
        }
        Label {
            id: delegateSubTitleNumber
            anchors.right: delegateAdd.left
            anchors.top: delegateTitle.bottom
            height: delegateSubTitle.height
            verticalAlignment: Text.AlignVCenter
            width: 50
            text: ""
            color: "white"
            elide: Text.ElideLeft
            font.pixelSize: delegateTitle.font.pixelSize - 2
            visible: placeListVisible == true || dateListVisible == true
        }
        Label {
            id: delegateAdd
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            verticalAlignment: Text.AlignVCenter
            width: 60
            text: "+"
            font.pixelSize: 40
            color: "white"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (dateListVisible == true)
                    {
                        console.log("uusi datella: " + date)
                        historyPage.newObsWithData(date)
                    }
                    else if (placeListVisible == true)
                    {
                        console.log("uusi placella ja datella: " + place + ", " +date)
                        historyPage.newObsWithData(date, place)
                    }
                    else
                    {
                        console.log("uusi place date ja laji:" + place + ", " + date + ", " + species)
                        historyPage.newObsWithData(date, place, species)
                    }
                }
            }
        }

        MouseArea {
            id: ma
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: delegateAdd.left
            onClicked: {
                if (dateListVisible == true)
                {
                    console.log("avaa date: " + date)
                    historyPage.showDate(date)
                }
                else if (placeListVisible == true)
                {
                    console.log("avaa place: " + place + ", with date: " +date)
                    historyPage.showPlace(place, date)
                }
                else
                {
                    console.log("click: " + itemid)
                    historyPage.clicked(itemid)
                }

            }
            onPressAndHold: {
                if (historyListView.model === historyModel)
                {
                    historyPage.showContextMenu(itemid, place, date)
                }
            }
        }
        states: [
            State {
                name: "pressed"
                when: ma.pressed == true
                PropertyChanges {
                    target: background
                    visible: true
                }
            }
        ]
    }
}
