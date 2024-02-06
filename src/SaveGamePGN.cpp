/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#include "stdafx.h"
#include "Structures.h"

void
MoveToString (int, char[]);

extern struct StackElement2 Stack2[];

extern int MoveNumber;

void
SaveGamePGN ()
{
  int numberFrom;
  int mv, xrow;
  char whitemove[20];
  char blackmove[20];
  int n;
  int n1;
  char movesstring[30];

  int xa, ya, xb, yb;

  for (n = 0; n < 24; n++)
    movesstring[n] = ' '; // make clear the string

  numberFrom = 0;
  numberFrom = numberFrom - (numberFrom & 1);

  FILE* ffff;
  ffff = fopen ("PartidaSalvata.lis", "wt");

  if (numberFrom < 0)
    numberFrom = 0;
  xrow = 1;
  for (n = 0; n <= (MoveNumber - 2); n = n + 2)
    {
      strcpy (whitemove, "          ");
      strcpy (blackmove, "          ");

      mv = n / 2 + 1;
      MoveToString (Stack2[n].move, whitemove);
      if (n <= (MoveNumber - 3))
        MoveToString (Stack2[n + 1].move, blackmove);
      sprintf (movesstring, " %3d. %s    ", mv, whitemove);
      fprintf (ffff, "%s %s \n", movesstring, blackmove);

      sprintf (movesstring, "%s %s", Stack2[n].info, Stack2[n + 1].info);
      fprintf (ffff, "%s \n", movesstring);
      xrow = xrow + 2;
      ;
    }
  return;
}
