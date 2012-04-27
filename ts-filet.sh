#!/bin/sh
#
/Users/Tero/QtSDK/Desktop/Qt/474/gcc/bin/lupdate -I qml/* -no-obsolete -ts cuteremoteqml_en.ts
/Users/Tero/QtSDK/Desktop/Qt/474/gcc/bin/lupdate -I qml/* -no-obsolete -ts cuteremoteqml_fi.ts
#/Users/Tero/QtSDK/Desktop/Qt/474/gcc/bin/lupdate qml/*qml qml/*.js -no-obsolete -ts cuteremoteqml_sv.ts
#/Users/Tero/QtSDK/Desktop/Qt/474/gcc/bin/lupdate qml/*qml qml/*.js -no-obsolete -ts cuteremoteqml_de.ts
#/Users/Tero/QtSDK/Desktop/Qt/474/gcc/bin/lupdate qml/*qml qml/*.js -no-obsolete -ts cuteremoteqml_fr.ts

/Users/Tero/QtSDK/Desktop/Qt/474/gcc/bin/lupdate -no-obsolete CuteRemote.pro
