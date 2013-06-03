#ifndef XEMAENUMS_H
#define XEMAENUMS_H

#include <QObject>

class XemaEnums : public QObject {
    Q_OBJECT
    Q_ENUMS(birdFields)
    Q_ENUMS(locationFields)
    Q_ENUMS(personFields)
    Q_ENUMS(observationFields)
    Q_ENUMS(statusFields)
    Q_ENUMS(atlasIndexFields)
    Q_ENUMS(tiiraFields)
    Q_ENUMS(importErrors)
    Q_ENUMS(importSuccess)
public:

public:
    enum birdFields {
        BIRD_ID = 0,
        BIRD_FIN_GROUP,
        BIRD_ENG_GROUP,
        BIRD_LATIN_GROUP,
        BIRD_FIN_NAME,
        BIRD_SWE_NAME,
        BIRD_LATIN_NAME,
        BIRD_ABBREV,
        BIRD_CATEGORY,
        BIRD_ENG_NAME,
        BIRD_SWE_GROUP,
        BIRD_CUSTOM
    };
    enum locationFields {
        LOCATION_TOWN = 0,
        LOCATION_PLACE,
        LOCATION_WGS,
        LOCATION_YKJ,
        LOCATION_SWETOWN,
        LOCATION_SWEPLACE,
        LOCATION_ENGTOWN,
        LOCATION_ENGPLACE,
        LOCATION_COUNTRY,
        LOCATION_SWECOUNTRY,
        LOCATION_ENGCOUNTRY,
        LOCATION_CUSTOM
    };
    enum personFields {
        PERSON_FIRSTNAME = 0,
        PERSON_SURNAME,
        PERSON_REGISTERED,
        PERSON_DEFAULT,
        PERSON_SAVER
    };
    enum statusFields {
        STATUS_FINABBREV = 0,
        STATUS_FINNAME,
        STATUS_SWENAME,
        STATUS_ENGNAME,
        STATUS_CUSTOM
    };
    enum observationFields {
        OBS_ID = 0,
        OBS_SPECIES,
        OBS_DATE1,
        OBS_DATE2,
        OBS_TIME1,
        OBS_TIME2, // 5
        OBS_TOWN,
        OBS_LOCATION,
        OBS_XCOORD,
        OBS_YCOORD,
        OBS_ACCURACY, // 10
        OBS_BIRD_XCOORD,
        OBS_BIRD_YCOORD,
        OBS_BIRD_ACCURACY,
        OBS_PAIKANNETTU,
        OBS_INFO, // 15
        OBS_ATLAS,
        OBS_SAVER,
        OBS_SAVETIME,
        OBS_REGPERSON,
        OBS_OTHERPERSON, // 20
        OBS_HIDDEN,
        OBS_KOONTIHAVAINTO,
        OBS_KUULUUHAVAINTOON,
        OBS_ROWCOUNT,
        OBS_BIRDCOUNT, // 25
        OBS_BIRDTIME1,
        OBS_BIRDTIME2,
        OBS_SEX,
        OBS_DRESS,
        OBS_AGE, // 30
        OBS_STATUS,
        OBS_BIRDINFO,
        OBS_LOFT,
        OBS_BONGAUS,
        OBS_NEST, // 35
        OBS_INDIRECT,
        OBS_WEATHER,
        OBS_EXPORTED // 38
    };

    enum tiiraFields {
        TIIRA_ID = 0,
        TIIRA_SPECIES_ABBR,
        TIIRA_DATE1,
        TIIRA_DATE2,
        TIIRA_TIME1,
        TIIRA_TIME2, //5
        TIIRA_TOWN,
        TIIRA_LOCATION,
        TIIRA_YCOORD,
        TIIRA_XCOORD,
        TIIRA_ACCURACY, // 10
        TIIRA_BIRD_XCOORD,
        TIIRA_BIRD_YCOORD,
        TIIRA_BIRD_ACCURACY,
        TIIRA_PAIKANNETTU,
        TIIRA_INFO, // 15
        TIIRA_ATLAS,
        TIIRA_SAVER,
        TIIRA_SAVETIME,
        TIIRA_PERSONS,
        TIIRA_HIDDEN, // 20
        TIIRA_KOONTIHAVAINTO,
        TIIRA_KUULUUHAVAINTOON,
        TIIRA_BIRDCOUNT,
        TIIRA_BIRDTIME1,
        TIIRA_BIRDTIME2, // 25
        TIIRA_SEX,
        TIIRA_DRESS,
        TIIRA_AGE,
        TIIRA_STATUS,
        TIIRA_BIRDINFO, // 30
        TIIRA_LOFT,
        TIIRA_BONGAUS,
        TIIRA_NEST,
        TIIRA_INDIRECT, // 34
        TIIRA_EXTRA_WEATHER, // 35 weather
        TIIRA_EXTRA_COUNTRY // 36 country
    };

    enum atlasIndexFields {
        ATLAS_VALUE = 0,
        ATLAS_FIN,
        ATLAS_SWE,
        ATLAS_ENG
    };

    enum sexFields {
        SEX_VALUE = 0,
        SEX_FIN,
        SEX_SWE,
        SEX_ENG
    };

    enum dressFields {
        DRESS_VALUE = 0,
        DRESS_FIN,
        DRESS_SWE,
        DRESS_ENG
    };

    enum ageFields {
        AGE_VALUE = 0,
        AGE_FIN,
        AGE_SWE,
        AGE_ENG
    };

    enum directionFields {
        DIRECTION_VALUE = 0,
        DIRECTION_FIN,
        DIRECTION_SWE,
        DIRECTION_ENG
    };

    static const int OBS_SUBFIELDCOUNT = 12;

    enum importErrors {
        IMPORT_NOERRORS = 0,
        IMPORT_LOCATIONERROR = 1,
        IMPORT_PERSONERROR = 2,
        IMPORT_BIRDERROR = 4,
        IMPORT_STATUSERROR = 8,
        IMPORT_HISTORYERROR = 16
    };

    enum importSuccess {
        IMPORT_LOCATION_OK = 32,
        IMPORT_PERSON_OK = 64,
        IMPORT_BIRD_OK = 128,
        IMPORT_STATUS_OK = 256,
        IMPORT_HISTORY_OK = 512
    };

};

#endif // XEMAENUMS_H
