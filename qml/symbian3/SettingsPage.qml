import QtQuick 1.1
import com.nokia.symbian 1.1
import Qt.labs.components 1.1


Page {
    id: settingsPage
    tools: toolBarLayout

    property bool usernameEdited: false
    property bool pwdEdited: false

    function editPeople(indexi)
    {
        var editorCompnent = Qt.createComponent(Qt.resolvedUrl("PersonEditPage.qml"))
        var editorObject = editorCompnent.createObject(window)
        pageStack.push(editorObject)
        editorObject.selectedIndex(indexi)
    }

    function editBird(indexi)
    {
        var editorCompnent = Qt.createComponent(Qt.resolvedUrl("BirdEditPage.qml"))
        var editorObject = editorCompnent.createObject(window)
        pageStack.push(editorObject)
        editorObject.selectedIndex(indexi)
    }

    function editStatus(indexi)
    {
        var editorCompnent = Qt.createComponent(Qt.resolvedUrl("StatusEditPage.qml"))
        var editorObject = editorCompnent.createObject(window)
        pageStack.push(editorObject)
        editorObject.selectedIndex(indexi)
    }

    function editLocation(indexi)
    {
        var editorCompnent = Qt.createComponent(Qt.resolvedUrl("LocationEditPage.qml"))
        var editorObject = editorCompnent.createObject(window)
        pageStack.push(editorObject)
        editorObject.selectedIndex(indexi)
    }

    function loginOk(name) {
        tiiraStatus.text = qsTr("Status: Signed in as %1").arg(name)
    }

    function loginFailedCredientals() {
        tiiraStatus.text = qsTr("Status: Wrong username/password")
    }

    function loginFailedNoRights() {
        tiiraStatus.text = qsTr("Status: Signed in. No upload rights")
    }

    function serverLoginFailed() {
        tiiraStatus.text = qsTr("Status: Tiira environment credientals wrong")
    }

    function loginFailedUnknown() {
        tiiraStatus.text = qsTr("Status: Unknown error")
    }

    function setServerCredientals() {
        console.log('setServerCredientals')
        window.tiiraServerUsername = tiiraServerUsername.text
        window.tiiraServerPassword = tiiraServerPassword.text
        window.saveTiiraServerUsername(tiiraServerUsername.text)
        window.saveTiiraServerPassword(tiiraServerPassword.text)
    }

    function checkCredientalsAndLogin() {
        console.log('checkCredientalsAndLogin')
        if (usernameEdited) {
            window.tiiraUsername = tiiraUsername.text
            window.saveTiiraUsername(tiiraUsername.text)
        }
        if (pwdEdited) {
            window.tiiraPwdHash = tiiraPassword.text
            window.saveTiiraPwdHash(tiiraPassword.text)
        }
        usernameEdited = false
        pwdEdited = false
        window.tiira_login()
    }

    onStatusChanged: {
        if (settingsPage.status == PageStatus.Active) {
            if (window.useTiira) {
                window.tiira_login()
            }
        }
    }

    Dialog {
        id: areYouSureDialog
        property alias titleText: titleTextField.text
        property alias dialogText: dialogTextField.text
        signal oksignal()
        signal cancelsignal()

        title: Label {
            id: titleTextField
            height: 30
            anchors.centerIn: parent
            width: parent.width
            color: "white"
            font.pixelSize: 36
            text: ""
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        content:Item {
            height: 150
            width: parent.width
            anchors.topMargin: 10
            Label {
                id: dialogTextField
                width: parent.width
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: ""
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 20
            }
        }

        buttons: Item { height: dialogYes.height + 2 * 20; width: parent.width - 20
            Button {
                id: dialogYes
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 5
                width: parent.width / 2
                text: qsTr("Yes")
                onClicked: {
                    areYouSureDialog.oksignal()
                    areYouSureDialog.close()
                }
            }
            Button {
                id: dialogNo
                anchors.bottom: parent.bottom
                anchors.left: dialogYes.right
                anchors.margins: 5
                width: parent.width / 2
                text: qsTr("No")
                onClicked: {
                    areYouSureDialog.cancelsignal()
                    areYouSureDialog.close()
                }
            }
        }
        onClickedOutside: {areYouSureDialog.cancelsignal(); areYouSureDialog.close()}
    }

    Flickable {
        contentWidth: width
        contentHeight: restoreHistory.y + restoreHistory.height + 10
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent
        anchors.margins: 5

        Label {
            id: systematicSortText
            text: qsTr("Species list sorting")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: parent.top
            color: "#ffffff"
        }

        CheckableGroup { id: orderGroup }
        Column {
            id: row1
            spacing: platformStyle.paddingMedium
            anchors.top: systematicSortText.bottom
            anchors.left: parent.left
            RadioButton {
                id: alphaButton
                text: qsTr("Alphabetical")
                platformExclusiveGroup: orderGroup
                checked: window.useSystematicSort == false
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.useSystematicSort = false
                        window.saveSystematicSorting(!checked)
                    }
                }
            }
            RadioButton {
                id: systemButton
                text: qsTr("Systematic")
                platformExclusiveGroup: orderGroup
                checked: window.useSystematicSort == true
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.useSystematicSort = true
                        window.saveSystematicSorting(checked)
                    }
                }
            }
        }

        Label {
            id: speciesListText
            text: qsTr("Species list")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 30
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: row1.bottom
            color: "#ffffff"
        }

        CheckableGroup { id: speciesGroup }
        Column {
            id: row2
            spacing: platformStyle.paddingMedium
            anchors.top: speciesListText.bottom
            anchors.left: parent.left
            RadioButton {
                id: finnishButton
                text: qsTr("Finnish")
                platformExclusiveGroup: speciesGroup
                checked: window.wpSpecies == false
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.wpSpecies = false
                        window.saveWPSpecies(!checked)
                    }
                }
            }
            RadioButton {
                id: wpButton
                text: qsTr("Western Palearctic")
                platformExclusiveGroup: speciesGroup
                checked: window.wpSpecies == true
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.wpSpecies = true
                        window.saveWPSpecies(checked)
                    }
                }
            }
        }
        CheckableGroup { id: group }
        Label {
            id: groupHeader
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.top: row2.bottom
            anchors.topMargin: 30
            text: qsTr("Default amount of fields")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            color: "#ffffff"
        }

        Column {
            id: row3
            spacing: platformStyle.paddingMedium
            anchors.top: groupHeader.bottom
            anchors.left: parent.left
            RadioButton {
                id: button1
                text: qsTr("Minimum")
                platformExclusiveGroup: group
                checked: window.defaultDetailLevel == 1
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.defaultDetailLevel = 1
                        window.saveDetailLevel(1)
                    }
                }
            }
            RadioButton {
                id: button2
                text: qsTr("Expanded")
                platformExclusiveGroup: group
                checked: window.defaultDetailLevel == 2
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.defaultDetailLevel = 2
                        window.saveDetailLevel(2)
                    }
                }
            }
            RadioButton {
                id: button3
                text: qsTr("All")
                platformExclusiveGroup: group
                checked: window.defaultDetailLevel == 3
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.defaultDetailLevel = 3
                        window.saveDetailLevel(3)
                    }
                }
            }
        }
        Label {
            id: defaultCountyText
            text: qsTr("Default country")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: row3.bottom
            anchors.topMargin: 30
            color: "#ffffff"
        }
        TextField {
            id: defaultCountryTf
            height: 50
            placeholderText: qsTr("Default country")
            text: window.defaultCountry
            anchors.top: defaultCountyText.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            onTextChanged: {
                if (activeFocus) {
                    locationSetTimer.start()
                }
            }

            Timer {
                id: locationSetTimer
                interval: 2000
                running: false
                repeat: false
                onTriggered: {
                    window.saveDefaultCountry(defaultCountryTf.text)
                    window.defaultCountry = defaultCountryTf.text
                    window.saveDefaultAssociation(defaultAssociationTf.text)
                    window.defaultAssociation = defaultAssociationTf.text
                }
            }
        }
        CheckBox {
            id: defaultCountryCb
            anchors.top: defaultCountryTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            checked: window.onlyDefaultCountry == true
            text: qsTr("Use only default country")
            onCheckedChanged: {
                window.onlyDefaultCountry = checked
                window.saveOnlyDefaultCountry(checked)
            }
        }

        Label {
            id: defaultAssociationText
            text: qsTr("Default association")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: defaultCountryCb.bottom
            anchors.topMargin: 30
            color: "#ffffff"
        }
        TextField {
            id: defaultAssociationTf
            height: 50
            placeholderText: qsTr("Default association")
            text: window.defaultAssociation
            anchors.top: defaultAssociationText.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            onTextChanged: {
                if (activeFocus) {
                    locationSetTimer.start()
                }
            }

        }
        CheckBox {
            id: defaultAssociationCb
            anchors.top: defaultAssociationTf.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            checked: window.onlyDefaultAssiciation == true
            text: qsTr("Use only default association")
            onCheckedChanged: {
                window.onlyDefaultAssiciation = checked
                window.saveOnlyDefaultAssociation(checked)
            }
        }
        Button {
            id: selectAssociations
            anchors.top: defaultAssociationCb.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Select")
            onClicked: window.showListPage("associations", defaultAssociationTf.text, defaultAssociationTf);
        }
        CheckBox {
            id: alwaysShowOwnCb
            anchors.top: selectAssociations.bottom
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            checked: window.alwaysShowOwn == true
            text: qsTr("Always show own/modified locations")
            onCheckedChanged: {
                window.alwaysShowOwn = checked
                window.saveAlwaysShowOwn(checked)
            }
        }

        Label {
            id: coordinateSystemText
            text: qsTr("Coordinate system in export")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 30
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: alwaysShowOwnCb.bottom
            color: "#ffffff"
        }

        CheckableGroup { id: coordinateGroup }
        Column {
            id: row4
            spacing: platformStyle.paddingMedium
            anchors.top: coordinateSystemText.bottom
            anchors.left: parent.left
            RadioButton {
                id: wgsButton
                text: qsTr("WGS84")
                platformExclusiveGroup: coordinateGroup
                checked: window.exportWgs == true
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.exportWgs = true
                        window.saveExportWgs(checked)
                    }
                }
            }
            RadioButton {
                id: ykjButton
                text: qsTr("YKJ (Used only in Finland)")
                platformExclusiveGroup: coordinateGroup
                checked: window.exportWgs == false
                property bool wasPressed: false
                onPressedChanged: {
                    if (pressed == true)
                    {
                        wasPressed = true
                    }
                }

                onCheckedChanged: {
                    if (checked == true && wasPressed == true)
                    {
                        wasPressed = false
                        window.exportWgs = false
                        window.saveExportWgs(!checked)
                    }
                }
            }
        }

        Item {
            id: tiiraItems
            height: childrenRect.height
            anchors.left: parent.left
            anchors.top: row4.bottom
            anchors.topMargin: 30
            anchors.right: parent.right
            CheckBox {
                id: useTiiraLabel
                text: qsTr("Use Tiira service")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.right: parent.right
                checked: window.useTiira == true
                onCheckedChanged: {
                    window.useTiira = checked
                    window.saveUseTiira(checked)
                    if (checked) {
                        checkCredientalsAndLogin()
                    }
                }
            }
            Item {
                anchors.top: useTiiraLabel.bottom
                anchors.topMargin: 8
                anchors.left: parent.left
                anchors.right: parent.right
                visible: useTiiraLabel.checked
                height: visible ? childrenRect.height : 0
                Label {
                    id: tiiraStatus
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    text: qsTr("Status: Not signed in")
                }
                Item {
                    id: signing
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: tiiraStatus.bottom
                    anchors.topMargin: 8
                    height: childrenRect.height
                    TextField {
                        id: tiiraUsername
                        anchors.left: parent.left
                        anchors.right: tiiraSignin.left
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        placeholderText: qsTr("Username")
                        text: window.tiiraUsername
                        onTextChanged: {
                            if (activeFocus) {
                                usernameEdited = true
                            }
                        }
                    }
                    TextField {
                        id: tiiraPassword
                        anchors.left: parent.left
                        anchors.right: tiiraSignin.left
                        anchors.top: tiiraUsername.bottom
                        anchors.topMargin: 8
                        placeholderText: qsTr("Password")
                        echoMode: TextInput.Password
                        text: window.tiiraPwdHash
                        onTextChanged: {
                            if (activeFocus) {
                                pwdEdited = true
                            }
                        }

                    }
                    Button {
                        id: tiiraSignin
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        width: 150
                        text: qsTr("Sign in")
                        onClicked: {
                            checkCredientalsAndLogin();
                        }
                        enabled: !window.tiiraLoginOk || settingsPage.usernameEdited || settingsPage.pwdEdited
                    }
                }
                CheckBox {
                    id: autosaveTiira
                    text: qsTr("Save new records to Tiira")
                    anchors.left: parent.left
                    anchors.top: signing.bottom
                    anchors.topMargin: 24
                    anchors.right: parent.right
                    checked: window.tiiraAutosave == true
                    onCheckedChanged: {
                        window.tiiraAutosave = checked
                        window.saveTiiraAutosave(checked)
                    }
                }
                Label {
                    id: tiiraEnvLabel
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: autosaveTiira.bottom
                    anchors.topMargin: 8
                    text: qsTr("Tiira environment")
                }
                Item {
                    id: tiiraServerItem
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: tiiraEnvLabel.bottom
                    anchors.topMargin: 8
                    height: childrenRect.height
                    CheckableGroup { id: serverGroup }
                    Column {
                        id: tiiraServerRow
                        spacing: platformStyle.paddingMedium
                        anchors.top: parent.top
                        anchors.left: parent.left
                        RadioButton {
                            id: tiiraProd
                            text: qsTr("Production")
                            platformExclusiveGroup: serverGroup
                            checked: window.tiiraServer == 0
                            property bool wasPressed: false
                            onPressedChanged: {
                                if (pressed == true)
                                {
                                    wasPressed = true
                                }
                            }

                            onCheckedChanged: {
                                if (checked == true && wasPressed == true)
                                {
                                    wasPressed = false
                                    window.tiiraServer = 0
                                    window.saveTiiraServer(0)
                                    setServerCredientals()
                                    checkCredientalsAndLogin()
                                }
                            }
                        }
                        RadioButton {
                            id: tiiraTest
                            text: qsTr("Test")
                            platformExclusiveGroup: serverGroup
                            checked: window.tiiraServer == 1
                            property bool wasPressed: false
                            onPressedChanged: {
                                if (pressed == true)
                                {
                                    wasPressed = true
                                }
                            }

                            onCheckedChanged: {
                                if (checked == true && wasPressed == true)
                                {
                                    wasPressed = false
                                    window.tiiraServer = 1
                                    window.saveTiiraServer(1)
                                    setServerCredientals()
                                    checkCredientalsAndLogin()
                                }
                            }
                        }
                        RadioButton {
                            id: tiiraDev
                            text: qsTr("Dev")
                            platformExclusiveGroup: serverGroup
                            checked: window.tiiraServer == 2
                            property bool wasPressed: false
                            onPressedChanged: {
                                if (pressed == true)
                                {
                                    wasPressed = true
                                }
                            }

                            onCheckedChanged: {
                                if (checked == true && wasPressed == true)
                                {
                                    wasPressed = false
                                    window.tiiraServer = 2
                                    window.saveTiiraServer(2)
                                    setServerCredientals()
                                    checkCredientalsAndLogin()
                                }
                            }
                        }
                    }

                    TextField {
                        id: tiiraServerUsername
                        anchors.left: parent.left
                        anchors.right: tiiraServerOk.left
                        anchors.top: tiiraServerRow.bottom
                        anchors.topMargin: 8
                        placeholderText: qsTr("Username")
                        text: window.tiiraServerUsername
                    }
                    TextField {
                        id: tiiraServerPassword
                        anchors.left: parent.left
                        anchors.right: tiiraServerOk.left
                        anchors.top: tiiraServerUsername.bottom
                        anchors.topMargin: 8
                        placeholderText: qsTr("Password")
                        echoMode: TextInput.Password
                        text: window.tiiraServerPassword
                    }
                    Button {
                        id: tiiraServerOk
                        anchors.right: parent.right
                        anchors.top: tiiraServerRow.bottom
                        anchors.topMargin: 8
                        width: 150
                        text: qsTr("Set")
                        onClicked: {
                            setServerCredientals()
                            checkCredientalsAndLogin()
                        }
                    }
                }
            }

        }

        Label {
            id: editText
            text: qsTr("Manage lists")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: tiiraItems.bottom
            anchors.topMargin: 30
            color: "#ffffff"
        }
        Button {
            id: editPeople
            anchors.top: editText.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Observers")
            onClicked: window.showListPage("editallpeople", "");
        }
        Button {
            id: editLocation
            anchors.top: editPeople.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Locations")
            onClicked: window.showListPage("editplaces", "");
        }
        Button {
            id: editBird
            anchors.top: editLocation.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Species")
            onClicked: window.showListPage("editbirds", "");
        }
        Button {
            id: editStatus
            anchors.top: editBird.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Statuses")
            onClicked: window.showListPage("editstatuses", "");
        }

        Label {
            id: backupText
            text: qsTr("Export/Import own lists")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: editStatus.bottom
            anchors.topMargin: 30
            color: "#ffffff"
        }
        Button {
            id: backupButton
            anchors.top: backupText.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Export")
            onClicked: window.exportOwnData()
        }
        Button {
            id: restoreButton
            anchors.top: backupButton.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Import")
            onClicked: window.importOwnData()
        }

        Label {
            id: restoreText
            text: qsTr("Restore default lists")
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            anchors.top: restoreButton.bottom
            anchors.topMargin: 30
            color: "#ffffff"
        }
        Button {
            id: restorePeople
            anchors.top: restoreText.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Observers")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Clear observers?")
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.open()
            }
            function callback() {
                window.restoreObservers()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }

        }
        Button {
            id: restoreLocation
            anchors.top: restorePeople.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Locations")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Restore locations?")
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.open()
            }
            function callback() {
                window.restoreLocations()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }

        }
        Button {
            id: restoreBird
            anchors.top: restoreLocation.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Species")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Restore species?")
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.open()
            }
            function callback() {
                window.restoreSpecies()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }

        }
        Button {
            id: restoreStatus
            anchors.top: restoreBird.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Statuses")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Restore statuses?")
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.open()
            }
            function callback() {
                window.restoreStatuses()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
        }
        Button {
            id: restoreHistory
            anchors.top: restoreStatus.bottom
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            text: qsTr("Observations")
            onClicked: {
                areYouSureDialog.titleText = qsTr("Restore")
                areYouSureDialog.dialogText = qsTr("Clear all history?")
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.connect(callback)
                areYouSureDialog.open()
            }
            function callback() {
                window.clearHistory()
                areYouSureDialog.cancelsignal.connect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
            function cancelCallback() {
                areYouSureDialog.cancelsignal.disconnect(cancelCallback)
                areYouSureDialog.oksignal.disconnect(callback)
            }
        }
    }
}
