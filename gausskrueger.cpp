/*
 * gausskrueger.c
 *
 * Written by Ville Saari
 * Copyright 2003 National Land Survey of Finland
 *
 * Created: 2003-01-14
 * Updated: 2003-02-11
 *
 * Functions to convert spherical map coordinates (latitude, longitude) to
 * rectangular Gauss-Krüger projection coordinates (northing, easting) and
 * vice versa.
 *
 * See gausskrueger.h for documentation of the functions.
 */

#include <math.h>
#include <stdio.h>
#include "gausskrueger.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795029
#endif
/*
#ifndef _POSIX_SOURCE
static double asinh(double x) { return log(x+sqrt(x*x+1.0)); }
static double atanh(double x) { return 0.5*log((1.0+x)/(1.0-x)); }
#endif
*/
void gk_init(struct gk_args *args, double a, double f,
	     double k0, double E0, double lambda0)
   {
   double b, Dn, rn2, rn3, rn4;

   args->a=a;
   args->f=f;
   args->E0=E0;
   args->lambda0=lambda0;
        
   args->b=b=a*(1.0-f);
   args->e=sqrt(a*a-b*b)/a;

   Dn=(a-b)/(a+b);
   rn2=Dn*Dn;
   rn3=Dn*rn2;
   rn4=rn2*rn2;

   args->A1=k0*(a/(1.0+Dn))*(1.0+rn2/4.0+rn4/64.0);
   args->h1p=Dn/2.0-2.0*rn2/3.0+5.0*rn3/16.0+41.0*rn4/180.0;
   args->h2p=13.0*rn2/48.0-3.0*rn3/5.0+557.0*rn4/1440.0;
   args->h3p=61.0*rn3/240.0-103.0*rn4/140.0;
   args->h4p=49561.0*rn4/161280.0;
   args->h1=Dn/2.0-2.0*rn2/3.0+37.0*rn3/96.0-rn4/360.0;
   args->h2=rn2/48.0+rn3/15.0-437.0*rn4/1440.0;
   args->h3=17.0*rn3/480.0-37.0*rn4/840.0;
   args->h4=4397.0*rn4/161280.0;
   }

void gk_sphericaltorectangular(struct gk_args *args, double fii, double lambda,
			       double *N, double *E)
   {
   double t, ksi1, eta1;

   lambda-=args->lambda0;
   t=sinh(asinh(tan(fii))-args->e*atanh(args->e*sin(fii)));
   ksi1=atan(t/cos(lambda));
   eta1=atanh(sin(lambda)/sqrt(1.0+t*t));

   *N=args->A1*(ksi1+args->h1p*sin(2.0*ksi1)*cosh(2.0*eta1)+
		     args->h2p*sin(4.0*ksi1)*cosh(4.0*eta1)+
                     args->h3p*sin(6.0*ksi1)*cosh(6.0*eta1)+
                     args->h4p*sin(8.0*ksi1)*cosh(8.0*eta1));

   *E=args->A1*(eta1+args->h1p*cos(2.0*ksi1)*sinh(2.0*eta1)+
                     args->h2p*cos(4.0*ksi1)*sinh(4.0*eta1)+
                     args->h3p*cos(6.0*ksi1)*sinh(6.0*eta1)+
                     args->h4p*cos(8.0*ksi1)*sinh(8.0*eta1))+args->E0;
   }

void gk_rectangulartospherical(struct gk_args *args, double N, double E,
			       double *fii, double *lambda)
   {
   double ksi, eta, sksi, seta, q, t;
   int i;

   ksi=N/args->A1;
   eta=(E-args->E0)/args->A1;

   sksi=ksi-args->h1*sin(2.0*ksi)*cosh(2.0*eta)-
            args->h2*sin(4.0*ksi)*cosh(4.0*eta)-
            args->h3*sin(6.0*ksi)*cosh(6.0*eta)-
            args->h4*sin(8.0*ksi)*cosh(8.0*eta);

   seta=eta-args->h1*cos(2.0*ksi)*sinh(2.0*eta)-
            args->h2*cos(4.0*ksi)*sinh(4.0*eta)-
            args->h3*cos(6.0*ksi)*sinh(6.0*eta)-
            args->h4*cos(8.0*ksi)*sinh(8.0*eta);

   q=asinh(cos(*lambda=atan(sinh(seta)/cos(sksi)))*tan(sksi));
   t=sin(atan(sinh(1.0040685*q)));

   i=6;
   do {
      t=sinh(q+args->e*atanh(args->e*t));
      t/=sqrt(1.0+t*t);
      }
   while(--i);

   *fii=asin(t);
   *lambda+=args->lambda0;
   }

