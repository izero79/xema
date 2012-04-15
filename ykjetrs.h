/*
 * ykjetrs.h
 *
 * Written by Ville Saari
 * Copyright 2003 National Land Survey of Finland
 *
 * Created: 2002-12-30
 * Updated: 2003-08-19
 */

#ifndef YKJETRS_H
#define YKJETRS_H

#define YKJETRS_VIRHE  0
#define YKJETRS_OK     1
#define YKJETRS_VIRT   2
#define YKJETRS_BW     3

/*
 * T‰m‰ muuntaa tasokoordinaatistossa ilmaistun pisteen (p, i) ETRS-TM35FIN:st‰
 * ykj:hin kolmioverkkomuunnoksella ja sijoittaa tuloksen pointtereiden pp
 * ja ip osoittamiin muuttujiin. Palauttaa jonkun yll‰olevista arvoista.
 * Normaalitapauksessa paluuarvo on YKJETRS_OK. YKJETRS_BW tarkoittaa,
 * ett‰ piste on kolmioverkon ulkopuolella ja muunnettiin 7 parametrin
 * Bursa-Wolf-muunnoksella. YKJETRS_VIRHE tarkoittaa muunnoksen ep‰onnistuneen
 * (esim. piste niin kaukana projektiokaistan keskimeridiaanista, ett‰
 * Bursa-Wolf-muunnoksen vaatima muunnos tasokoordinaateista
 * pallokoordinaatteihin ei en‰‰ ole mahdollista).
 *
 * (YKJETRS_VIRT arvoa t‰m‰ versio ei k‰yt‰)
 */

int etrstm35fin_ykj(double p, double i, double *pp, double *ip);

/*
 * Edellisen k‰‰nteisoperaatio
 */

int ykj_etrstm35fin(double p, double i, double *pp, double *ip);

#endif
