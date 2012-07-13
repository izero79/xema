/*
 * bursawolf.c
 *
 * Written by Ville Saari
 * Copyright 2003 National Land Survey of Finland
 *
 * Created: 2003-02-05
 * Updated: 2003-08-15
 *
 * Seitsemän parametrin yhdenmuotoisuusmuunnos ETRF89:n ja kkj:n välillä.
 */

#include <math.h>
#include "gausskrueger.h"
#include "ykjetrsdata-bw.h"

/*
 * Joitakin vakioja
 */

#define DEG       (M_PI/180.0) /* yksi aste radiaaneina */
#define Hayford_a 6378388.0    /* Hayfordin ellipsoidin isoakselin puolikas */
#define Hayford_f (1.0/297.0)  /* ja litistyneisyys */
#define GRS80_a   6378137.0    /* Samat GRS80-ellipsoidille */
#define GRS80_f   (1.0/298.257222101)

/*
 * Latitudin, longitudin ja korkeuden muuntaminen suorakulmaisiksi
 * koordinaateiksi ja takaisin.
 */
static void sphericaltoXYZ(double a, double f,
                           double phi, double lambda, double h,
                           double *X, double *Y, double *Z)
   {
   double sinphi=sin(phi), cosphi=cos(phi), e2=2.0*f-f*f, N;

   N=a/sqrt(1.0-e2*sinphi*sinphi);
   *X=(N+h)*cosphi*cos(lambda);
   *Y=(N+h)*cosphi*sin(lambda);
   *Z=(N*(1.0-e2)+h)*sinphi;
   }

static void XYZtospherical(double a, double f,
                           double X, double Y, double Z,
                           double *phi, double *lambda, double *h)
   {
   double e2=2.0*f-f*f, r=sqrt(X*X+Y*Y), sinphi, N;
   int n, t;

   *lambda=atan2(Y, X);
   *phi=atan(Z/((1.0-e2)*r));
   t=fabs(*phi)<M_PI/4.0;

   for(n=0; n<6; n++)
      {
      sinphi=sin(*phi);
      N=a/sqrt(1.0-e2*sinphi*sinphi);
      *h=t?r/cos(*phi)-N:Z/sinphi-(1.0-e2)*N;
      *phi=atan(Z/(r*(1.0-(e2*N/(N+*h)))));
      }
   }

/*
 * Ja sitten itse asiaan. Ensin muunnokset pallokoordinaatistossa.
 */

void etrs_kkj_bw_pallo(double fii, double lam, double *Fii, double *Lam)
   {
   double X, Y, Z, x, y, z;

   sphericaltoXYZ(GRS80_a, GRS80_f, fii, lam, 0.0, &x, &y, &z);

   X=m_xx*x+m_xy*y+m_xz*z+m_x;
   Y=m_yx*x+m_yy*y+m_yz*z+m_y;
   Z=m_zx*x+m_zy*y+m_zz*z+m_z;

   XYZtospherical(Hayford_a, Hayford_f, X, Y, Z, Fii, Lam, &z);
   }

void kkj_etrs_bw_pallo(double fii, double lam, double *Fii, double *Lam)
   {
   double X, Y, Z, x, y, z;

   /*
    * Muunnetaan kaksi kertaa. Ensimmäisestä yrityksestä hyödynnetään vain
    * korkeustieto. Alkuperäisen pisteen korkeudeksi pistetään muunnoksessa
    * saadun korkeuden vastaluku ja näin saatu piste muunnetaan uudestaan.
    * Näin saadaan lopullinen piste lähelle nollakorkeutta, jolloin
    * koordinaatistojen painovoimavektoreiden välinen poikkeama ei pääse
    * vääristämään pisteen tasopaikkaa.
    *
    * Tämä temppu pienentää edestakaisen muunnoksen poikkeaman
    * millimetriluokasta mikrometriluokkaan.
    */

   sphericaltoXYZ(Hayford_a, Hayford_f, fii, lam, 0.0, &x, &y, &z);

   X=r_xx*x+r_xy*y+r_xz*z+r_x;
   Y=r_yx*x+r_yy*y+r_yz*z+r_y;
   Z=r_zx*x+r_zy*y+r_zz*z+r_z;

   XYZtospherical(GRS80_a, GRS80_f, X, Y, Z, &X, &Y, &Z);
   sphericaltoXYZ(Hayford_a, Hayford_f, fii, lam, -Z, &x, &y, &z);

   X=r_xx*x+r_xy*y+r_xz*z+r_x;
   Y=r_yx*x+r_yy*y+r_yz*z+r_y;
   Z=r_zx*x+r_zy*y+r_zz*z+r_z;

   XYZtospherical(GRS80_a, GRS80_f, X, Y, Z, Fii, Lam, &z);
   }

/*
 * Ja lopuksi sama tasokoordinaatistossa.
 *
 * 29 000 kilometrin kaista riittänee kotitarpeisiin!
 * Se on yli 155 astetta leveä ja kattaa 97.7% maapallon
 * pinnasta. Kauemmaksi ei kannata mennä, sill Gauss-Krger-
 * projektiokaavojen virhe on tällä etäisyydellä samaa
 * suuruusluokkaa, kuin ETRF89:n ja kkj:n ero.
 */

#define HALFWIDTH 14500000.0

static struct gk_args kkj_gk;
static struct gk_args etrs_gk;
static int gkargs_ok=0;

static void init_gk(void)
   {
   if(gkargs_ok) return;

   gk_init(&etrs_gk, GRS80_a,   GRS80_f,   0.9996,  500000.0, 27.0*DEG);
   gk_init(&kkj_gk,  Hayford_a, Hayford_f, 1.0,    3500000.0, 27.0*DEG);

   gkargs_ok=1;
   }

int etrstm35fin_ykj_bw(double p, double i, double *P, double *I)
   {
   if(i<500000.0-0.9996*HALFWIDTH || i>500000.0+0.9996*HALFWIDTH)
      return 0;

   init_gk();

   gk_rectangulartospherical(&etrs_gk, p, i, &p, &i);
   etrs_kkj_bw_pallo(p, i, &p, &i);
   gk_sphericaltorectangular(&kkj_gk, p, i, P, I);

   return 1;
   }

int ykj_etrstm35fin_bw(double p, double i, double *P, double *I)
   {
   if(i<3500000.0-HALFWIDTH || i>3500000.0+HALFWIDTH)
      return 0;

   init_gk();

   gk_rectangulartospherical(&kkj_gk, p, i, &p, &i);
   kkj_etrs_bw_pallo(p, i, &p, &i);
   gk_sphericaltorectangular(&etrs_gk, p, i, P, I);

   return 1;
   }

