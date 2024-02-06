/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

int
wpionx (int, int, int);
int
bpionx (int, int, int);
int
GenTrax (int, int, int);
int
GenTray (int, int, int);

extern int dk[9];
extern int dr[5];
extern int db[5];
extern int dn[9];

int
QuikBlack (int loc1, int loc2, int poz1, int poz2, int poz3, int poz4);
int
bpionx (int xloc1, int xloc2, int qcaz);
int
GenQuik (int poz1, int poz2, int poz3, int poz4);

int kpoz1, kpoz2, kpoz3, kpoz4;
int zpoz1, zpoz2, zpoz3, zpoz4;

int xqcaz;
int xp1, xp2, xt1, xt2, xw1, xw2, xw3;

//
// GenQuik - estimare rapida daca e mat in 2 mutari + GetMat4
//

int
GenQuik (int poz1, int poz2, int poz3, int poz4)
{

  int *p1, *p3;
  int q1;

  int spiece1;
  int spiece2;

  kpoz1 = poz1; // pozitia piesa1
  kpoz2 = poz2; // pozitia rege alb
  kpoz3 = poz3; // pozitia rege negru
  kpoz4 = poz4; // pozitia piesa2

  npiese = 4;

  if (piece2 == 0)
    {
      npiese = 3;
      kpoz4 = 0;
    }

  lwhite[0] = 6;
  lwhite[1] = poz2;
  lblack[0] = 12;
  lblack[1] = poz3;

  if ((piece2 > WK) || (piece2 == 0))
    { // one piece is white and, may be, another is black
      nwhite = 6;
      nblack = 6;
      if (piece2 == 0)
        nblack = 3;
      lwhite[3] = piece1;
      lwhite[3 + 1] = poz1;
      lwhite[nwhite] = 0;

      lblack[3] = piece2;
      lblack[3 + 1] = poz4;
      lblack[nblack] = 0;
    }
  else
    { // both pieces are white
      nwhite = 9;
      if (piece2 == 0)
        nwhite = 6;
      nblack = 3;
      lwhite[3] = piece1;
      lwhite[3 + 1] = poz1;
      lwhite[6] = piece2;
      lwhite[6 + 1] = poz4;
      lwhite[nwhite] = 0;
      lblack[nblack] = 0;
    }

  p1 = (int*)&lwhite[0];
  for (i = 0; *p1 != 0; p1 = p1 + 3)
    {
      piesa = *p1;
      loc1 = *(p1 + 1);
      xqcaz = 0;
      switch (piesa)
        {

        case WQ:
          p3 = (int*)&dk[0];
          goto eee2;
        case WB:
          p3 = (int*)&db[0];
          goto eee2;
        case WR:
          p3 = (int*)&dr[0];
        eee2:

          for (; *p3 != 0; p3++)
            {
              loc2 = loc1;
              for (;;)
                {
                  loc2 = loc2 + *p3;

                  if ((int)tabla0[loc2] < 0)
                    break; // e margine

                  if (loc2 == poz3)
                    return (int)MAT - 1;

                  if ((piece2 != 0) && (piece2 < BP) && (loc2 == poz4))
                    break; // stop at own piece

                  if ((loc2 == poz1) || (loc2 == poz2))
                    break;

                  q1 = 0;
                  if (loc2 == poz4 && piece2 >= BP)
                    {
                      spiece1 = piece1;
                      spiece2 = piece2;
                      piece2 = 0;
                      q1 = QuikBlack (
                          loc1, loc2, loc2, kpoz2, kpoz3,
                          0); // a black piece taken, continue wih 3 pieces
                      piece1 = spiece1;
                      piece2 = spiece2;
                    }
                  else if (loc2 != poz4)
                    q1 = QuikBlack (loc1, loc2, loc2, kpoz2, kpoz3,
                                    kpoz4); // continue with 4 pieces

                  if (((int)MAT - q1) == (2 * pas - 1))
                    return q1;

                  if ((loc2 == poz1) || (loc2 == poz3) || (loc2 == poz4))
                    break; // ok - direction stop condition

                } // end for a direction

            } // end for directions for a piece
          break;

        case WN:
          if (((int)KnightKing[loc1][poz3] & 1) == 1)
            return (int)MAT - 1;
          p3 = (int*)&dn[0];
          for (; *p3 != 0; p3++)
            {
              loc2 = loc1 + *p3;

              if ((int)tabla0[loc2] < 0)
                continue; // if edge of chess board

              if ((piece2 != 0) && (piece2 < BP) && (loc2 == poz4))
                continue; // own piece
              if ((loc2 == poz1) || (loc2 == poz2))
                continue;
              q1 = 0;
              if (loc2 == poz4 && piece2 >= BP)
                {
                  spiece1 = piece1;
                  spiece2 = piece2;
                  piece2 = 0;
                  q1 = QuikBlack (loc1, loc2, loc2, kpoz2, kpoz3, 0);
                  piece1 = spiece1;
                  piece2 = spiece2;
                }
              else if (loc2 != poz4)
                q1 = QuikBlack (loc1, loc2, loc2, kpoz2, kpoz3, kpoz4);

              if (((int)MAT - q1) == (2 * pas - 1))
                return q1;

            } // end for
          break;

        case WK:

          if ((int)DistKing[loc1][poz3] == 1)
            return (int)MAT - 1; // advers king is taken

          p3 = (int*)&dk[0];
          for (; *p3 != 0; p3++)
            {
              loc2 = loc1 + *p3;

              if ((int)tabla0[loc2] < 0)
                continue; // e margine

              if ((piece2 != 0) && (piece2 < BP) && (loc2 == poz4))
                continue;

              if (loc2 == poz1)
                continue;
              q1 = 0;
              if (loc2 == poz4 && piece2 >= BP)
                {
                  spiece1 = piece1;
                  spiece2 = piece2;
                  piece2 = 0;
                  q1 = QuikBlack (loc1, loc2, kpoz1, loc2, kpoz3,
                                  0); // black piece taken, continue 3 pieces
                  piece1 = spiece1;
                  piece2 = spiece2;
                }
              else if (loc2 != poz4)
                q1 = QuikBlack (loc1, loc2, kpoz1, loc2, kpoz3,
                                kpoz4); // continue with 4 pieces

              if (((int)MAT - q1) == (2 * pas - 1))
                return q1;

            } // end for
          break;

        case WP:
          q1 = 0;
          if (((loc1 + 9) == poz3) || ((loc1 + 11) == poz3))
            return (int)MAT - 1;

          if (((loc1 + 10) != poz1) && ((loc1 + 10) != poz2)
              && ((loc1 + 10) != poz3) && ((loc1 + 10) != poz4))
            {

              q1 = wpionx (loc1, loc1 + 10, 1);
              if (((int)MAT - q1) == (2 * pas - 1))
                return q1;

              if ((loc1 < 40) && ((loc1 + 20) != poz1) && ((loc1 + 20) != poz2)
                  && ((loc1 + 20) != poz3) && ((loc1 + 20) != poz4))
                {
                  q1 = wpionx (loc1, loc1 + 20, 0);
                  if (((int)MAT - q1) == (2 * pas - 1))
                    return q1;
                }
            }
          // take a black piece by white pawn
          if ((piece2 > WK) && ((loc1 + 9) == poz4))
            {
              q1 = wpionx (loc1, loc1 + 9, 1); //  here will be tested transf.
              if (((int)MAT - q1) == (2 * pas - 1))
                return q1;
            }

          if ((piece2 > WK) && ((loc1 + 11) == poz4))
            {
              q1 = wpionx (loc1, loc1 + 11, 1); // here will be tested transf.
              if (((int)MAT - q1) == (2 * pas - 1))
                return q1;
            }
          // test en-passant
          if ((piece2 == BP) && ((loc1 - 1) == poz4) && (loc1 > 60)
              && (loc1 < 70))
            {
              q1 = wpionx (loc1, loc1 + 9,
                           3); //  here will be tested en passant
              if (((int)MAT - q1) == (2 * pas - 1))
                return q1;
            }
          if ((piece2 == BP) && ((loc1 + 1) == poz4) && (loc1 > 60)
              && (loc1 < 70))
            {
              q1 = wpionx (loc1, loc1 + 11,
                           3); //  here will be tested en passant
              if (((int)MAT - q1) == (2 * pas - 1))
                return q1;
            }
          break;
        }
    }
  return -1;
}

//
//  Mutare de pion alb
//
int
wpionx (int xloc1, int xloc2, int qcaz)
{
  int iix;

  xqcaz = 0;

  if (tabla0[xloc2] < 0)
    return 0;

  if (xloc2 > 90)
    {
      iix = GenTrax (xloc1, xloc2, WQ);
      if (((int)MAT - iix) == (2 * pas - 1))
        return iix;
      iix = GenTrax (xloc1, xloc2, WR);
      if (((int)MAT - iix) == (2 * pas - 1))
        return iix;
      iix = GenTrax (xloc1, xloc2, WB);
      if (((int)MAT - iix) == (2 * pas - 1))
        return iix;
      iix = GenTrax (xloc1, xloc2, WN);
      if (((int)MAT - iix) == (2 * pas - 1))
        return iix;
      return 0;
    }
  else
    {
      iix = GenTrax (xloc1, xloc2, WP);
      if (((int)MAT - iix) == (2 * pas - 1))
        return iix;
      return 0;
    }
}

//
//  G e n T r a x - Generare transformare sau mutare
//
int
GenTrax (int yloc1, int yloc2, int pi2)
{
  int q1;

  int piece1sav;
  int tx, tx1;

  int spiece1;
  int spiece2;

  if (pi2 > WP)
    {
      piece1sav = piece1;
      piece1 = pi2;
      tx = tabla[yloc1];
      tabla[yloc1] = pi2;
      tx1 = lwhite[3];
      lwhite[3] = pi2;

      if (yloc2 == kpoz4)
        {
          spiece1 = piece1;
          spiece2 = piece2;
          piece1 = pi2;
          piece2 = 0;
          q1 = QuikBlack (yloc1, yloc2, yloc2, kpoz2, kpoz3,
                          0); // transform with take a black piece
          piece1 = spiece1;
          piece2 = spiece2;
        }
      else
        {
          spiece1 = piece1;
          spiece2 = piece2;
          piece1 = pi2;
          q1 = QuikBlack (yloc1, yloc2, yloc2, kpoz2, kpoz3, kpoz4);
          piece1 = spiece1;
          piece2 = spiece2;
        }

      piece1 = piece1sav;
      tabla[yloc1] = tx;
      lwhite[3] = tx1;

      if (((int)MAT - q1) == (2 * pas - 1))
        return q1;
    }
  else
    {
      if (yloc2 == kpoz4)
        {
          spiece1 = piece1;
          spiece2 = piece2;
          piece1 = pi2;
          piece2 = 0;
          q1 = QuikBlack (yloc1, yloc2, yloc2, kpoz2, kpoz3, 0);
          piece1 = spiece1;
          piece2 = spiece2;
        }
      else
        {
          spiece1 = piece1;
          spiece2 = piece2;
          piece1 = pi2;
          q1 = QuikBlack (yloc1, yloc2, yloc2, kpoz2, kpoz3, kpoz4);
          piece1 = spiece1;
          piece2 = spiece2;
        }

      if (((int)MAT - q1) == (2 * pas - 1))
        return q1;
    }

  return 0; // mutare pusa in lista si nu e "cut"
} // end GenTrax

//
// Test rapid mat ca raspuns la mutare alb
//
// white move from, white move to, position piece1, position white king,
// position black king, position piece2
int
QuikBlack (int xloc1, int xloc2, int xpoz1, int xpoz2, int xpoz3, int xpoz4)
{

  int minblack;

  int *p1, *p3;
  int apiesa, aloc1, aloc2;

  int spiece1;
  int spiece2;

  // Obs: If white take a black piece, only the black king can re-take !!!!!

  if (pas == 0)
    return 0;

  spiece1 = piece1; // save piece1 piece2 because may change in this function
  spiece2 = piece2;

  zpoz1 = xpoz1;
  zpoz2 = xpoz2;
  zpoz3 = xpoz3;
  zpoz4 = xpoz4;

  if ((xloc1 == xpoz1) && (xloc2 == xpoz4)) // white piece1 takes black piece2
    {
      piece2 = 0;
    }
  if ((xloc1 == xpoz2) && (xloc2 == xpoz4)) // white king take piece2
    {
      piece2 = 0;
    }

  minblack = (int)MAT;

  p1 = (int*)&lblack[0];
  for (i = 0; *p1 != 0; p1 = p1 + 3)
    {
      apiesa = *p1;
      aloc1 = *(p1 + 1);

      if (aloc1 == xloc2)
        {
          // if black piece to move from aloc1 is taken in xloc2, it cannot
          // move wait black king moves to see that mate info can be obtain
          // from repository
          continue;
        }

      switch (apiesa)
        {

        case BQ:
          p3 = (int*)&dk[0];
          goto eee2;
        case BB:
          p3 = (int*)&db[0];
          goto eee2;
        case BR:
          p3 = (int*)&dr[0];
        eee2:
          for (; *p3 != 0; p3++)
            {
              aloc2 = aloc1;
              for (;;)
                {
                  aloc2 = aloc2 + *p3;

                  if ((int)tabla0[aloc2] < 0)
                    break; // e margine

                  if (aloc2 == zpoz2)
                    return -(int)MAT; // black take white king

                  if (aloc2 == zpoz1)
                    return 0; // black taking a piece   -> no mate info

                  q = AccessMat4 (0, zpoz1, zpoz2, zpoz3, aloc2);

                  if ((q == 0) || (q > (2 * pas - 1)))
                    return 0;

                  if (((int)MAT - q) < minblack)
                    minblack = (int)MAT - q;

                } // end for pe o directie

            } // end for directii o piesa
          break;

        case BN:
          p3 = (int*)&dn[0];
          for (; *p3 != 0; p3++)
            {
              aloc2 = aloc1 + *p3;

              if ((int)tabla0[aloc2] < 0)
                continue; // edge

              if (aloc2 == zpoz2)
                return -(int)MAT; // black piece take white king

              if (aloc2 == zpoz1)
                return 0; // noking black piece take white piece -> no mate
                          // info

              q = AccessMat4 (0, zpoz1, zpoz2, zpoz3, aloc2);

              if ((q == 0) || (q > (2 * pas - 1)))
                return 0;

              if (((int)MAT - q) < minblack)
                minblack = (int)MAT - q;

            } // end for
          break;

        case BK:
          p3 = (int*)&dk[0];
          for (; *p3 != 0; p3++)
            {
              aloc2 = aloc1 + *p3;

              if ((int)tabla0[aloc2] < 0)
                continue; // edge

              if (aloc2 == zpoz2)
                return -MAT; // black king take the white king

              // kings clash testing
              if ((int)DistKing[aloc2][zpoz2]
                  == 1) // very necesary to stalemate detection
                {
                  if (((int)MAT - 1) < minblack)
                    minblack = (int)MAT - 1;
                  continue;
                }

              // case where piece2 isnt more because white take a black piece2
              // (or it was no piece2)
              if (zpoz4 == 0)
                {
                  if (aloc2 == zpoz1)
                    return 0; // black king take the last white piece

                  q = 0;
                  // there is white piece1 on table
                  switch (piece1) // only piece1 now on chess board and kings
                                  // are not clash

                    {
                    case WP:
                      q = AccessMat4 (0, zpoz1, zpoz2, aloc2,
                                      0); // info mate with 3 pieces
                      break;

                    case WQ:
                      q = AccessMat4 (0, zpoz1, zpoz2, aloc2, 0);
                      break;
                    case WR:
                      q = AccessMat4 (0, zpoz1, zpoz2, aloc2, 0);
                      break;
                    }
                  // obs : knight or bishop cannot mate single
                  if ((q == 0) || (q > (2 * pas - 1)))
                    return 0;

                  if (((int)MAT - q) < minblack)
                    minblack = (int)MAT - q;

                  // piesa negru rege ia #rege ; se intra in mat cu 3 piese
                  continue;
                }

              // zpoz4 != 0 now
              // case piece2 is white  and black king take piece2
              // -> mate with 3 pieces
              if ((aloc2 == zpoz4) && (piece2 < BP))
                {
                  q = 0;

                  switch (piece1) // switch with remaining piece1
                    {

                    case WP:
                      q = AccessMat4 (0, zpoz1, zpoz2, aloc2, 0);
                      break;

                    case WQ:
                      q = AccessMat4 (0, zpoz1, zpoz2, aloc2, 0);
                      break;
                    case WR:
                      q = AccessMat4 (0, zpoz1, zpoz2, aloc2, 0);
                      break;
                    }
                  if ((q == 0) || (q > (2 * pas - 1)))
                    return 0;

                  if (((int)MAT - q) < minblack)
                    minblack = (int)MAT - q;

                  continue;
                }

              // case piece2 exists, is white  and black king take piece1
              // -> mate with 3 pieces
              if ((aloc2 == zpoz1) && (piece2 < BP) && (piece2 > 0)
                  && (zpoz4 > 0))
                {
                  q = 0;

                  int savepiece1 = piece1;
                  piece1 = piece2;
                  piece2 = 0;
                  switch (piece2) // switch with remaining piece
                    {

                    case WP:
                      q = AccessMat4 (0, zpoz4, zpoz2, aloc2, 0);
                      break;

                    case WQ:
                      q = AccessMat4 (0, zpoz4, zpoz2, aloc2, 0);
                      break;
                    case WR:
                      q = AccessMat4 (0, zpoz4, zpoz2, aloc2, 0);
                      break;
                    }
                  piece2 = piece1;
                  piece1 = savepiece1;

                  if ((q == 0) || (q > (2 * pas - 1)))
                    return 0;

                  if (((int)MAT - q) < minblack)
                    minblack = (int)MAT - q;

                  continue;
                }

              // white doesnt take black piece2  but black king take piece1
              if ((xloc2 != zpoz4) && (piece2 > WK) && (aloc2 == zpoz1))
                continue; // no mate info

              // all 4 pieces alive, then test mate info with 4 pieces
              if ((xloc2 != zpoz4) && (piece2 > 0) && (aloc2 != zpoz1)
                  && (aloc2 != zpoz4))
                {
                  q = AccessMat4 (0, zpoz1, zpoz2, aloc2, zpoz4);

                  if ((q == 0) || (q > (2 * pas - 1)))
                    return 0;

                  if (((int)MAT - q) < minblack)
                    minblack = (int)MAT - q;
                }

            } // end for
          break;

        case BP:

          // only xpoz4 for black pawn; if pawn take something -> cannot have
          // mate info
          if (((zpoz4 - 9) == zpoz2) || ((zpoz4 - 11) == zpoz2))
            return -(int)MAT + 1;

          if (((zpoz4 - 10) != zpoz1) && ((zpoz4 - 10) != zpoz2)
              && ((loc1 - 10) != zpoz3) && ((loc1 - 10) != zpoz4))
            {
              // if black pawn move forward cant take any piece
              // only when black pawn go forward we can have mate info !!!!!
              q = bpionx (zpoz4, zpoz4 - 10, 0);

              if ((q == 0) || (q > (2 * pas - 1)))
                return 0;

              if (((int)MAT - q) < minblack)
                minblack = (int)MAT - q;

              if ((zpoz4 > 80) && ((zpoz4 - 20) != zpoz1)
                  && ((zpoz4 - 20) != zpoz2) && ((loc1 - 20) != zpoz3))
                {
                  q = bpionx (zpoz4, zpoz4 - 20, 0);
                  if ((q == 0) || (q > (2 * pas - 1)))
                    return 0;

                  if (((int)MAT - q) < minblack)
                    minblack = (int)MAT - q;
                }
            }

          // the other case degenerate white position, cannot have white mate
          break;
        } // end switch

    } // end for
  // finaly, test if black is in check to declare mate position, when black
  // king cannot move
  if (minblack == (MAT - 1))
    {
      xt1 = tabla[xloc1];
      xt2 = tabla[xloc2];

      xw1 = lwhite[1];
      xw2 = lwhite[3 + 1];
      xw3 = lwhite[6 + 1];

      tabla[xloc2] = xt1;
      tabla[xloc1] = 0;

      if (lwhite[1] == xloc1)
        lwhite[1] = xloc2;

      if (lwhite[3 + 1] == xloc1)
        lwhite[3 + 1] = xloc2;

      if (lwhite[6 + 1] == xloc1)
        lwhite[6 + 1] = xloc2;

      if (TablaBlackIsCheck (zpoz3) == 0)
        minblack = 0; // stalemate

      tabla[xloc1] = xt1;
      tabla[xloc2] = xt2;

      lwhite[1] = xw1;
      lwhite[3 + 1] = xw2;
      lwhite[6 + 1] = xw3;
    }

  piece1 = spiece1;
  piece2 = spiece2;

  return minblack;
}

//
//  Mutare de pion negru
//
int
bpionx (int xloc1, int xloc2, int qcaz)
{
  int iix;

  if (tabla0[xloc2] < 0)
    return 0;

  if (xloc2 < 30)
    {
      iix = GenTray (xloc1, xloc2, BQ);
      if (iix > 0)
        return iix;
      iix = GenTray (xloc1, xloc2, BR);
      if (iix > 0)
        return iix;
      iix = GenTray (xloc1, xloc2, BB);
      if (iix > 0)
        return iix;
      iix = GenTray (xloc1, xloc2, BN);
      return iix;
    }
  else
    {
      iix = GenTray (xloc1, xloc2, piece2);
      return iix;
    }
}

//
//  G e n T r a y - Generare transformare sau mutare pion negru
//
int
GenTray (int yloc1, int yloc2, int pi2)
{
  int q, piece1sav;

  if ((yloc2 == zpoz2) || (yloc2 == zpoz3) || (yloc2 == zpoz4))
    return 0; // if black PAWN take a king or the white last piece - no mate
              // info

  if (pi2 != 0)
    {
      // transformare - if only black has piece2, a pawn
      piece1sav = piece1;
      piece1 = pi2;
      q = AccessMat4 (0, zpoz1, zpoz2, zpoz3, yloc2);
      piece1 = piece1sav;
      if (((int)MAT - q) == (2 * pas - 1))
        return q;
    }
  else
    {
      // mutare care nu e transformare
      q = AccessMat4 (0, zpoz1, zpoz2, zpoz3, yloc2);
      if (((int)MAT - q) == (2 * pas - 1))
        return q;
    }

  return 0;
} // end GenTray
