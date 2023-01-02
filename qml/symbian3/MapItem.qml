import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.location 1.2

Rectangle {
    id: mapArea

    property string mode: "place"
    property double place_x: -1
    property double place_y: -1
    property double bird_x: -1
    property double bird_y: -1
    property int bigButtonSize: 75
    property int mediumButtonSize: 50
    property int bigButtonFontSize: 60
    property int mediumButtonFontSize: 40

    function pan(deltaX,deltaY) {
        map.pan(deltaX,deltaY);
    }

    function setType(type) {
        mapArea.mode = type
    }

    function setPos(x, y) {
        console.log('setPos: ' + x + ':' + y)
        mapPos.latitude = x
        mapPos.longitude = y
        map.center = mapPos
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

    function startGps() {
        gpsSource.start()
    }

    function stopGps() {
        gpsSource.stop()
    }

    Coordinate {
        id: pos
        latitude: 63.8392;
        longitude: 23.1337;
    }

    Coordinate {
        id: mapPos
        latitude: 63.8392;
        longitude: 23.1337;
    }

    MapGroup {
        id: bird
        MapImage {
            source: "../common_icons/xema.png"
            coordinate: Coordinate {latitude: bird_x; longitude: bird_y}
            offset.x: -40
            offset.y: -120
        }
    }
    MapGroup {
        id: place
        MapImage {
            source: "../common_icons/havainnoija.png"
            coordinate: Coordinate {latitude: place_x; longitude: place_y}
            offset.x: -40
            offset.y: -120
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
                parameters: [
                    PluginParameter{ name:  "mapping.app_id" ; value: "app_id"},
                    PluginParameter{ name:  "mapping.token" ; value: "token"}
                ]
            }
            anchors.fill: parent
            size.width: parent.width
            size.height: parent.height
            zoomLevel: 10
            center: mapPos

            MapGroup {
                id: myPosition
                MapImage {
                    id: myPositionImage
                    source: gpsSource.coordinatesValid ? "../common_icons/validposition.png" : "../common_icons/invalidposition.png"
                    coordinate: pos
                    offset.x: -15
                    offset.y: -15
                    smooth: true
                    z: myPositionArea.z + 1
                }

                MapCircle {
                    id: myPositionArea
                    center: myPositionImage.coordinate
                    smooth: true
                    color: "#60d6e6c6"
                    border.color: "#BAadcb94"
                    border.width: 4
                    radius: gpsSource.accuracy > 100 ? -1.0 : gpsSource.accuracy
                    visible: gpsSource.accuracy <= 100 ? true : false
                }
            }

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
                          if (mapArea.mode == "place") {
                              console.log('add place')
                              map.removeMapObject(place)
                              place_x = map.toCoordinate(Qt.point(mouse.x, mouse.y)).latitude
                              place_y = map.toCoordinate(Qt.point(mouse.x, mouse.y)).longitude
                              map.addMapObject(place)
                          }
                          if (mapArea.mode == "bird") {
                              console.log('add bird')
                              map.removeMapObject(bird)
                              bird_x = map.toCoordinate(Qt.point(mouse.x, mouse.y)).latitude
                              bird_y = map.toCoordinate(Qt.point(mouse.x, mouse.y)).longitude
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
                  map.center = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                  map.zoomLevel += 1
              }
           }

        Rectangle {
            id: gotoPos
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 8
            width: bigButtonSize
            height: bigButtonSize
            color: "#80ffffff"
            opacity: 1.0
            Image {
                id: gotoPosLabel
                anchors.centerIn: parent
                source: gpsSource.coordinatesValid ? "../common_icons/validposition.png" : "../common_icons/invalidposition.png"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mapPos.latitude = pos.latitude
                    mapPos.longitude = pos.longitude
                }
            }
        }


        Rectangle {
            id: zoomPlus
            anchors.right: parent.right
            anchors.bottom: zoomMinus.top
            anchors.margins: 8
            width: bigButtonSize
            height: bigButtonSize
            color: "#ffffff"
            opacity: 0.5
            Label {
                id: zoomPlusLabel
                anchors.fill: parent
                text: "+"
                color: "#000000"
                font.pixelSize: bigButtonFontSize
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
            width: bigButtonSize
            height: bigButtonSize
            color: "#ffffff"
            opacity: 0.5
            Label {
                id: zoomMinusLabel
                anchors.fill: parent
                text: "-"
                color: "#000000"
                font.pixelSize: bigButtonFontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                MouseArea {
                    anchors.fill: parent
                    onClicked: map.zoomLevel -= 1
                }
            }
        }


        Rectangle {
            id: mapStreet
            anchors.left: parent.left
            anchors.bottom: mapSatellite.top
            anchors.margins: 8
            width: mediumButtonSize
            height: mediumButtonSize
            color: "#ffffff"
            opacity: 0.5
            Label {
                id: mapStreetLabel
                anchors.fill: parent
                text: "M"
                color: "#000000"
                font.pixelSize: mediumButtonFontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                MouseArea {
                    anchors.fill: parent
                    onClicked: map.mapType = Map.StreetMap
                }
            }
        }

        Rectangle {
            id: mapSatellite
            anchors.left: parent.left
            anchors.bottom: mapTerrain.top
            anchors.margins: 8
            width: mediumButtonSize
            height: mediumButtonSize
            color: "#ffffff"
            opacity: 0.5
            Label {
                id: mapSatelliteLabel
                anchors.fill: parent
                text: "S"
                color: "#000000"
                font.pixelSize: mediumButtonFontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                MouseArea {
                    anchors.fill: parent
                    onClicked: map.mapType = Map.SatelliteMapDay
                }
            }
        }

        Rectangle {
            id: mapTerrain
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 8
            width: mediumButtonSize
            height: mediumButtonSize
            color: "#ffffff"
            opacity: 0.5
            Label {
                id: mapTerrainLabel
                anchors.fill: parent
                text: "T"
                color: "#000000"
                font.pixelSize: mediumButtonFontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                MouseArea {
                    anchors.fill: parent
                    onClicked: map.mapType = Map.TerrainMap
                }
            }
        }

    }
    PositionSource {
        id: gpsSource
        active: true
        property double accuracy: gpsSource.position.horizontalAccuracy > gpsSource.position.verticatalAccuracy ? gpsSource.position.horizontalAccuracy : gpsSource.position.verticalAccuracy
        property bool coordinatesValid: (gpsSource.position.latitudeValid && gpsSource.position.longitudeValid &&
                                        gpsSource.position.coordinate.latitude != "NaN" && gpsSource.position.coordinate.longitude != "NaN")
        updateInterval: 2500

        onActiveChanged: {
            console.log("Active Changed called")
        }

        onPositionChanged: {
            console.log(gpsSource.position.coordinate.latitude + ", " + gpsSource.position.coordinate.longitude + ", acc: " + gpsSource.accuracy)
            if (coordinatesValid) {
                pos.latitude = gpsSource.position.coordinate.latitude
                pos.longitude = gpsSource.position.coordinate.longitude
            }
        }
    }
}
