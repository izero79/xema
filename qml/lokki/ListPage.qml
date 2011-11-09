import QtQuick 1.0
import com.nokia.symbian 1.0


Page {
    id: listPage

    function clicked( name )
    {
        if( listPageType == "birds" )
        {
            window.birdChanged( name )
        }
        else if( listPageType == "places" )
        {
            window.placeChanged( name )
        }
        else if( listPageType == "regpeople")
        {
            window.regpeopleChanged( name )
        }
        else if( listPageType == "people")
        {
            window.peopleChanged( name )
        }
        pageStack.pop()
    }

    ListModel {
        id: testModelNames
        ListElement {
            name: "Toni U"
        }
        ListElement {
            name: "Uni Too"
        }
        ListElement {
            name: "Joku Muu"
        }
    }

    ListModel {
        id: testModelNames2
        ListElement {
            name: "Toni U"
        }
        ListElement {
            name: "Uni Too"
        }
        ListElement {
            name: "Joku Muu"
        }
    }

    ListModel {
        id: testModelBirds
        ListElement {
            name: "Talitiainen"
        }
        ListElement {
            name: "Tipu"
        }
        ListElement {
            name: "Harakka"
        }
        ListElement {
            name: "Vanha haahka"
        }
    }

    ListModel {
        id: testModelPlaces
        ListElement {
            name: "Kokkola, Herman Renlundinkatu"
        }
        ListElement {
            name: "Kokkola, Komia"
        }
        ListElement {
            name: "Kokkola, Calle"
        }
        ListElement {
            name: "Kokkola, Kärrynpyörä"
        }
    }

    Item {
        id: item1
        width: parent.width
        height: 50
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 8

        TextField {
            id: textfield1
            height: 50
            text: listPageType
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            onTextChanged: {
                console.log("teksti muuttuu: " + text)
            }
        }
    }
    ListView {
        model: {
            if( listPageType == "birds" )
            {
                return testModelBirds
            }
            else if( listPageType == "places" )
            {
                return testModelPlaces
            }
            else if( listPageType == "regpeople" )
            {
                return testModelNames
            }
            else
            {
                return testModelNames2
            }
        }
        anchors.top: item1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        delegate: myTestDelegate
        clip: true
    }
    ListDelegate {
        id: myTestDelegate

    }
}
