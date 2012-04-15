/*
 * ykjetrsdata.h
 *
 * Written by Ville Saari
 * Copyright 2003 National Land Survey of Finland
 *
 * Created: 2002-12-30
 * Updated: 2003-08-15
 */

#ifndef YKJETRSDATA_H
#define YKJETRSDATA_H

struct ykjetrsdata
   {
   const int index_p, index_i;
   const double alku_p, alku_i;
   const double ruutu_p, ruutu_i;
   const short * const *index;
   const double (* const piste)[2];
   const unsigned short (* const kolmio)[3];
   const double (*const args)[6];
   };

extern const struct ykjetrsdata ykjetrsdata, etrsykjdata;

#endif
