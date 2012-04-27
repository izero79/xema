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
public:

public:
    enum birdFields {
        BIRD_ID = 0,
        BIRD_FIN_GROUP,
        BIRD_SWE_GROUP,
        BIRD_LATIN_GROUP,
        BIRD_FIN_NAME,
        BIRD_SWE_NAME,
        BIRD_LATIN_NAME,
        BIRD_ABBREV,
        BIRD_CATEGORY
    };
    enum locationFields {
        LOCATION_TOWN = 0,
        LOCATION_PLACE,
        LOCATION_WGS,
        LOCATION_YKJ
    };
    enum personFields {
        PERSON_FIRSTNAME = 0,
        PERSON_SURNAME,
        PERSON_REGISTERED,
        PERSON_DEFAULT
    };
    enum statusFields {
        STATUS_FINABBREV = 0,
        STATUS_FINNAME,
        STATUS_SWENAME,
        STATUS_ENGNAME
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
        TIIRA_EXTRA1 // 35 weather
    };

    enum atlasIndexFields {
        ATLAS_VALUE
    };
    static const int OBS_SUBFIELDCOUNT = 12;


};

#endif // XEMAENUMS_H
