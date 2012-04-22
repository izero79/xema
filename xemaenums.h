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
        STATUS_NAME = 0,
        STATUS_ABBREV
    };
    enum observationFields {
        OBS_ID = 0,
        OBS_SPECIES = 1,
        OBS_DATE1 = 2,
        OBS_DATE2 = 3,
        OBS_TIME1 = 4,
        OBS_TIME2 = 5,
        OBS_PLACE = 6,
        OBS_INFO = 7,
        OBS_ATLAS = 8,
        OBS_REGPERSON = 9,
        OBS_OTHERPERSON = 10,
        OBS_HIDDEN = 11,
        OBS_ROWCOUNT = 12,
        OBS_BIRDCOUNT = 13,
        OBS_BIRDTIME1 = 14,
        OBS_BIRDTIME2 = 15,
        OBS_SEX = 16,
        OBS_DRESS = 17,
        OBS_AGE = 18,
        OBS_STATUS = 19,
        OBS_BIRDINFO = 20,
        OBS_LOFT = 21,
        OBS_BONGAUS = 22,
        OBS_NEST = 23,
        OBS_WEATHER = 24,
        OBS_EXPORTED = 25
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
        TIIRA_XCOORD,
        TIIRA_YCOORD,
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
        TIIRA_INDIRECT // 34
    };

    enum atlasIndexFields {
        ATLAS_VALUE
    };
    static const int OBS_SUBFIELDCOUNT = 12;


};

#endif // XEMAENUMS_H
