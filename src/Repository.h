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
// SaveBestWay()
//
int
SaveBestWay (int bestmove)
{
  int i;

  for (i = 0; Stack2[gameDepth + 1].BestWay[i] > 0; i++)
    {
      Stack2[gameDepth].BestWay[i + 1] = Stack2[gameDepth + 1].BestWay[i];
      if (i > 12)
        break;
    }
  Stack2[gameDepth].BestWay[0] = bestmove;

  Stack2[gameDepth].BestWay[i + 1] = 0;
  return 1;
}

//
// Set History
//
int inline SetHistory (const int move, const int& depth, const int& ahead,
                       const char& who, const int& valueCall, int flag)
{
  if (move == 0 || move == NO_BEST)
    return 1;

  int value = valueCall;

  int mv;

  int de;

  int mo;
  int SqTo = move & 255;
  int SqFrom = (move >> 8) & 255;
  int PieceMoved = (move >> 20) & 15;
  int PieceTaken = (move >> 16) & 15;
  int Indicators = (move >> 24) & 15;

  if (Indicators > 0)
    return 0;

  de = depth;
  if (de > ksup)
    de = ksup; // do not use a very long branches
  if (de < 0)
    de = 1;
  mo = move & 255;

  if (SqTo > 100 || SqFrom > 100 || PieceMoved == 0 || PieceMoved > 12
      || PieceTaken != 0 && PieceTaken > 12)
    return 1;

  mv = PieceMoved * 120 + SqTo;

  if (PieceTaken == 0)
    {
      if (PieceMoved < 7)
        {
          History.whiteHistory[mv] += de * de + 1;
          if (History.whiteHistory[mv] > 4096)
            History.whiteHistory[mv] -= History.whiteHistory[mv] / 8;
        }
      else
        {
          History.blackHistory[mv] += de * de + 1;
          if (History.blackHistory[mv] > 4096)
            History.blackHistory[mv] -= History.blackHistory[mv] / 8;
        }
    }
  if (PieceMoved < 7 && value > 10000)
    History.whiteMateHistory[mv] = value; // mates history
  if (PieceMoved >= 7 && value < -10000)
    History.blackHistory[mv] = value;

  /*
   ----------------------------------------------------------
  |                                                          |
  |   now, add the same move to the current killer moves if  |
  |   it is not already there, otherwise, increment the      |
  |   appropriate counter and sort if needed.                |
  |                                                          |
   ----------------------------------------------------------
  */

  if ((Stack2[gameDepth].PieceTaken != 0)
      || (Stack2[gameDepth].Indicators != 0))
    return 1;

  if (Stack2[gameDepth].killers_count[0] == 0)
    {
      Stack2[gameDepth].killers_count[0]++;
      Stack2[gameDepth].killers[0] = move;
      return 1;
    }
  else if (Stack2[gameDepth].killers[0] == move)
    Stack2[gameDepth].killers_count[0]++;
  else if (Stack2[gameDepth].killers[1] == move)
    {
      Stack2[gameDepth].killers_count[1]++;
      if (Stack2[gameDepth].killers_count[0]
          < Stack2[gameDepth].killers_count[1])
        {
          int temp = Stack2[gameDepth].killers_count[0];

          Stack2[gameDepth].killers_count[0]
              = Stack2[gameDepth].killers_count[1];

          Stack2[gameDepth].killers_count[1] = temp;

          int tempm = Stack2[gameDepth].killers[0];

          Stack2[gameDepth].killers[0] = Stack2[gameDepth].killers[1];

          Stack2[gameDepth].killers[1] = tempm;
        }
    }
  else
    {
      Stack2[gameDepth].killers_count[1] = 1;

      Stack2[gameDepth].killers[1] = move;
    }

  return 1;
} // end SaveHistory
