/*
 * Tämä tiedosto on generoitu koneellisesti laske7par (laske7par.c)
 * -ohjelmalla 2004-03-15 16:58:02.
 *
 * Bursa-Wolf-munnosparametrit matriisimuodossa ETRF89 -> kkj
 *
 * Matriisi on laskettu käyttäen todellisia rotaatiokaavoja, siis
 * ei pienten kulmien yksinkertaistuksella. Laskenta tapahtui 80 bitin
 * liukulukuaritmetiikalla, joten pyöristysvirheiden tuottama epätarkkuus
 * tuloksissa on vähemmän kuin DBL_EPSILON. Kokonaislukusuuruusluokkaa
 * olevissa kertoimissa siis viimeinenkin bitti on oikein.
 *
 * Lähtöarvot olivat:
 *
 * DeltaX =  96.0610 m    epsilonx =  4.80109 "    m = -1.49651 ppm
 * DeltaY =  82.4298 m    epsilony =  0.34546 "
 * DeltaZ = 121.7485 m    epsilonz = -1.37645 "
 */

#define m_xx 0.999998503466331524
#define m_xy -6.67316894121057108e-06
#define m_xz -1.67499016372273719e-06
#define m_x  96.061
#define m_yx 6.67320792703598055e-06
#define m_yy 0.999998503196840758
#define m_yz 2.32762951499466059e-05
#define m_y  82.4298
#define m_zx 1.67483483634938828e-06
#define m_zy -2.32763063269700585e-05
#define m_zz 0.999998503217703889
#define m_z  121.7485

/*
 * Ja sama toisinpäin
 *
 * Nämä parametrit taas on laskettu johtamalla edelliselle aito
 * käänteismatriisi. Näin saadut tulokset poikkeavat jotain millimetrin osia
 * julkaistuista parametreista, mutta näin edestakaisen muunnoksen virhe
 * saadaan merkittävästi pienemmäksi.
 */

#define r_xx 1.0000014964885711
#define r_xy 6.67322790012560591e-06
#define r_xz 1.6748398491748028e-06
#define r_x  -96.0618977362691679
#define r_yx -6.67318891418351003e-06
#define r_yy 1.00000149621907941
#define r_yz -2.3276375993576804e-05
#define r_y  -82.4264484359765328
#define r_zx -1.67499517701305066e-06
#define r_zy 2.32763648165199003e-05
#define r_zz 1.00000149623994252
#define r_z  -121.750439929353495

