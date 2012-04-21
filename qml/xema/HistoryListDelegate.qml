import QtQuick 1.1
import com.nokia.symbian 1.1

Component {
    Item {
        id: myTestDelegate
        width: parent.width
        height: 60
        Text {
            id: delegateTitle
            anchors.right: delegateAdd.left
            anchors.top: parent.top
            height: parent.height / 2
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            text: {
                if (dateListVisible == true)
                {
                    date + " (" + dateCount + ")"
                }
                else if (placeListVisible == true)
                {
                    place + " (" + placeCount + ")"
                }
                else
                {
                    species
                }
            }
            color: "white"
            elide: Text.ElideRight
        }
        Text {
            id: delegateSubTitle
            anchors.right: delegateAdd.left
            anchors.top: delegateTitle.bottom
            height: parent.height / 2
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            text: species
            color: "white"
            elide: Text.ElideRight
            font.pixelSize: delegateTitle.font.pixelSize - 2
            visible: placeListVisible == true || dateListVisible == true
        }
        Text {
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
    }
}
