/*
 * gausskrueger.h
 *
 * Written by Ville Saari
 * Copyright 2003 National Land Survey of Finland
 *
 * Created: 2003-01-14
 * Updated: 2003-01-15
 */

struct gk_args
   {
   double E0, lambda0, a, f, b, e, A1, h1p, h2p, h3p, h4p, h1, h2, h3, h4;
   };

/*
 * Initialize the gk_args structure
 */

void gk_init(
   struct gk_args *args, /* pointer to uninitialized gk_args  */
   double a,             /* semimajor axis of the ellipsoid   */
   double f,             /* flattening of the ellipsoid       */
   double k0,            /* scale factor at prime meridian    */
   double E0,            /* false easting                     */
   double lambda0);      /* prime meridian                    */

/*
 * Convert spherical coordinates to rectangular coordinates
 */

void gk_sphericaltorectangular(
   struct gk_args *args, /* gk_args initialized by gk_init()  */
   double phi,           /* latitude                          */
   double lambda,        /* longitude                         */
   double *N,            /* pointer to northing               */
   double *E);           /* pointer to easting                */

/*
 * Convert rectangular coordinates to spherical coordinates
 */

void gk_rectangulartospherical(
   struct gk_args *args, /* gk_args initialized by gk_init()  */
   double N,             /* northing                          */
   double E,             /* easting                           */
   double *phi,          /* pointer to latitude               */
   double *lambda);      /* pointer to longitude              */
