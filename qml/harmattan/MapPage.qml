import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.location 1.2

Page {
    id: mapPage
    tools: mapToolBarLayout

    property string mode: "place"
    property double place_x: -1
    property double place_y: -1
    property double bird_x: -1
    property double bird_y: -1
    property double pos_x: -1
    property double pos_y: -1

    ToolBarLayout {
        id: mapToolBarLayout
        visible: true
        ToolIcon {
            //flat: true
            visible: true
            platformIconId: "toolbar-back"
            onClicked: {
                mapItem.item.stopGps()
                if (mapItem.item.mode == "place" && mapItem.item.place_x != -1 && mapItem.item.place_y != -1) {
                    var pcoords = mapItem.item.place_x.toFixed(4) + ":" + mapItem.item.place_y.toFixed(4)
                    window.setPlaceCoords(pcoords)
                } else if (mapItem.item.mode == "bird" && mapItem.item.bird_x != -1 && mapItem.item.bird_y != -1) {
                    var bcoords = mapItem.item.bird_x.toFixed(4) + ":" + mapItem.item.bird_y.toFixed(4)
                    window.setBirdCoords(bcoords)
                }
                pageStack.pop()
            }
        }
    }

    function pan(deltaX,deltaY) {
        mapItem.item.pan(deltaX,deltaY);
    }

    function setType(type) {
        if (mapItem.item) {
            mapItem.item.setType(type)
        } else {
            mapPage.mode = type
        }
    }

    function setPos(x, y) {
        if (mapItem.item) {
            mapItem.item.setPos(x, y)
        } else if (x != null && y != null){
            mapPage.pos_x = x
            mapPage.pos_y = y
        }
    }

    function setBirdPos(x, y) {
        if (mapItem.item) {
            mapItem.item.setBirdPos(x, y)
        } else if (x != null && y != null){
            mapPage.bird_x = x
            mapPage.bird_y = y
        }
    }

    function setPlacePos(x, y) {
        if (mapItem.item) {
            mapItem.item.setPlacePos(x, y)
        } else if (x != null && y != null){
            mapPage.place_x = x
            mapPage.place_y = y
        }
    }

    function startGps() {
        if (mapItem.item) {
            mapItem.item.startGps()
        }
    }

    Rectangle {
        id: mapBase
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width
        color: "#343434"
    }

    Loader {
        id : mapItem
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width
        z: mapBase.z + 1
        source: ""
        onSourceChanged: {
            if (source != "") {
                mapItem.item.setType(mapPage.mode)
                if (mapPage.pos_x != -1 && mapPage.pos_y != -1) {
                    mapItem.item.setPos(pos_x, pos_y)
                }
                if (mapPage.place_x != -1 && mapPage.place_y != -1) {
                    mapItem.item.setPlacePos(place_x, place_y)
                }
                if (mapPage.bird_x != -1 && mapPage.bird_y != -1) {
                    mapItem.item.setBirdPos(bird_x, bird_y)
                }
                mapItem.item.startGps()
            }
        }
    }

    Connections {
        target: mapItem.item
    }

    Component.onCompleted:
        mapLoaderTimer.start()

    Timer {
        id: mapLoaderTimer
        repeat: false
        interval: 300
        onTriggered: {
            mapItem.source = Qt.resolvedUrl( "MapItem.qml" )
        }
    }
}
