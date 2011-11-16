import QtQuick 1.0
import com.nokia.symbian 1.1


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
        else if( listPageType == "sex")
        {
            window.sexChanged( name )
        }
        else if( listPageType == "age")
        {
            window.ageChanged( name )
        }
        else if( listPageType == "dress")
        {
            window.dressChanged( name )
        }
        pageStack.pop()
    }

    function selectNames( names )
    {
        var selNames = new Array();
        console.log( "names: " + names )

        selNames = names.split(",")
        console.log( "name[0]: " + selNames[0] )

        for( var i = 0; i < selNames.length; i++ )
        {
            for( var j = 0; j < personModel.rowCount(); j++ )
            {
                if( personModel.data( j, 35 ) == selNames[i] )
                {
                    personModel.setData( j, true, 0 )
                }
            }
        }
    }

    function selectStatus( names )
    {
        var selNames = new Array();
        console.log( "select status names: " + names )

        selNames = names.split(",")
        console.log( "names length: " + selNames.length )

        if( selNames.length == 1 && selNames[0] == "" )
        {
            return
        }

        for( var i = 0; i < selNames.length; i++ )
        {
            for( var j = 0; j < statusModel.rowCount(); j++ )
            {
                if( statusModel.data( j, 35 ) == selNames[i] )
                {
                    statusModel.setData( j, true, 0 )
                }
            }
        }
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

    ListModel {
        id: sexModel
        ListElement {
            name: "koiras"
            selected: false
        }
        ListElement {
            name: "naaras"
            selected: false
        }
        ListElement {
            name: "pariutuneet"
            selected: false
        }
    }

    ListModel {
        id: ageModel
        ListElement {    name: "fl" }
        ListElement {    name: "pm" }
        ListElement {    name: "pp" }
        ListElement {    name: "+1kv" }
        ListElement {    name: "1kv" }
        ListElement {    name: "+2kv" }
        ListElement {    name: "2kv" }
        ListElement {    name: "+3kv" }
        ListElement {    name: "3kv" }
        ListElement {    name: "+4kv" }
        ListElement {    name: "4kv" }
        ListElement {    name: "+5kv" }
        ListElement {    name: "5kv" }
        ListElement {    name: "+6kv" }
        ListElement {    name: "6kv" }
        ListElement {    name: "+7kv" }
        ListElement {    name: "7kv" }
        ListElement {    name: "+8kv" }
        ListElement {    name: "8kv" }
    }

    ListModel {
        id: dressModel
        ListElement { name: "ad" }
        ListElement { name: "eijp" }
        ListElement { name: "imm" }
        ListElement { name: "jp" }
        ListElement { name: "juv" }
        ListElement { name: "n-puk" }
        ListElement { name: "pep" }
        ListElement { name: "pull" }
        ListElement { name: "ss" }
        ListElement { name: "subad" }
        ListElement { name: "tp" }
        ListElement { name: "vp" }
    }

    Component.onCompleted: {
        console.log("ListPage loaded")
        listView.model.filter( "" )
        for( var i = 0; i < listView.model.rowCount(); i++ )
        {
            listView.model.setData( i, false, 0 )
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
            text: "" //listPageType
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            onTextChanged: {
                console.log("teksti muuttuu: " + text)
                listView.model.filter( text )
            }
        }
    }
    ListView {
        id: listView
        property bool showRegistered: false
        model: {
            if( listPageType == "birds" )
            {
                return birdModel
            }
            else if( listPageType == "places" )
            {
                return locationModel
            }
            else if( listPageType == "status" )
            {
                return statusModel
            }
            else if( listPageType == "sex" )
            {
                return sexModel
            }
            else if( listPageType == "dress" )
            {
                return dressModel
            }
            else if( listPageType == "age" )
            {
                return ageModel
            }
            else if( listPageType == "regpeople" )
            {
                return personModel
            }
            else
            {
                return personModel
            }
        }
        anchors.top: item1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        delegate: {
            if( listPageType == "birds" )
            {
                if( useSystematicSort == true )
                {
                    model.setSorting( 0, true )
                }
                else
                {
                    model.setSorting( 1, true )
                }
                return birdDelegate
            }
            else if( listPageType == "places" )
            {
                return locationDelegate
            }
            else if( listPageType == "status" )
            {
                return statusDelegate
            }
            else if( listPageType == "dress" || listPageType == "age" || listPageType == "sex" )
            {
                return simpleDelegate
            }
            else if( listPageType == "regpeople" )
            {
                showRegistered = true

                return myTestDelegate
            }
            else
            {
                showRegistered = false
                return myTestDelegate
            }
        }
        clip: true
    }
    ListDelegate {
        id: myTestDelegate

    }
    BirdListDelegate {
        id: birdDelegate

    }
    LocationListDelegate {
        id: locationDelegate

    }
    StatusListDelegate {
        id: statusDelegate

    }
    SimpleDelegate {
        id: simpleDelegate

    }
}
