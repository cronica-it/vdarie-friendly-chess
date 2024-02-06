/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

char name[50];

char str1[20];
char str2[20];

int
PutMoveString (int Move);

//
// SAVE GAME - Subroutine to save a played game into a file
//
int
SaveGame ()
{
  int m;

  sprintf (name, "SavedGames\\Game%2d .game", ksup, agresivityMode);
  FILE* f;
  f = fopen (name, "wt");

  fprintf (f, " \n\n Game played with Friendly Chess Program");
  fprintf (f,
           " \n\n Parameters:  Main Depth = %2d  Supplimenary Depth = %2d  "
           "Agresivity Mode = %2d ",
           ksup, limHigt - kmax, agresivityMode);
  if (WhiteIsMan == 0)
    strcpy (str1, "Computer");
  else
    strcpy (str1, "Player");

  if (BlackIsMan == 0)
    strcpy (str2, "Computer");
  else
    strcpy (str2, "Player");

  fprintf (f, "\n\n %s  vs  %s ", str1, str2);

  for (m = 0; m <= gameDepth; m++)
    {

      //	SqFrom = Stack2[m].SqFrom;
      //	SqTo   = Stack2[m].SqTo;
      //	PieceMoved = Stack2[m].PieceMoved;
      //	PieceTaken = Stack2[m].PieceTaken;

      PutMoveString (Stack2[m].move);

      fprintf (
          f,
          "\n\n %3d.  %s  Nodes = %10d  BestWayDepth = %2d  MaxDepth = %2d\n",
          m / 2 + 1, moveout, 0, Stack2[m].BestLineDepth, Stack2[m].MaxDepth);

      fprintf (f, "\n             ");

      if ((Stack2[m].flagsOfGame == (int)512))
        {
          fprintf (f, " REMIZA - PAT");
        }

      if (MoveToDoMoveValue > (MAT - 1000) && (MoveToDoMoveValue < MAT))
        {
          fprintf (f, " Albul da MAT in %d mutari \n",
                   (MAT - MoveToDoMoveValue) / 2);
        }

      if (MoveToDoMoveValue < (-MAT + 1000) && (MoveToDoMoveValue > -MAT))
        {
          fprintf (f, " Negrul da MAT in %d mutari \n",
                   (MAT + MoveToDoMoveValue) / 2);
        }
      /*
           for(n=0;Stack2[m].BestWay[n].SqFrom > 0;n++)
               {
                      SqFrom = BestWay[n].SqFrom;
                      SqTo   = BestWay[n].SqTo;
                      PieceMoved = BestWay[n].PieceMoved;
                      PieceTaken = BestWay[n].PieceTaken;

              PutMoveString( SqFrom, SqTo ,  PieceMoved, PieceTaken,
                       BestWay[n].Indicators) ;

                      fprintf(f, "  %s ", moveout); // print moves from best
         way

               }
      */
    }

  fclose (f);

  return 1;
}
