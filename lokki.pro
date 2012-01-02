# Add more folders to ship with the application, here
folder_01.source = qml/lokki
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

#CONFIG +=debug
#CONFIG -=release
CONFIG -=liteVersion
CONFIG -=forOvi
CONFIG -=forUnsinged
CONFIG -=S603x
CONFIG -=Symbian1
CONFIG -=performancetest
CONFIG -=useMediaKeys
#symbian:DEFINES +=DEBUGONLYTOFILE

!S603x{
    Symbian1{
    }else{
    CONFIG +=qt-components
    DEFINES += SYMBIAN3
    }
}

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE3CA679E

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices
symbian:TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

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
    modeldatawriter.cpp \
    historyitem.cpp \
    historymodel.cpp \
    settings.cpp

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

symbian {

symbian3 = \
"; Symbian3" \
"[0x20022E6D],0,0,0,{\"S60ProductId\"}"

qtquickcomponents = \
"; Qt Quick components" \
"(0x200346DE),1,1,0,{\"Qt Quick components\"}"

my_deployment.pkg_prerules += symbian3
my_deployment.pkg_prerules += qtquickcomponents
}

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
    lokkiconstants.h

RESOURCES += \
    symbianresources.qrc \
    commonresources.qrc
