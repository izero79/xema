import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.location 1.2

Page {
    id: mapPage
    tools: mapToolBarLayout

    property string mode: "place"
    property double place_x: 64
    property double place_y: 24
    property double bird_x: 64
    property double bird_y: 24

    ToolBarLayout {
        id: mapToolBarLayout
        visible: true
        ToolIcon {
            //flat: true
            visible: true
            iconId: "icon-m-toolbar-back-white"
            onClicked: {
                pageStack.pop()
            }
        }
    }

    function pan(deltaX,deltaY) {
        map.pan(deltaX,deltaY);
    }

    function setType(type) {
        mapPage.mode = type
        if (type == "place") {

        }
        if (type == "bird") {

        }
    }

    function setPos(x, y) {
        console.log('setPos: ' + x + ':' + y)
        pos.latitude = x
        pos.longitude = y
        map.center = pos
    }

    function setBirdPos(x, y) {
        console.log('setBirde: ' + x + ':' + y)
        map.removeMapObject(bird)
        if (x != null && y != null) {
            bird_x = x
            bird_y = y
            map.addMapObject(bird)
        }
    }

    function setPlacePos(x, y) {
        console.log('setPlace: ' + x + ':' + y)
        map.removeMapObject(place)
        if (x != null && y != null) {
            place_x = x
            place_y = y
            map.addMapObject(place)
        }
    }

    Coordinate {
        id: pos
        latitude: 64;
        longitude: 24;
    }

    MapGroup {
        id: bird
        MapImage {
            source: "../common_icons/bird.png"
            coordinate: Coordinate {latitude: bird_x; longitude: bird_y}
            offset.x: -64
            offset.y: -64
        }
    }
    MapGroup {
        id: place
        MapImage {
            source: "../common_icons/place.png"
            coordinate: Coordinate {latitude: place_x; longitude: place_y}
            offset.x: -31
            offset.y: -64
        }
    }

    Rectangle {
        id: dataArea
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width
        color: "#343434"

        Map {
            id: map
            plugin : Plugin {
                name : "nokia"
            }
            anchors.fill: parent
            size.width: parent.width
            size.height: parent.height
            zoomLevel: 6
            center: pos

        } // map

        PinchArea {
              id: pincharea

              property double __oldZoom

              anchors.fill: parent

              function calcZoomDelta(zoom, percent) {
                 return zoom + Math.log(percent)/Math.log(2)
              }

              onPinchStarted: {
                 __oldZoom = map.zoomLevel
              }

              onPinchUpdated: {
                 map.zoomLevel = calcZoomDelta(__oldZoom, pinch.scale)
              }

              onPinchFinished: {
                 map.zoomLevel = calcZoomDelta(__oldZoom, pinch.scale)
              }
           }

        MouseArea {
              id: mousearea
              anchors.fill : parent
              property int __lastX: -1
              property int __lastY: -1
              property double __lastMapX: -1
              property double __lastMapY: -1

              onPressed: {
                  if(mouse.button == Qt.LeftButton){
                      __lastX = mouse.x
                      __lastY = mouse.y
                      __lastMapX = map.center.latitude
                      __lastMapY = map.center.longitude

                      window.mousePressedNow();
                  }
              }

              onReleased: {
                  if(mouse.button == Qt.LeftButton){
                      window.mouseReleasedNow();
                      var dx = __lastMapX - map.center.latitude;
                      var dy = __lastMapY -  map.center.longitude;
                      console.log('dx: ' +dx)
                      console.log('dy: ' +dy)
                      if (dx == 0 && dy == 0) {
                          if (mapPage.mode == "place") {
                              console.log('add place')
                              map.removeMapObject(place)
                              place_x = map.center.latitude
                              place_y = map.center.longitude
                              map.addMapObject(place)
                          }
                          if (mapPage.mode == "bird") {
                              console.log('add bird')
                              map.removeMapObject(bird)
                              bird_x = map.center.latitude
                              bird_y = map.center.longitude
                              map.addMapObject(bird)
                          }
                      }
                  }
              }

              onPositionChanged: {
                    var dx = __lastX - mouse.x;
                    var dy = __lastY - mouse.y;
                    window.mouseMovedNow(dx, dy);
                    __lastX = mouse.x
                    __lastY = mouse.y
              }

              onCanceled: {
                  if(mouse.button == Qt.LeftButton){
                    window.mouseReleasedNow();
                  }
              }
              onDoubleClicked: {
                  map.zoomLevel += 1
              }
           }

        Rectangle {
            id: zoomPlus
            anchors.right: parent.right
            anchors.bottom: zoomMinus.top
            anchors.margins: 8
            width: 75
            height: 75
            color: "#ffffff"
            opacity: 0.5
            Label {
                id: zoomPlusLabel
                anchors.fill: parent
                text: "+"
                color: "#000000"
                font.pixelSize: 80
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                MouseArea {
                    anchors.fill: parent
                    onClicked: map.zoomLevel += 1
                }
            }
        }

        Rectangle {
            id: zoomMinus
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 8
            width: 75
            height: 75
            color: "#ffffff"
            opacity: 0.5
            Label {
                id: zoomMinusLabel
                anchors.fill: parent
                text: "-"
                color: "#000000"
                font.pixelSize: 80
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                MouseArea {
                    anchors.fill: parent
                    onClicked: map.zoomLevel -= 1
                }
            }
        }

    }
}
