import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.location 1.2

Page {
    id: mapPage
    tools: mapToolBarLayout

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
            center: Coordinate {latitude: 53; longitude: 12}
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

              onPressed: {
                  if(mouse.button == Qt.LeftButton){
                      __lastX = mouse.x
                      __lastY = mouse.y
                      window.mousePressedNow();
                  }
              }

              onReleased: {
                  if(mouse.button == Qt.LeftButton){
                       window.mouseReleasedNow();
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
           }

    }
}
