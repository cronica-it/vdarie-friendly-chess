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
RestoreBestWay ();
int
SaveToRepository ();
int
InitValPiesa (int level);
int
AgeReductionHistory (int percent);

extern int ValPiesa[];

//
// Age Reduction History by a percent
//
int
AgeReductionHistory (int percent)
{
  int i;

  for (i = 0; i < 13 * 120; i++)
    {
      History.whiteHistory[i] = (History.whiteHistory[i] * percent) / 100;
      History.blackHistory[i] = (History.blackHistory[i] * percent) / 100;
      History.whiteMateHistory[i] = 0;
      History.blackMateHistory[i] = 0;
    }

  return 1;
}

//
// Clear History
//
int
ClearHistory ()
{
  int i;

  for (i = 0; i < 13 * 120; i++)
    {
      History.whiteHistory[i] = 0;
      History.blackHistory[i] = 0;

      History.numarInRamura[i] = 0;
      History.sumaPonderiInRamura[i] = 0;
      History.whiteHistory3[i] = 0;
      History.blackHistory3[i] = 0;
      History.whiteMateHistory[i]
          = 0; // mate history - moves that lead to mates
      History.blackMateHistory[i] = 0;
    }

  return 1;
}

//
// Init weakness of pieces for beginners
//
int
InitValPiesa (int level)
{
  int i;

  for (i = 0; i < 13; i++)
    {
      ValPiesa[i] = ValPiesaReal[i];
      //   if((i != 1) &&  (i != 7) && (gameDepth > 40) && (gameDepth < 80))
      //	   ValPiesa[i] += ValPiesa[i];
    }

  if (level <= 4)
    {

      if (level == 4)
        {
          ValPiesa[WQ] = 600;
          ValPiesa[BQ] = -600;
        } // a weak queen

      if (level == 3)
        {
          ValPiesa[WR] = 300;
          ValPiesa[BR] = -300;
        } // a rook like a bishop

      if (level == 2)
        {
          ValPiesa[WB] = 100;
          ValPiesa[BB] = -100;
        } // a bishop like a pawn
      if (level == 2)
        {
          ValPiesa[WN] = 100;
          ValPiesa[BN] = -100;
        } // a bishop like a pawn

      if (level == 1)
        {
          ValPiesa[WP] = 10;
          ValPiesa[BP] = -10;
        } // a pawn without value

      if (level == 0)
        {
          ValPiesa[WB] = 20;
          ValPiesa[BB] = -20;
        } // a bishop with no value
      if (level == 0)
        {
          ValPiesa[WN] = 20;
          ValPiesa[BN] = -20;
        } // a knight with no value
      if (level == 0)
        {
          ValPiesa[WP] = 10;
          ValPiesa[BP] = -10;
        } // a knight with no value
    }
  return 1;
}

// given a move in binary format, make an output string reprezenting the move
int
PutMoveString (int Move)
{
  int k;

  int sq1, sq2, indicators;

  sq1 = (Move >> 8) & 255;
  sq2 = (Move & 255);
  indicators = (Move >> 24) & 15;

  if (sq1 == 0)
    {
      strcpy (moveout, "      ");
      return 0;
    }

  for (k = 0; moveout[k] = ' ', k < 10; k++)
    ;

  moveout[1] = (sq1 % 10) + 64 + 32;
  moveout[2] = (sq1 / 10) + 48 - 1;

  moveout[3] = '-'; // intre cele doua patrate este '-' sau 'x' daca se ia ceva
  if (p2 != 0)
    {
      moveout[3] = 'x';
    }
  moveout[4] = (sq2 % 10) + 64 + 32;

  moveout[5] = (sq2 / 10) + 48 - 1;

  moveout[0] = ListPieces[p1];
  moveout[6] = ' ';
  k = indicators;
  if ((k == 2) || (k == 3))
    moveout[6] = 'p';
  if ((sq1 == 25) && (sq2 == 27) && (p1 == WK))
    strcpy (moveout, " 0 - 0 ");
  if ((sq1 == 95) && (sq2 == 97) && (p1 == BK))
    strcpy (moveout, " 0 - 0 ");
  if ((sq1 == 25) && (sq2 == 23) && (p1 == WK))
    strcpy (moveout, " 0-0-0 ");
  if ((sq1 == 95) && (sq2 == 93) && (p1 == BK))
    strcpy (moveout, " 0-0-0 ");

  moveout[7] = 0;

  return 1;
}

int xnWP[13] = { 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
int xnBP[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 };

//
// PlayMove - Mutare pe TABLA a unei mutari
//
int inline PlayMove (int MoveToDo)
{

  int retVal;

  int SqTo = MoveToDo & 255;
  int SqFrom = (MoveToDo >> 8) & 255;
  int PieceMoved = (MoveToDo >> 20) & 15;
  int PieceTaken = (MoveToDo >> 16) & 15;
  int Indicators = (MoveToDo >> 24) & 15;

  pstack = (struct StackElement*)&Stack[gameDepth];
  pstackxx = (struct StackElement*)&Stack[gameDepth + 1];
  pstack2 = (struct StackElement2*)&Stack2[gameDepth];
  pstackxx2 = (struct StackElement2*)&Stack2[gameDepth + 1];

  pstack2->SqFrom = SqFrom;
  pstack2->SqTo = SqTo;
  pstack2->PieceMoved = PieceMoved;
  pstack2->PieceTaken = PieceTaken;
  pstack2->Indicators = Indicators;

  retVal = NO_EXT;

  // Cksum Problem for each position after a move

  CopyMemory ((char*)&Stack[gameDepth + 1], (char*)&Stack[gameDepth],
              sizeof (struct StackElement));

  if (tabla[SqFrom] != PieceMoved)
    {
      printf (" PlayMove - EROARE !!!!!!!!!!  piesa care muta nu mai e in "
              "tabla !!");
      getch ();
    }
  if (tabla[SqTo] != PieceTaken)
    {

      printf (" PlayMove - EROARE !!!!!!!!!!  piesa unde muta nu e cea din "
              "tabla !!");
      getch ();
    }

  if (PieceTaken != 0)
    pstackxx->nPieces--;

  tabla[SqFrom] = 0;

  tabla[SqTo] = PieceMoved;

  UpdateNormal (SqFrom, SqTo, PieceMoved, PieceTaken);

  // test for for rocades and enPassant

  switch (Indicators)
    {
    case 1: // white rocade
      pstackxx->WCastle = 3;
      if (SqTo == 27)
        {
          tabla[28] = 0;
          tabla[26] = WR;
          UpdateNormal (28, 26, WR, 0);
        }
      else
        {
          tabla[21] = 0;
          tabla[24] = WR;
          UpdateNormal (21, 24, WR, 0);
        }
      break;
    case 2: // black rocade
      pstackxx->BCastle = 3;
      if (SqTo == 97)
        {
          tabla[98] = 0;
          tabla[96] = BR;
          UpdateNormal (98, 96, BR, 0);
        }
      else
        {
          tabla[91] = 0;
          tabla[94] = BR;
          UpdateNormal (91, 94, BR, 0);
        }
      break;
    case 3: // white en passant
      if (SqTo == (SqFrom + 9))
        {
          pstackxx->PawnHash -= BPHashSq[SqFrom - 1];
          tabla[SqFrom - 1] = 0;
          UpdateNormal (0, SqFrom - 1, 0, BP);
        }
      else
        {
          tabla[SqFrom + 1] = 0;
          pstackxx->PawnHash -= BPHashSq[SqFrom + 1];
          UpdateNormal (0, SqFrom + 1, 0, BP);
        }
      break;
    case 4: // black en passant
      if (SqTo == (SqFrom - 9))
        {
          tabla[SqFrom + 1] = 0;
          pstackxx->PawnHash -= WPHashSq[SqFrom + 1];
          UpdateNormal (0, SqFrom + 1, 0, WP);
        }
      else
        {
          tabla[SqFrom - 1] = 0;
          pstackxx->PawnHash -= WPHashSq[SqFrom - 1];
          UpdateNormal (0, SqFrom - 1, 0, WP);
        }
      break;
    case 5: // white transform in queen
      tabla[SqTo] = WQ;
      pstackxx->whitePower += ValPiesa[WQ] - ValPiesa[WP];
      AddPiece (SqTo, WP, WQ);
      break;
    case 6: // white transform in rook
      tabla[SqTo] = WR;
      pstackxx->whitePower += ValPiesa[WR] - ValPiesa[WP];
      AddPiece (SqTo, WP, WR);
      break;
    case 7: // white transform in bishop
      tabla[SqTo] = WB;
      pstackxx->whitePower += ValPiesa[WB] - ValPiesa[WP];
      AddPiece (SqTo, WP, WB);
      break;
    case 8: // white transform in knight
      tabla[SqTo] = WN;
      pstackxx->whitePower += ValPiesa[WN] - ValPiesa[WP];
      AddPiece (SqTo, WP, WN);
      break;
    case 9: // black transform in queen
      tabla[SqTo] = BQ; // Atentie blackPower e pozitiv !!!!!!
      pstackxx->blackPower -= ValPiesa[BQ] - ValPiesa[BP];
      AddPiece (SqTo, BP, BQ);
      break;
    case 10: // black transform in rook
      tabla[SqTo] = BR;
      pstackxx->blackPower -= ValPiesa[BR] - ValPiesa[BP];
      AddPiece (SqTo, BP, BR);
      break;
    case 11: // black transform in bishop
      tabla[SqTo] = BB;
      pstackxx->blackPower -= ValPiesa[BB] - ValPiesa[BP];
      AddPiece (SqTo, BP, BB);
      break;
    case 12: // black transform in knight
      tabla[SqTo] = BN;
      pstackxx->blackPower -= ValPiesa[BN] - ValPiesa[BP];
      AddPiece (SqTo, BP, BN);
      break;
    case 0:
      break;
    default:
      break;
    } // end switch

  pstackxx->ponder = pstackxx->whitePower - pstackxx->blackPower;

  pstackxx->whiteKingSquare = pstackxx->pieceAPosition[WK][0];
  pstackxx->blackKingSquare = pstackxx->pieceAPosition[BK][0];

  pstackxx->nWMinors = pstackxx->pieceCounts[WN] + pstackxx->pieceCounts[WB];
  pstackxx->nBMinors = pstackxx->pieceCounts[BN] + pstackxx->pieceCounts[BB];

  if (RocadaFlagsMayChange[SqFrom] != 0 || RocadaFlagsMayChange[SqTo] != 0)
    {
      if (tabla[21] != WR)
        pstackxx->RocadeIndicators = pstackxx->RocadeIndicators & (63 - 32);
      if (tabla[28] != WR)
        pstackxx->RocadeIndicators = pstackxx->RocadeIndicators & (63 - 16);
      if (tabla[91] != BR)
        pstackxx->RocadeIndicators = pstackxx->RocadeIndicators & (63 - 8);
      if (tabla[98] != BR)
        pstackxx->RocadeIndicators = pstackxx->RocadeIndicators & (63 - 4);
      if (tabla[25] != WK)
        pstackxx->RocadeIndicators = pstackxx->RocadeIndicators & (63 - 2);
      if (tabla[95] != BK)
        pstackxx->RocadeIndicators = pstackxx->RocadeIndicators & (63 - 1);
    }

  pstack2->move = MoveToDo;

  gameDepth++;

  pstackxx2->sumranks = pstack2->sumranks + pstack2->rankOnLevel;

  who = (who + 1) & 1;

  if (PieceTaken > 0)
    {
      if (PieceTaken < 7) // White piece Taken
        {
          if (PieceTaken == WP)
            {
              retVal = PAWNCAP;
              if (SqTo == 48 && PieceMoved == BQ && tabla[27] == WK
                  && tabla[37] != WP)
                retVal = KINGSAFETY2;
            }
          else
            {
              if (abs (ValPiesa[PieceTaken] + ValPiesa[PieceMoved]) < 70)
                retVal = SqTo;
              else
                retVal = CAPTURE; // for recapture ext
              if ((pstackxx->nWP + pstackxx->nBP) == 0)
                retVal = KINGPAWNS;
              if (PieceMoved == BK && (SqTo == 88)
                  && pstackxx->pieceCounts[WQ] > 0)
                retVal = MATETHREAT;
            }
        }
      else // Black piece Taken
        {
          if (PieceTaken == BP)
            {
              retVal = PAWNCAP;
              if (SqTo == 78 && PieceMoved == WQ && tabla[97] == BK
                  && tabla[87] != BP)
                retVal = KINGSAFETY2;
            }
          else
            {
              // if ( nCap < WROOK) Board.nWMinors--;
              if (abs (ValPiesa[PieceTaken] + ValPiesa[PieceMoved]) < 70)
                retVal = SqTo;
              else
                retVal = CAPTURE; // for recapture ext
              if ((pstackxx->nWP + pstackxx->nBP) == 0)
                retVal = KINGPAWNS;
              if (PieceMoved == WK && (SqTo == 38)
                  && pstackxx->pieceCounts[BQ] > 0)
                retVal = MATETHREAT;
            }
        }
    } // if pieceTaken > 0

  if (PieceMoved == WP)
    {
      if (SQRANK[SqTo] >= 6)
        {
          if (retVal != KINGPAWNS)
            {
              if (SQRANK[SqTo] == 7)
                retVal = PAWNPUSH;
              else if (retVal != ENPASSANT && SQRANK[SqTo] == 6
                       && pstackxx->nBP <= 1 && tabla[SqTo + 10] == 0
                       && tabla[SqTo + 9] != BP && tabla[SqTo + 11] != BP)
                retVal = PAWNPUSH2;
            }
        }
    }
  else if (PieceMoved == BP)
    {

      if (SQRANK[SqTo] <= 3)
        {
          if (retVal != KINGPAWNS)
            {
              if (SQRANK[SqTo] == 2)
                retVal = PAWNPUSH;
              else if (retVal != ENPASSANT && SQRANK[SqTo] == 3
                       && pstackxx->nWP <= 1 && tabla[SqTo - 10] == 0
                       && tabla[SqTo - 9] != WP && tabla[SqTo - 11] != WP)
                retVal = PAWNPUSH2;
            }
        }
    }

  // ensure counts be adequated to positions list
  for (int it = WP; it < BK; it++)
    {
      int nx = Stack[gameDepth].pieceCounts[it];
      if (nx == 0)
        {
          Stack[gameDepth].pieceAPosition[it][0] = 0;
          Stack[gameDepth].pieceAPosition[it][1] = 0;
          Stack[gameDepth].pieceAPosition[it][2] = 0;
        }
      else
        Stack[gameDepth].pieceAPosition[it][nx] = 0;
    }

  return retVal;

} // end PlayMove

int inline UpdateNormal (int SqFrom, int SqTo, int PieceMoved, int PieceTaken)
{

  pstackxx->positionVal += TabPositionValues[PieceMoved][SqTo]
                           - TabPositionValues[PieceMoved][SqFrom];

  for (i = 0; i < pstackxx->pieceCounts[PieceMoved]; i++)
    {
      if (pstackxx->pieceAPosition[PieceMoved][i] == SqFrom)
        {
          pstackxx->pieceAPosition[PieceMoved][i] = SqTo;
          break;
        }
    }
  if (PieceTaken > 0)
    {

      if (PieceTaken < BP)
        pstackxx->whitePower -= ValPiesa[PieceTaken];
      else
        pstackxx->blackPower += ValPiesa[PieceTaken];

      Stack[gameDepth].nWP -= xnWP[PieceTaken];
      Stack[gameDepth].nBP -= xnBP[PieceTaken];

      for (i = 0; i < pstackxx->pieceCounts[PieceTaken]; i++)
        {
          if (pstackxx->pieceAPosition[PieceTaken][i] == SqTo)
            {
              pstackxx->pieceAPosition[PieceTaken][i]
                  = pstackxx->pieceAPosition[PieceTaken]
                                            [pstackxx->pieceCounts[PieceTaken]
                                             - 1];
              pstackxx->pieceAPosition[PieceTaken]
                                      [pstackxx->pieceCounts[PieceTaken] - 1]
                  = 0;
              pstackxx->pieceCounts[PieceTaken]--;
              break;
            }
        }
      if (PieceTaken == WP)
        {
          pstackxx->PawnHash -= WPHashSq[SqTo];
          pstackxx->pawnWOnFiles[SqTo % 10]--;
        }

      if (PieceTaken == BP)
        {
          pstackxx->PawnHash -= BPHashSq[SqTo];
          pstackxx->pawnBOnFiles[SqTo % 10]--;
        }

      pstackxx->positionVal -= TabPositionValues[PieceTaken][SqTo];

    } // piece taken

  if (PieceMoved == WP)
    {
      pstackxx->pawnWOnFiles[SqTo % 10]++;
      pstackxx->pawnWOnFiles[SqFrom % 10]--;
      pstackxx->PawnHash += WPHashSq[SqTo] - WPHashSq[SqFrom];

      if ((SqTo - SqFrom) == 20)
        pstackxx2->enPassant = SqTo;
      else
        pstackxx2->enPassant = 0;
    }
  else if (PieceMoved == BP)
    {
      pstackxx->pawnBOnFiles[SqTo % 10]++;
      pstackxx->pawnBOnFiles[SqFrom % 10]--;
      pstackxx->PawnHash += BPHashSq[SqTo] - BPHashSq[SqFrom];

      if ((SqTo - SqFrom) == -20)
        pstackxx2->enPassant = SqTo;
      else
        pstackxx2->enPassant = 0;
    }

  return 0;
}
// Case transform; To update list structure in transformation square
int inline AddPiece (int SqTo, int OldPiece, int NewPiece)
{

  for (i = 0; i < pstackxx->pieceCounts[OldPiece]; i++)
    {
      if (pstackxx->pieceAPosition[OldPiece][i] == SqTo)
        {
          pstackxx->pieceAPosition[OldPiece][i]
              = pstackxx->pieceAPosition[OldPiece]
                                        [pstackxx->pieceCounts[OldPiece] - 1];
          pstackxx
              ->pieceAPosition[OldPiece][pstackxx->pieceCounts[OldPiece] - 1]
              = 0;
          pstackxx->pieceCounts[OldPiece]--;
          break;
        }
    }

  pstackxx->pieceAPosition[NewPiece][pstackxx->pieceCounts[NewPiece]] = SqTo;
  pstackxx->pieceCounts[NewPiece]++;

  if (OldPiece == WP)
    {
      pstackxx->PawnHash -= WPHashSq[SqTo];
      pstackxx->pawnWOnFiles[SqTo & 10]--;
    }

  if (OldPiece == BP)
    {
      pstackxx->PawnHash -= BPHashSq[SqTo];
      pstackxx->pawnBOnFiles[SqTo % 10]--;
    }
  Stack[gameDepth].nWP -= xnWP[OldPiece];
  Stack[gameDepth].nBP -= xnBP[OldPiece];

  pstackxx->positionVal -= TabPositionValues[OldPiece][SqTo];
  pstackxx->positionVal += TabPositionValues[NewPiece][SqTo];

  return 1;
}

//
// Test Repetition
//
int inline TestRepeat ()
{
  int repeatPosition;
  int reps;
  int m;

  repeatvalue = 0;

  int aa, pie, pie2;

  aa = 0;
  // Check if no 3 times position repetition
  for (repeatPosition = 0, m = gameDepth; m > 0; m--)
    {
      pie = Stack2[m - 1].PieceMoved;
      pie2 = Stack2[m - 1].PieceTaken;

      if ((pie == WP) || (pie == BP) || pie2 > 0)
        break;

      if ((Stack[gameDepth].HashCheck == Stack[m].HashCheck)
          && (Stack[m].HashCheck != 0)
          && (Stack[gameDepth].HashKey == Stack[m].HashKey)
          && (Stack[m].HashKey != 0))
        repeatPosition++;
    }
  reps = repeatPosition;
  if (repeatPosition >= 3)
    {
      aa = 0;
      if (who == 0)
        aa = 100;
      else
        aa = -100;

      //				CopyMove(&Stack[gameDepth].explicitMove,&Stack[gameDepth].MiniMaxMove);
      //				CopyMove(&Stack[gameDepth].explicitMove,&Stack[gameDepth].BestWay[0]);

      Stack2[gameDepth].BestWay[0] = 0;
      Stack2[gameDepth].flagsOfGame = 44; // REPET 3 times

      repeatvalue = aa;

      return reps;
    }

  // Rull 50 moves inactivity
  for (repeatPosition = 0, m = gameDepth; m > 0; m--)
    {
      int PieceMoved1 = Stack2[m].PieceMoved;
      int PieceTaken1 = Stack2[m].PieceTaken;

      if (PieceMoved1 == WP || PieceTaken1 != 0 || Stack2[m].Indicators != 0
          || PieceMoved1 == BP)
        break;

      repeatPosition++;
    }
  if (repeatPosition >= 100)
    {
      if (who == 0)
        aa = 100;
      else
        aa = -100;

      Stack2[gameDepth].BestWay[0] = 0;
      Stack2[gameDepth].flagsOfGame = 33; // Inactivitate 50 mutari

      repeatvalue = aa;

      return repeatPosition;
    }
  return reps;
} // Test Repetition

//
// Compute Extensions
//
void inline ComputeExtensions (int& depth, int& ahead, int& Ext,
                               int& nextDepth, int& nDst)
{
  int ex;
  int nd;

  int SqTo = nDst;
  ex = suplimDepth[ahead - 1];

  nextDepth = depth - 1;

  //		if(ahead > ksup) return; // do not extend things that may be
  //done by Quesence algorithm

  if (Ext == KINGPAWNS
      && recap[ahead - 1]
             <= CAPTURE) // Extend 2 ply on transition to King and Pawns ending
    {
      if (ahead < 4)
        nextDepth += 3;
      else
        nextDepth += 2;
    }

  // Pawn Push or Checked Opponent PAWNPUSH
  else if (Ext >= PAWNPUSH || Stack2[gameDepth].IsCheckOwn > 0)
    {
      ex += 4;
    }
  else if (Ext == MATETHREAT)
    {
      ex += 2;
    }

  // Single Move in check
  else if (Stack2[gameDepth - 1].movesOnLevel == 1)
    {

      if (Stack2[gameDepth - 1].SqTo == Stack2[gameDepth - 1].IsCheckOwn
          && (Stack2[gameDepth - 1].PieceMoved == WK
              || Stack2[gameDepth - 1].PieceMoved == BK))
        ex += 1;
      else
        ex += 3;
    }
  else if (SqTo == recap[ahead - 1])
    {
      // Recapture
      recap[ahead] = NO_EXT; // Don't do 2 recaps in a row
      if (Stack[gameDepth].nWP == 1 && Stack[gameDepth].nBP == 1)
        ex += 4; // recap to 1 piece ending
      else if (Stack2[gameDepth].IsCheckOwn == 0)
        ex += 3;
      else
        ex += 1;
    }

  else if (Stack2[gameDepth - 1].movesOnLevel <= 2) // 2 responses to check
    {
      ex += 1;
    }
  //		else if(Stack[gameDepth - 1].explicitMove.Indicators > 0)
  //		{
  //			if ((gameDepth - treeDepth) <= (2 * kmax))
  //				ex += 4; // till 2 * search depth full ply,
  //alter only 3/4 of play 			else 				ex += 3;
  //		}
  // extent the first ranks on the width of the tree

  //		else if((Stack[gameDepth - 1].sumRanks - EXTRANK ) < (gameDepth
  //- treeDepth) && 			(kmax + EXTRANK) >= (gameDepth - treeDepth) &&
  //			Stack[gameDepth - 1].sumRanks == (gameDepth -
  //treeDepth) && 			Stack[gameDepth - 1].sumRanks > 0 && nodes > 0 && ex < 4) 				ex
  //+= 4;

  //		else if((Stack[gameDepth - 1].PieceMoved == WP) &&
  //(Stack[gameDepth - 1].SqTo > 80)) 			ex += 4;

  //		else if((Stack[gameDepth - 1].PieceMoved == BP) &&
  //(Stack[gameDepth - 1].SqTo < 40)) 			ex += 4; // extend if pawn moves to 7
  //line
  /*
                  // king safety extentions
                  if(Stack[gameDepth - 1].PieceTaken > 0)
                  {
                          if(who == 1 && DistKing[Stack[gameDepth -
     1].SqTo][Stack[gameDepth].blackKingSquare] <= 2 && ex < 4) ex = 4; if(who
     == 0 && DistKing[Stack[gameDepth -
     1].SqTo][Stack[gameDepth].whiteKingSquare] <= 2 && ex < 4) ex = 4;
                  }
  */

  if (ex >= 4)
    {
      nextDepth++; // modify nextdepth if have enough suplimdepth
      ex -= 4;
    }
  suplimDepth[ahead] = ex; // set new extension

  if (nextDepth < 1 && recap[ahead] > RECAP)
    eChecks[ahead + 1] = 1;
  else
    eChecks[ahead + 1] = 2;
}

//
// PlayMoveTa - Move forward simplified
//
int
PlayMoveTa ()
{

  int SqTo;
  int SqFrom;
  int PieceTaken;
  int PieceMoved;
  int Indicators;

  SqTo = MoveToDoTa & 255;

  SqFrom = (MoveToDoTa >> 8) & 255;

  PieceTaken = (MoveToDoTa >> 16) & 15;

  PieceMoved = (MoveToDoTa >> 20) & 15;
  Indicators = (MoveToDoTa >> 24) & 15;

  Stack2[gameDepth].SqFrom = SqFrom;
  Stack2[gameDepth].SqTo = SqTo;
  Stack2[gameDepth].PieceMoved = PieceMoved;
  Stack2[gameDepth].PieceTaken = PieceTaken;
  Stack2[gameDepth].Indicators = Indicators;

  ta[SqFrom] = 0;
  ta[SqTo] = PieceMoved;

  switch ((MoveToDoTa >> 24) & 15)
    {
    case 1: // white rocade
      if (SqTo == 27)
        {
          ta[28] = 0;
          ta[26] = WR;
        }
      else
        {
          ta[21] = 0;
          ta[24] = WR;
        }
      break;
    case 2: // black rocade
      if (SqTo == 97)
        {
          ta[98] = 0;
          ta[96] = BR;
        }
      else
        {
          ta[91] = 0;
          ta[94] = BR;
          if (SqTo != 93)
            {
              who = who;
            }
        }
      break;
    case 3: // white en passant
      if (SqTo == (SqFrom + 9))
        {
          ta[SqFrom - 1] = 0;
        }
      else
        {
          ta[SqFrom + 1] = 0;
        }
      break;
    case 4: // black en passant
      if (SqTo == (SqFrom - 9))
        {
          ta[SqFrom + 1] = 0;
        }
      else
        {
          ta[SqFrom - 1] = 0;
        }
      break;
    case 5: // white transform in queen
      ta[SqTo] = WQ;
      break;
    case 6: // white transform in rook
      ta[SqTo] = WR;
      break;
    case 7: // white transform in bishop
      ta[SqTo] = WB;
      break;
    case 8: // white transform in knight
      ta[SqTo] = WN;
      break;
    case 9: // black transform in queen
      ta[SqTo] = BQ;
      break;
    case 10: // black transform in rook
      ta[SqTo] = BR;
      break;
    case 11: // black transform in bishop
      ta[SqTo] = BB;
      break;
    case 12: // black transform in knight
      ta[SqTo] = BN;
      break;
    default:
      break;
    } // end switch

  return 1;

} // end PlayMoveTa

// AllowedMove - Deternima daca o mutare este legala
int
AllowedMove (int MoveToDoWW, int& NewMove)
{
  int aa;
  int i;

  int SqFrom, SqTo, PieceMoved, PieceTaken, Indicators;

  int MoveToDoXX;
  MoveToDoXX = MoveToDoWW;

  for (i = 0; i < Stack2[gameDepth].nbAttacks; i++)
    {
      MoveToDo = Stack2[gameDepth].Attacks[i];

      SqTo = MoveToDo & 255;
      SqFrom = (MoveToDo >> 8) & 255;
      PieceMoved = (MoveToDo >> 20) & 15;
      PieceTaken = (MoveToDo >> 16) & 15;
      Indicators = (MoveToDo >> 24) & 15;

      PlayMove (MoveToDo);

      aa = MoveIsCheck ();

      MoveBack ();

      if (aa < 0)
        continue;

      if (((MoveToDoXX & 0x0000FFFF) == (MoveToDo & 0x0000FFFF)
           && (((MoveToDo >> 24) & 15) == 3 || ((MoveToDo >> 24) & 15) == 4
               || ((MoveToDoXX >> 24) & 15) == (MoveToDo >> 24) & 15)))
        {
          MoveToDoXX = MoveToDo;
          NewMove = MoveToDo;
          return 1; // gasit in lista
        }
    }

  SqFromActive = 0;
  SqToActive = 0;
  return 0; // non found in list generated
}

//
// MoveBack() - Move back for only one level
//
int inline MoveBack ()
{

  int xx;

  int SqTo;
  int SqFrom;
  int PieceTaken;
  int PieceMoved;
  int indi;

  gameDepth--;

  // Stack[gameDepth].HashKey   ^= HashFunction  [8];
  // Stack[gameDepth].HashCheck ^= HashFunction2 [8];

  SqFrom = (Stack2[gameDepth].move >> 8) & 255;
  SqTo = Stack2[gameDepth].move & 255;
  PieceMoved = (Stack2[gameDepth].move >> 20) & 15;
  PieceTaken = (Stack2[gameDepth].move >> 16) & 15;

  indi = (Stack2[gameDepth].move >> 24) & 15;

  if (indi == 0)
    {
      tabla[SqTo] = PieceTaken;
      tabla[SqFrom] = PieceMoved;
      who = (who + 1) & 1;
      return 0;
    }

  if (tabla[SqFrom] != 0)
    {
      printf (" MoveBack - EROARE !!!!!!!!!!  locul in care muta inapoi e "
              "ocupat !!");
      getch ();
    }
  if (tabla[SqTo] == 0)
    {
      printf (" MoveBack - EROARE !!!!!!!!!!  locul din care muta  inapoi nu "
              "are piesa !!");
      getch ();
    }

  tabla[SqTo] = PieceTaken;
  tabla[SqFrom] = PieceMoved;

  switch (indi)
    {
    case 1: // white rocade
      if (SqTo == 27)
        {
          tabla[28] = WR;
          tabla[26] = 0;
        }
      else
        {
          tabla[21] = WR;
          tabla[24] = 0;
        }
      break;
    case 2: // black rocade
      if (SqTo == 97)
        {
          tabla[98] = BR;
          tabla[96] = 0;
        }
      else
        {
          tabla[91] = BR;
          tabla[94] = 0;
          if (SqTo != 93)
            {
              who = who;
            }
        }
      break;
    case 3: // white en passant
      if (SqTo == (SqFrom + 9))
        {
          tabla[SqFrom - 1] = BP;
        }
      else
        {
          tabla[SqFrom + 1] = BP;
        }
      break;
    case 4: // black en passant
      if (SqTo == (SqFrom - 9))
        {
          tabla[SqFrom + 1] = WP;
        }
      else
        {
          tabla[SqFrom - 1] = WP;
        }
      break;
    case 5: // white transform in queen
      tabla[SqTo] = PieceTaken;
      break;
    case 6: // white transform in rook
      tabla[SqTo] = PieceTaken;
      break;
    case 7: // white transform in bishop
      tabla[SqTo] = PieceTaken;
      break;
    case 8: // white transform in knight
      tabla[SqTo] = PieceTaken;
      break;
    case 9: // black transform in queen
      tabla[SqTo] = PieceTaken;
      break;
    case 10: // black transform in rook
      tabla[SqTo] = PieceTaken;
      break;
    case 11: // black transform in bishop
      tabla[SqTo] = PieceTaken;
      break;
    case 12: // black transform in knight
      tabla[SqTo] = PieceTaken;
      break;
    case 0:
      break;
    default:
      who = who;
      break;
    } // end switch

  who = (who + 1) & 1;

  return 1;
} // end MoveBack aaaaaa

//
// MoveBackTa() - Move back simplified
//
int
MoveBackTa ()
{
  int xx;

  int SqTo;
  int SqFrom;
  int PieceTaken;
  int PieceMoved;
  int Indicators;

  SqTo = MoveToDoTa & 255;

  SqFrom = (MoveToDoTa >> 8) & 255;

  PieceTaken = (MoveToDoTa >> 16) & 15;

  PieceMoved = (MoveToDoTa >> 20) & 15;

  Indicators = (MoveToDoTa >> 24) & 15;

  ta[SqTo] = PieceTaken;
  ta[SqFrom] = PieceMoved;

  switch (Indicators)
    {
    case 1: // white rocade
      if (SqTo == 27)
        {
          ta[28] = WR;
          ta[26] = 0;
        }
      else
        {
          ta[21] = WR;
          ta[24] = 0;
        }
      break;
    case 2: // black rocade
      if (SqTo == 97)
        {
          ta[98] = BR;
          ta[96] = 0;
        }
      else
        {
          ta[91] = BR;
          ta[94] = 0;
        }
      break;
    case 3: // white en passant
      if (SqTo == (SqFrom + 9))
        {
          ta[SqFrom - 1] = BP;
        }
      else
        {
          ta[SqFrom + 1] = BP;
        }
      break;
    case 4: // black en passant
      if (SqTo == (SqFrom - 9))
        {
          ta[SqFrom + 1] = WP;
        }
      else
        {
          ta[SqFrom - 1] = WP;
        }
      break;
    case 5: // white transform in queen
      ta[SqTo] = PieceTaken;
      break;
    case 6: // white transform in rook
      ta[SqTo] = PieceTaken;
      break;
    case 7: // white transform in bishop
      ta[SqTo] = PieceTaken;
      break;
    case 8: // white transform in knight
      ta[SqTo] = PieceTaken;
      break;
    case 9: // black transform in queen
      ta[SqTo] = PieceTaken;
      break;
    case 10: // black transform in rook
      ta[SqTo] = PieceTaken;
      break;
    case 11: // black transform in bishop
      ta[SqTo] = PieceTaken;
      break;
    case 12: // black transform in knight
      ta[SqTo] = PieceTaken;
      break;
    default:
      break;
    } // end switch

  return 1;
} // end MoveBackTa

int
SetTabPositionValues ()
{
  int PieceMoved;
  int SqFrom;
  for (PieceMoved = 0; PieceMoved < 13; PieceMoved++)
    {
      for (SqFrom = 0; SqFrom < 128; SqFrom++)
        {
          switch (PieceMoved)
            {
            case 0:
              TabPositionValues[PieceMoved][SqFrom] = 0;
              break;
            case WP:
              TabPositionValues[PieceMoved][SqFrom] = WPawnPos[SqFrom];
              break;
            case WR:
              TabPositionValues[PieceMoved][SqFrom] = WRookValue[SqFrom];
              break;
            case WN:
              TabPositionValues[PieceMoved][SqFrom] = WKnightPost[SqFrom];
              break;
            case WB:
              TabPositionValues[PieceMoved][SqFrom] = WBishopValue[SqFrom];
              break;
            case WQ:
              TabPositionValues[PieceMoved][SqFrom] = WQueenValue[SqFrom];
              break;
            case WK:
              if (Stack[gameDepth].blackPower > 2000)
                TabPositionValues[PieceMoved][SqFrom] = WKingValue[SqFrom];
              else
                TabPositionValues[PieceMoved][SqFrom] = WKingValueEnd[SqFrom];
              break;
            case BP:
              TabPositionValues[PieceMoved][SqFrom]
                  = -WPawnPos[Simetric[SqFrom]];
              break;
            case BR:
              TabPositionValues[PieceMoved][SqFrom]
                  = -WRookValue[Simetric[SqFrom]];
              break;
            case BN:
              TabPositionValues[PieceMoved][SqFrom]
                  = -WKnightPost[Simetric[SqFrom]];
              break;
            case BB:
              TabPositionValues[PieceMoved][SqFrom]
                  = -WBishopValue[Simetric[SqFrom]];
              break;
            case BQ:
              TabPositionValues[PieceMoved][SqFrom]
                  = -WQueenValue[Simetric[SqFrom]];
              break;
            case BK:
              if (Stack[gameDepth].whitePower > 2000)
                TabPositionValues[PieceMoved][SqFrom]
                    = -WKingValue[Simetric[SqFrom]];
              else
                TabPositionValues[PieceMoved][SqFrom]
                    = -WKingValueEnd[Simetric[SqFrom]];
              break;
            }
        } // for SqFrom
    } // for PieceMoved
  return 1;
}
