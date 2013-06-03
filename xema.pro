#CONFIG +=debug
#CONFIG -=release
CONFIG -=liteVersion
CONFIG -=forOvi
CONFIG -=forUnsigned
CONFIG -=S603x
CONFIG -=Symbian1
CONFIG -=performancetest
CONFIG -=useMediaKeys
DEFINES +=DEBUGONLYTOFILE
CONFIG +=mobility
MOBILITY +=location systeminfo

#DEFINES += PERFTEST
DEFINES += USE_MOBILITY

#oma
#DEFINES += ONLYFORIMEI=357923041627788
DEFINES += ONLYFORIMEI=0

VERSION = 1.1.0

DEFINES += MAJORVERSION=1
DEFINES += MINORVERSION=1
DEFINES += PATCHVERSION=0

TARGET = xema
DEPLOYMENT.display_name = "Xema"

customrules.pkg_prerules = \
        "; Localised Vendor name" \
        "%{\"Tero Siironen\"}" \
        " " \
        "; Unique Vendor name" \
        ":\"Tero Siironen\""

DEPLOYMENT += customrules

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =


# Add more folders to ship with the application, here
#folder_03.source = qml/common
#folder_03.target = qml
#DEPLOYMENTFOLDERS += folder_03


#folder_01.source = qml/symbian3
#folder_01.target = qml
#DEPLOYMENTFOLDERS += folder_01
#folder_02.source = qml/symbian3_icons
#folder_02.target = qml
#DEPLOYMENTFOLDERS += folder_02
#folder_04.source = qml/harmattan
#folder_04.target = qml
#DEPLOYMENTFOLDERS += folder_04
#folder_05.source = qml/harmattan_icons
#folder_05.target = qml
#DEPLOYMENTFOLDERS += folder_05

symbian{

forOvi{
    # For symbian signed app
    # when building signed app, enable define below
    DEFINES += BUILDFORSIGNED

    # UID for ovi store
    symbian:TARGET.UID3 = 0x20046EA2

    # needed for network info, but needs signing
    symbian:TARGET.CAPABILITY += ReadDeviceData
    symbian:TARGET.CAPABILITY += SwEvent
    symbian:TARGET.CAPABILITY += ReadUserData

}else{
    !forUnsigned{
        # For symbian signed app
        # when building signed app, enable define below
        DEFINES += BUILDFORSIGNED

        # UID for symbian signed
        symbian:TARGET.UID3 = 0x2006AF48

        # needed for network info, but needs signing
        symbian:TARGET.CAPABILITY += ReadDeviceData
        symbian:TARGET.CAPABILITY += SwEvent
        symbian:TARGET.CAPABILITY += ReadUserData
    }else{
        # For self signed app:
        # UID for self signed
        # A0015538 should be
        symbian:TARGET.UID3 = 0xA0015538
    }
}


# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

symbian3 = \
"; Symbian3" \
"[0x20022E6D],0,0,0,{\"S60ProductId\"}"

qtquickcomponents = \
"; Qt Quick components" \
"(0x200346DE),1,1,0,{\"Qt Quick components\"}"

!S603x{
    Symbian1{
    default_deployment.pkg_prerules -= pkg_platform_dependencies
    my_deployment.pkg_prerules += symbian1
    }else{
    #remove platform dependencies only
    default_deployment.pkg_prerules -= pkg_platform_dependencies
    #remove webkit component dependencies
    default_deployment.pkg_prerules -= pkg_depends_webkit

    DEFINES += SYMBIAN3
    my_deployment.pkg_prerules += symbian3 qtquickcomponents
    }
}
DEPLOYMENT += my_deployment

DEPLOYMENT.installer_header = 0x2002CCCF
}

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices
symbian:TARGET.CAPABILITY += Location
symbian:TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

unix:!symbian:!maemo5:!macx {
# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
    CONFIG += qdeclarative-boostable
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic


    DEFINES += HARMATTAN

    splash_portrait.files = splash_portrait.png
    splash_portrait.path = /opt/xema/
    splash_landscape.files = splash_landscape.png
    splash_landscape.path = /opt/xema/

    INSTALLS += splash_portrait splash_landscape

}

# Add dependency to Symbian components
CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    person.cpp \
    location.cpp \
    bird.cpp \
    birdmodel.cpp \
    locationmodel.cpp \
    personmodel.cpp \
    modeldataloader.cpp \
    applicationcontroller.cpp \
    qmlwindow.cpp \
    filtermodel.cpp \
    status.cpp \
    statusmodel.cpp \
    historyitem.cpp \
    historymodel.cpp \
    settings.cpp \
    atlasindex.cpp \
    atlasindexmodel.cpp \
    ykjetrs.cpp \
    ykjetrsdata.cpp \
    ykjetrs-bw.cpp \
    gausskrueger.cpp \
    coordinateconverter.cpp \
    age.cpp \
    dress.cpp \
    sex.cpp \
    agemodel.cpp \
    dressmodel.cpp \
    sexmodel.cpp \
    systeminfoprovider.cpp \
    direction.cpp \
    directionmodel.cpp \
    modeldatawriter.cpp \
    kineticscroller.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qmldir

HEADERS += \
    person.h \
    location.h \
    bird.h \
    birdmodel.h \
    locationmodel.h \
    personmodel.h \
    modeldataloader.h \
    applicationcontroller.h \
    qmlwindow.h \
    filtermodel.h \
    status.h \
    statusmodel.h \
    dummydebugprinter.h \
    debugprinter.h \
    modeldatawriter.h \
    historyitem.h \
    historymodel.h \
    settings.h \
    xemaconstants.h \
    atlasindex.h \
    atlasindexmodel.h \
    coordinateconverter.h \
    xemaenums.h \
    age.h \
    dress.h \
    sex.h \
    agemodel.h \
    dressmodel.h \
    sexmodel.h \
    systeminfoprovider.h \
    direction.h \
    directionmodel.h \
    kineticscroller.h

RESOURCES += \
    commonresources.qrc

symbian{
RESOURCES += \
    symbianresources.qrc
}else{
RESOURCES += \
    harmattanresources.qrc
}

macx|win32 {
    RESOURCES += \
    symbianresources.qrc \
}

OTHER_FILES += \
    ykjetrs.h \
    ykjetrsdata.h \
    ykjetrs-bw.h \
    ykjetrsdata-bw.h \
    gausskrueger.h \
    qml/harmattan/* \
    qml/symbian3/* \
    splash_landscape.png \
    splash_portrait.png
