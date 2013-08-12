import QtQuick 1.1
import com.nokia.symbian 1.1

FocusScope {
    id: buttonDialog

    property int screenY: -y
    property int screenX: -x
    property int screenWidth: window.width ? window.height : window.width
    property int screenHeight: window.width ? window.height : window.width

    property string text: appName + "\n" + qsTr( "version " ) + versionString + "\nTero Siironen" + "\nizero79@gmail.com"
    property string button1text: qsTr( "Ok" )
    property int marginSize: 10
    property int textFontSize: 20
    property int buttonFontSize: 30
    property int buttonHeight: 50
    property int buttonWidth: 100
    property bool portrait: window.inPortrait
    property string iconFileName: "/qml/symbian3_icons/xema96.png"
    property int iconSize: 96
    property bool useHighlight: false
    property string hiliteColor: "#0000ff"
    property bool keysUsed: false
    property bool isVisible: state != "invisible"

    signal button1Clicked()
    signal canceled()
    signal openHomepage()

    Keys.onContext2Pressed:
    {
        canceled()
    }

    Keys.onPressed: {
        if( event.key == Qt.Key_F2 )
        {
            canceled()
        }
        event.accepted = true
    }

    function open()
    {
        buttonDialog.state = "visible"
        buttonDialog.focus = true
    }

    function close()
    {
        buttonDialog.state = "invisible"
    }

    state: "invisible"
    width: 100
    height: 100

    focus: true

    Rectangle{
        id: backgroundFade
        y: screenY
        x: screenX
        width: screenWidth
        height: screenHeight *2
        z: 1
        visible: backgroundMouseArea.enabled
        color: "#c8000000"
        MouseArea{
            id: backgroundMouseArea
            anchors.fill: parent
            z: 2
            enabled: buttonDialog.state == "visible"
            onClicked: {
                buttonDialog.canceled()
            }
        }
    }

    Rectangle {
        id: buttonDialogBase
        color: "#c8000000"
        radius: 7
        border.width: 4
        border.color: "#c8ffffff"
        anchors.fill: parent
        focus: true
        z: backgroundFade.z + 2

        Image {
            id: icon
            visible: iconFileName != ""
            source: iconFileName
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 20
            //y: ( text1.height - text1.y ) / 2 - ( height / 2 )
            height: iconSize
            width: iconSize
        }

        Label {
            id: text1
            anchors.top: parent.top
            anchors.left: icon.right
            anchors.right: parent.right
            anchors.margins: marginSize
            height: buttonDialog.textFontSize * 4 + 2 * marginSize
            text:  buttonDialog.text
            smooth: true
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            color: "#ffffff"
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: buttonDialog.textFontSize
        }
        Label {
            id: urlText
            z: backgroundMA.z + 1
            anchors.top: text1.height >= icon.height ? text1.bottom : icon.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: marginSize
            text:  "http://www.iki.fi/z7/xema"
            smooth: true
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            color: buttonDialog.hiliteColor
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: buttonDialog.textFontSize
            MouseArea {
                id: urlMouse
                anchors.fill: parent
                onClicked: {
                    buttonDialog.openHomepage()
                }
            }
        }

        Button {
            id: button1
            z: urlMouse.z + 1

            width: {
                if( buttonWidth == 0 )
                {
                    parent.width - ( 2 * marginSize )
                }
                else
                {
                    buttonWidth
                }
            }
            height: buttonHeight
            text: button1text
            //fontSize: buttonFontSize
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: marginSize
            //hiliteColor: buttonDialog.hiliteColor
            //useHighlight: buttonDialog.useHighlight || keysUsed
            focus: true
            onClicked: {
                buttonDialog.button1Clicked()
            }
            Keys.onReturnPressed: clicked()
            Keys.onEnterPressed: clicked()
            Keys.onSelectPressed: clicked()
            Keys.onPressed: {
                keysUsed = true
            }
        }
        MouseArea {
            id: backgroundMA
            anchors.fill: parent
        }
    }
    states: [ State {
        name: "visible";
        PropertyChanges { target: buttonDialogBase; opacity: 1 }
        PropertyChanges { target: buttonDialog; visible: true }
    },
    State {
            name: "invisible";
            PropertyChanges { target: buttonDialogBase; opacity: 0 }
            PropertyChanges { target: buttonDialog; visible: false }
        }
    ]
    transitions: Transition {
        NumberAnimation { target: buttonDialogBase; properties: "opacity"; duration: 500; easing.type: Easing.InOutQuad }
    }
}
