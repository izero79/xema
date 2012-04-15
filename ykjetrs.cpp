/*
 * ykjetrs.c
 *
 * Written by Ville Saari
 * Copyright 2003 National Land Survey of Finland
 *
 * Created: 2002-12-30
 * Updated: 2003-08-15
 */

#include <stdio.h>
#include <math.h>

#include "ykjetrs.h"
#include "ykjetrsdata.h"
#include "ykjetrs-bw.h"

static int pistekolmiossa(double x, double y,
			  double x1, double y1,
			  double x2, double y2,
			  double x3, double y3)
   {   
   double fab, fbc, fca;

   fab=(y-y1)*(x2-x1)-(x-x1)*(y2-y1);
   fbc=(y-y2)*(x3-x2)-(x-x2)*(y3-y2);

   if((fab<0.0 && fbc>0.0) || (fab>0.0 && fbc<0.0))
      return 0;

   fca=(y-y3)*(x1-x3)-(x-x3)*(y1-y3);
   if((fbc<0.0 && fca>0.0) || (fbc>0.0 && fca<0.0))
      return 0;

   return 1;
   }

static int muunnos(double p, double i, double *pp, double *ip,
		   const struct ykjetrsdata *data)
   {
   int n, f;
   const short *s;
   const double *a, *b, *c;

   f=(int)floor((p-data->alku_p)/data->ruutu_p);
   n=(int)floor((i-data->alku_i)/data->ruutu_i);

   if(f>=0 && n>=0 && f<data->index_p && n<data->index_i)
      for(s=data->index[f*data->index_i+n]; *s!=-1; s++)
	 {
	 a=data->piste[data->kolmio[*s][0]];
	 b=data->piste[data->kolmio[*s][1]];
	 c=data->piste[data->kolmio[*s][2]];

	 if(pistekolmiossa(p, i, a[0], a[1], b[0], b[1], c[0], c[1]))
	    {
	    a=data->args[*s];
	    *pp=a[0]*p+a[1]*i+a[2];
	    *ip=a[3]*p+a[4]*i+a[5];

	    return YKJETRS_OK;
	    }
	 }
   
   return 0;
   }

int ykj_etrstm35fin(double p, double i, double *pp, double *ip)
   {
   int t;

   if((t=muunnos(p, i, pp, ip, &ykjetrsdata)))
      return t;

   return ykj_etrstm35fin_bw(p, i, pp, ip)?YKJETRS_BW:0;
   }

int etrstm35fin_ykj(double p, double i, double *pp, double *ip)
   {
   int t;

   if((t=muunnos(p, i, pp, ip, &etrsykjdata)))
      return t;

   return etrstm35fin_ykj_bw(p, i, pp, ip)?YKJETRS_BW:0;
   }
