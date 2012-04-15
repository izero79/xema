/*
 * ykjetrs-bw.h
 *
 * Written by Ville Saari
 * Copyright 2003 National Land Survey of Finland
 *
 * Created: 2003-02-05
 * Updated: 2003-08-13
 */

#ifndef YKJETRS_BW_H
#define YKJETRS_BW_H

/*
 * Muuntaa pallokoordinaatistossa ilmaistun pisteen (fii, lambda) ETRF89:st‰
 * kkj:hin 7 parametrin Bursa-Wolf-muunnoksella ja sijoittaa tuloksen
 * pointtereiden fp ja lp osoittamiin muuttujiin.
 */

void etrs_kkj_bw_pallo(double fii, double lambda, double *fp, double *lp);


/*
 * Muuntaa tasokoordinaatistossa ilmaistun pisteen (p, i) ETRS-TM35FIN:st‰
 * ykj:hin 7 parametrin Bursa-Wolf-muunnoksella ja sijoittaa tuloksen
 * pointtereiden pp ja ip osoittamiin muuttujiin. Palauttaa nollasta
 * poikkeavan arvon normaalitilanteessa ja nollan, jos muunnos ep‰onnistui
 * siksi, ett‰ koordinaatit ovat liian kaukana kaistan keskimeridiaanista.
 */

int etrstm35fin_ykj_bw(double p, double i, double *pp, double *ip);


/*
 * K‰‰nteismuunnokset edellisille
 */

void kkj_etrs_bw_pallo(double fii, double lambda, double *fp, double *lp);
int ykj_etrstm35fin_bw(double p, double i, double *pp, double *ip);

#endif
