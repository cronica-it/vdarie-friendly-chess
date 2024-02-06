/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

//
// Test if BLACK attacked on square Sq
//
int inline TablaBlackIsCheck (int Sq)
{
  int i;

  if (tabla[Sq - 9] == WP)
    return Sq - 9;
  if (tabla[Sq - 11] == WP)
    return Sq - 11;

  for (i = 0; i < 8; i++)
    if (tabla[Sq + dn[i]] == WN)
      return Sq + dn[i];

  int Slide = Sq + 1;
  while (tabla[Slide] == 0)
    Slide += 1;
  if (tabla[Slide] == WR || tabla[Slide] == WQ)
    return Slide;

  Slide = Sq - 1;
  while (tabla[Slide] == 0)
    Slide -= 1;
  if (tabla[Slide] == WR || tabla[Slide] == WQ)
    return Slide;

  Slide = Sq + 10;
  while (tabla[Slide] == 0)
    Slide += 10;
  if (tabla[Slide] == WR || tabla[Slide] == WQ)
    return Slide;

  Slide = Sq - 10;
  while (tabla[Slide] == 0)
    Slide -= 10;
  if (tabla[Slide] == WR || tabla[Slide] == WQ)
    return Slide;

  Slide = Sq + 9;
  while (tabla[Slide] == 0)
    Slide += 9;
  if (tabla[Slide] == WB || tabla[Slide] == WQ)
    return Slide;

  Slide = Sq - 9;
  while (tabla[Slide] == 0)
    Slide -= 9;
  if (tabla[Slide] == WB || tabla[Slide] == WQ)
    return Slide;

  Slide = Sq + 11;
  while (tabla[Slide] == 0)
    Slide += 11;
  if (tabla[Slide] == WB || tabla[Slide] == WQ)
    return Slide;

  Slide = Sq - 11;
  while (tabla[Slide] == 0)
    Slide -= 11;
  if (tabla[Slide] == WB || tabla[Slide] == WQ)
    return Slide;

  //	for (int i = 0; i < 8; i++) if (tabla[Sq + dk[i]] == WK) return Sq +
  //dk[i];

  if (tabla[Sq - 11] == WK)
    return Sq - 11;
  if (tabla[Sq - 10] == WK)
    return Sq - 10;
  if (tabla[Sq - 9] == WK)
    return Sq - 9;
  if (tabla[Sq - 1] == WK)
    return Sq - 1;
  if (tabla[Sq + 1] == WK)
    return Sq + 1;
  if (tabla[Sq + 9] == WK)
    return Sq + 9;
  if (tabla[Sq + 10] == WK)
    return Sq + 10;
  if (tabla[Sq + 11] == WK)
    return Sq + 11;

  return 0;
}

//
// Test if WHITE attacked on square Sq
//

int inline TablaWhiteIsCheck (int Sq)
{
  int i;

  if (tabla[Sq + 9] == BP)
    return Sq + 9;
  if (tabla[Sq + 11] == BP)
    return Sq + 11;

  for (i = 0; i < 8; i++)
    if (tabla[Sq + dn[i]] == BN)
      return Sq + dn[i];

  int Slide = Sq + 1;
  while (tabla[Slide] == 0)
    Slide += 1;
  if (tabla[Slide] == BR || tabla[Slide] == BQ)
    return Slide;

  Slide = Sq - 1;
  while (tabla[Slide] == 0)
    Slide -= 1;
  if (tabla[Slide] == BR || tabla[Slide] == BQ)
    return Slide;

  Slide = Sq + 10;
  while (tabla[Slide] == 0)
    Slide += 10;
  if (tabla[Slide] == BR || tabla[Slide] == BQ)
    return Slide;

  Slide = Sq - 10;
  while (tabla[Slide] == 0)
    Slide -= 10;
  if (tabla[Slide] == BR || tabla[Slide] == BQ)
    return Slide;

  Slide = Sq + 9;
  while (tabla[Slide] == 0)
    Slide += 9;
  if (tabla[Slide] == BB || tabla[Slide] == BQ)
    return Slide;

  Slide = Sq - 9;
  while (tabla[Slide] == 0)
    Slide -= 9;
  if (tabla[Slide] == BB || tabla[Slide] == BQ)
    return Slide;

  Slide = Sq + 11;
  while (tabla[Slide] == 0)
    Slide += 11;
  if (tabla[Slide] == BB || tabla[Slide] == BQ)
    return Slide;

  Slide = Sq - 11;
  while (tabla[Slide] == 0)
    Slide -= 11;
  if (tabla[Slide] == BB || tabla[Slide] == BQ)
    return Slide;

  if (tabla[Sq - 11] == BK)
    return Sq - 11;
  if (tabla[Sq - 10] == BK)
    return Sq - 10;
  if (tabla[Sq - 9] == BK)
    return Sq - 9;
  if (tabla[Sq - 1] == BK)
    return Sq - 1;
  if (tabla[Sq + 1] == BK)
    return Sq + 1;
  if (tabla[Sq + 9] == BK)
    return Sq + 9;
  if (tabla[Sq + 10] == BK)
    return Sq + 10;
  if (tabla[Sq + 11] == BK)
    return Sq + 11;

  return 0;
}

//
// Test if generated move is CHECK
//

int
MoveIsCheck ()
{
  int kingSquare;

  int bb1;

  int SqTo;
  int SqFrom;
  int PieceTaken;
  int PieceMoved;

  SqFrom = (MoveToDo >> 8) & 255;

  SqTo = MoveToDo & 255;

  PieceMoved = (MoveToDo >> 20) & 15;

  PieceTaken = (MoveToDo >> 16) & 15;

  if (who == WHITE)
    {

      kingSquare = Stack[gameDepth].blackKingSquare;

      bb1 = TablaBlackIsCheck (kingSquare); // test illegal

      if (bb1 > 0)
        {
          return -1;
        }
    }
  else
    {
      kingSquare = Stack[gameDepth].whiteKingSquare;

      bb1 = TablaWhiteIsCheck (kingSquare); // test illegal
      if (bb1 > 0)
        {
          return -1;
        }
    }
  return 0;
}

//
// Set King King distances structure
//

int
SetKingKing ()
{
  int i1, i2, j1, j2, v, v1, v2, v3;

  for (i = 0; i < 120; i++)
    for (j = 0; j < 120; j++)
      {
        KingKing[i][j] = (char)0;
        if ((tabla0[i] < 0) || (tabla0[j] < 0))
          continue;
        i1 = i / 10;
        i2 = i % 10;
        j1 = j / 10;
        j2 = j % 10;
        v = KingCenter[i] - KingCenter[j];
        v1 = i1 - j1;
        v2 = i2 - j2;
        if (v1 < 0)
          v1 = -v1;
        if (v2 < 0)
          v2 = -v2;
        v3 = v1;
        if (v2 > v3)
          v3 = v2;
        DistKing[i][j] = (char)v3; // distanta tip rege
        v = 2 * v - 4 * v1 - 4 * v2 - 8 * v3;
        if ((v1 <= 1) && (v2 <= 1))
          v = -127;
        if ((v1 == 0) && (v2 == 0))
          v = -128;
        KingKing[i][j] = (char)v; // pondere tip rege-rege
      }
  return 0;
}

//
// Set King Queen attack structure
//
int
SetQueenKing ()
{
  int i1, i2, j1, j2, v, v1, v2;
  int isah;

  for (i = 0; i < 120; i++)
    for (j = 0; j < 120; j++)
      {
        QueenKing[i][j] = (char)0;
        if ((tabla0[i] < 0) || (tabla0[j] < 0))
          continue;
        i1 = i / 10;
        i2 = i % 10;
        j1 = j / 10;
        j2 = j % 10;
        v = KingCenter[i] - KingCenter[j];
        v1 = i1 - j1;
        v2 = i2 - j2;
        if (v1 < 0)
          v1 = -v1;
        if (v2 < 0)
          v2 = -v2;
        v = 2 * v - 2 * v1 - 2 * v2;
        isah = 0;
        if ((v1 == 0) || (v2 == 0) || (v1 == v2))
          isah = 1;
        //  v = v + isah;
        v = isah;
        for (;;)
          {
            if ((v1 == 0) && (v2 == 0))
              {
                v = 1;
                break;
              }

            if ((v1 <= 1) && (v2 <= 1))
              {
                v = 1;
                break;
              }
            break;
          }
        QueenKing[i][j] = (char)v;
      }
  return 0;
}

//
// Set King Rook attack structure
//
int
SetRookKing ()
{
  int i1, i2, j1, j2, v, v1, v2;
  int v3;

  for (i = 0; i < 120; i++)
    for (j = 0; j < 120; j++)
      {
        RookKing[i][j] = (char)0;
        if ((tabla0[i] < 0) || (tabla0[j] < 0))
          continue;
        i1 = i / 10;
        i2 = i % 10;
        j1 = j / 10;
        j2 = j % 10;
        v = 0;
        v1 = i1 - j1;
        v2 = i2 - j2;
        if (v1 < 0)
          v1 = -v1;
        if (v2 < 0)
          v2 = -v2;
        v3 = v1;
        if (v2 < v3)
          v3 = v2;
        RookKing[i][j] = (char)v3;
      }
  return 0;
}
//
// Set King Bishop attack structure
//
int
SetBishopKing ()
{
  int i1, i2, j1, j2, v, v1, v2;
  int isah;

  for (i = 0; i < 120; i++)
    for (j = 0; j < 120; j++)
      {
        BishopKing[i][j] = (char)0;
        if ((tabla0[i] < 0) || (tabla0[j] < 0))
          continue;
        i1 = i / 10;
        i2 = i % 10;
        j1 = j / 10;
        j2 = j % 10;
        v = -KingCenter[j];
        v1 = i1 - j1;
        v2 = i2 - j2;
        if (v1 < 0)
          v1 = -v1;
        if (v2 < 0)
          v2 = -v2;
        v = 2 * v - 2 * v1 - 2 * v2;
        isah = 0;
        if (v1 == v2)
          isah = 1;
        v = v + isah;
        for (;;)
          {
            if ((v1 == 0) && (v2 == 0))
              {
                v = -128;
                break;
              }

            if ((v1 <= 1) && (v2 <= 1) && (v1 == v2))
              {
                v = -127;
                break;
              }
            break;
          }
        BishopKing[i][j] = (char)v;
      }
  return 0;
}
//
// Set King Knight attack structure
//
int
SetKnightKing ()
{
  int i1, i2, j1, j2, v, v1, v2;

  for (i = 0; i < 120; i++)
    for (j = 0; j < 120; j++)
      {
        KnightKing[i][j] = (char)0;
        if ((tabla0[i] < 0) || (tabla0[j] < 0))
          continue;
        i1 = i / 10;
        i2 = i % 10;
        j1 = j / 10;
        j2 = j % 10;
        v = KingCenter[i] - KingCenter[j];
        v1 = i1 - j1;
        v2 = i2 - j2;
        if (v1 < 0)
          v1 = -v1;
        if (v2 < 0)
          v2 = -v2;
        v = 2 * v - 2 * v1 - 2 * v2;
        if (((v1 == 1) && (v2 == 2)) || ((v1 == 2) && (v2 == 1)))
          v = -127; // sah cu calul
        KnightKing[i][j] = (char)v;
      }
  return 0;
}

//
// DirKing - Set Queen Directions
//
int
SetDirKing2 ()
{
  int i;
  int j;
  int dir;
  int sq;

  for (i = 0; i < 120; i++)
    for (j = 0; j < 120; j++)
      DirKing[i][j] = 0;

  for (i = 21; i < 99; i++)
    {
      if (tabla0[i] < 0)
        continue;
      for (j = 0; j < 8; j++)
        {
          dir = dk[j];
          sq = i;
          for (;;)
            {
              sq = sq + dir;
              if (tabla0[sq] < 0)
                break;
              DirKing[i][sq] = dir;
            }
        }
    }
  return 0;
}

//
// Setare TabCheck pentru pretabelare valori Checksum de pozitii
int
SetTabCheck ()
{
  int Piece, Sq;
  for (Piece = 0; Piece < 13; Piece++)
    {
      for (Sq = 0; Sq < 120; Sq++)
        {
          TabCheck[Piece][Sq]
              = Sq * 2997 * (Piece + 19) + (101 - Sq) * Sq * tso[Piece] * 99
                + (Sq + Piece) * 171; // compute checkSum of position needed to
                                      // detect repetition

          TabCheck2[Piece][Sq]
              = 19 + Sq * 797 * (Piece + 7) + (101 - Sq) * Sq * tso[Piece] * 99
                + (Sq + Piece) * 197; // compute checkSum of position needed to
                                      // detect repetition

          //			pstackxx->checkSumPosition2 -= SqFrom *
          //PieceMoved + (SqFrom - PieceMoved) * SqFrom;
        }
    }
  return 0;
}
