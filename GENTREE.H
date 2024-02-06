
// int lastOptimalMove;
int ind_pv = 0;

int pondere_return;
int indi_return;
 int nxxxtree = 0;
 int zeros[80];
 
 int  EvaluateAttacks(int SqFrom,int SqTo,int PieceMoved,int PieceTaken,int ahead,int depth);
 
 int WhiteCantWin(int &);
 int BlackCantWin(int &);
 
 int DepuneVarianta(int ponderea,int indicatori);
 int RegasireVarianta(int caz);
int ClearVariante();
 
int level_zero_moves[1000];
int level_zero_values[1000];
int level_zero_count;

int max_hist = 111111111;

int over_100 = 0;
int zero_level = 0;
int from_attack = 0;
int egal_hi = 0;
int take_big = 0;
int take_small = 0;
int under_attack = 0;
int egal_lite = 0;
int egal_penalties = 0;

int skip1 = 0;
int skip2 = 0;
int noskip = 0;

int extensie_PV ;
int reducere;
int nb_reduceri = 0;
int reduceri_pe_ahead[50];

int taieri_history;

int nb_moveback;


double total_lite_white;
double total_lite_black;

double ee_total_lite_white; // succeded to find in hist2
double ee_total_lite_black;

double mutari_excenge = 0;

double total_flag1 = 0;

double total_abserch = 0;
double total_absearch_peste_kmax = 0;

int evaluare_ok; // if current evaluare better then the last selected as max, then continue search it, if no-alpha-beta prunning
  

struct xxtree {
	int nivel;
	int minimax;
	int movetodo;
	int move_value;
} xxxtree[1000], xxx1;

extern int xc;
extern int yc;
       
int nkillers1 = 0;
int nkillers2 = 0;
int nbest = 0;
int nattack = 0;
int nnoattack = 0;
int zerowindow = 0;
int regasiri_variante = 0;

int no2raz;
double factor[100];
double rankscut[100];
double rankcount[100];

int DRAWSCORE;
int trace=0;
int no1;
int no2;

int levNodes1[100];
int levNodes2[100];

int queenAttacked;


int baseMiniMaxValue = 0;
int nrand = 0;

int nBestmove;
// xxx int tt_negasit1,tt_gasit1;
int iteratii [20002];
int sumnodes = 0;

MSG msg;

int easy = 0;
int heavy = 0;

int alphanodes[100];
int betanodes[100];
int fulllevel[100];
int alphanull[100];
int betanull[100];
int final[100];
int razoring2[100];
int sumwidth[100];
 

long limMax[20] = {100000,100000,100000,20000000,500000,200000000,10000000,40000000,
				  200000000,990000000,0,0,0,0,0,0,0,0,0,0}; // limits for each level

  
//
// GetOptimalMove() - Obtain an optimal move starting from current table : tabla[]
//  
int GetOptimalMove()
{

	
if(treeDepth > 50)
{
	int frr = 0;
}
long xx;

int m;

int p;

int xxMAT = MAT;

for(i=0;i<80;i++) zeros[i] = 0;

for(i=0;i<20002;i++)
 iteratii[i] = 0;

for(i=0;i<50;i++)
{
 if(treeDepth == 0)
	 reduceri_pe_ahead[i] = 0 ;
}


int da;

int lastMiniMaxValue;

DRAWSCORE = 0;

nBestmove = NO_BEST;

baseMiniMaxValue = 0;

double total_flag1 = 0;


if(gameDepth == 60)
{
	int aa = 7;
}

if(who == 0)
 BestVal = -MAT;
else
 BestVal = MAT;

findopening = 0;

// no opening move found, so needs to compute the best move
generatedTotalTotal +=(int)nodes;
generatedExchangeTotal +=generatedExchange;

nodes = 0; // reset moves counter
generatedExchange = 0; // reset exchangemoves counter

time(&rawtime0); // set initial time for thinking

nrand = rawtime0 & 63;

thinking = 1;

g_LastBest = 0;

g_lasteval = 0;


lgBestWay = 0;
lgMax     = 0;
lgMaxKmax = 0;

noTime = 0; // there is still time

g_ucAge++;


// Call function that find and restore a Best Way from repository
SetStruct(); // needed for checksum calculation
BestLine[0] = 0;
SetTabPositionValues();

ClearHistory();
level_zero_count = 0;

  // reset killers moves
  for(i=0;i<MAXDEPTH;i++)
  {
	  Stack2[treeDepth + i].killers[0] = 0;
	  Stack2[treeDepth + i].killers[1] = 0;	  
  }



da = TakeOpeningMove();
if ( treeDepth >= 5)
	da = 0; // test without opening
if(da > 0 && Options.level > 4) return 1; //  found a opening move

rawtime0 = time(&rawtime0);


m = 1;

savedKmax = 0;


int whoroot = who;
for(ind_pv = 1;ind_pv <= 1;ind_pv++)
{
 nodes = 0;
 for(kmax=m;kmax<=ksup + 25;kmax++)
 {
     
	// dont make deep analyse for endgames with rook against bishop or knight;


    int pow1 = Stack[gameDepth].whitePower ;
    int pow2 = Stack[gameDepth].blackPower;

    if(kmax > 7 && pow1 == ValPiesa[WR] && ( pow2 == ValPiesa[WR] || pow2 == ValPiesa[WB] || pow2 == ValPiesa[WN])) break;
    if(kmax > 7 && pow2 == ValPiesa[WR] && ( pow1 == ValPiesa[WR] || pow1 == ValPiesa[WB] || pow1 == ValPiesa[WN])) break;

    // in number of pieces on table is very few and kmax > 12 stop 
	
    if(kmax >= 10 && Stack[gameDepth].nPieces <= 5 && (Stack[gameDepth].whitePower + Stack[gameDepth].whitePower) < 600) break;
	
    Alpha = -MAT + 2 + kmax;
    Beta  =  MAT - 2 - kmax;

    for(i=0;i< 100; i++) rankscut[i] = 0;
    for(i=0;i< 100; i++) rankcount[i] = 1; 
    for(i=0;i< 100; i++) factor[i] = 0; 
	
    if(kmax == 3) nodes = 0; // may be too many nodes in first two iterations	strcpy(Stack2[gameDepth + 1].info, " ");
  
    AgeReductionHistory(20);

    lastMiniMaxValue = GenTree();  


	 da = 0;
 	 int xx = lastMiniMaxValue - OptimalMoveMoveValue;
	 	
	 if((Stack2[gameDepth].rankOnLevel > 3) &&
		 ( Stack2[gameDepth].rankOnLevel * 8 > Stack2[gameDepth].movesOnLevel))
		    da++;

 
	 if(Stack[treeDepth].nPieces > 4 && Stack2[gameDepth].rankOnLevel <= 1 && Stack2[gameDepth].movesOnLevel > 1) da = 0;

	 if(Stack2[gameDepth].rankOnLevel >= Stack2[gameDepth].movesOnLevel ) da = 1;

	 if((lastMiniMaxValue > (MAT - 1000) || lastMiniMaxValue < (-MAT + 1000)) )
		 da ++;

	 if(abs(lastMiniMaxValue) == MAT && noTime == 0 || lastMiniMaxValue == NOTIME ) 
	 {
		 da = 0;
	 }

	 if( (abs(lastMiniMaxValue) < MAT - 1000) && (da > 0) && (Stack2[gameDepth].movesOnLevel > Stack2[gameDepth].rankOnLevel) &&
		  ((Stack2[gameDepth].movesOnLevel > 2 * Stack2[gameDepth].rankOnLevel) ||  (Stack2[gameDepth].movesOnLevel < 10)) &&
		 ((who == WHITE && xx < -80) || ( who == BLACK && xx > 80))) 
		 da = 0; // dont take in account unfinished level search when bad return

	 if(lastMiniMaxValue != NOTIME && noTime == 0) da++; // nivel terminat complet

	 if(kmax == 1) da = 1;
	if( lastMiniMaxValue == MAT - 3 || lastMiniMaxValue == -MAT + 3)
	{
		OptimalMoveMoveValue = lastMiniMaxValue;
		da++;
	}	 
	 if (da > 0  )
	 {
		 // save only when kmax = first iteration or when the ponder is better
		 OptimalMove = nBestmove;
		 MoveToDo = nBestmove;
		 OptimalMoveMoveValue = lastMiniMaxValue;

		savedKmax = kmax;

		 for(m=0;Stack2[gameDepth].BestWay[m] > 0;m++)
		 {
			BestLine[m] = Stack2[gameDepth].BestWay[m];
		   lgBestWay = m+1; // find the length of the best way
		 }
		 BestLine[m] = 0;
		 // save context of the best search
		 bestMiniMax = lastMiniMaxValue;


	 } // if noTime == 0

 //   if( da == 0) break;
	
    if((kmax > 1 && OptimalMoveMoveValue >= (MAT - kmax - 2)) || (OptimalMoveMoveValue <= (-MAT + kmax + 2)))
	{
           if(noTime == 0) break; // a MATE found no more levels needed
	}
    if(kmax == 4 && Stack[treeDepth].nPieces <=  3 && abs(lastMiniMaxValue)  >= (20000 - 100))
	{
           if(noTime == 0) break; // a MATE found no more levels needed
	}
    if(Options.level <= 2 && kmax > 2) return ShowMate();
//    if (Stack2[gameDepth].movesOnLevel <= 1 && kmax >= 2) break; // save the time if only one move
    if(kmax <= 6  && kmax == ( ksup + 1)) break;
	
    if (noTime > 0) break; // really no time
  
	int limsup = limMax[ksup];
	if (Stack[treeDepth].nPieces <= 10) limsup = limsup + limsup / 3 ;

    if (kmax >= 5 && nodes >= limsup  / 3 && Stack2[treeDepth].movesOnLevel > 6) 
		 break; // no time to waste for doing some unfinished works on the next level

 //   if(lastMiniMaxValue > (MAT - 1000) || lastMiniMaxValue < (-MAT + 1000)) break;
 
    if( OptimalMoveMoveValue == (MAT - kmax - 2) || OptimalMoveMoveValue == (-MAT + kmax + 2))
	{
		int xx = 1;
          break; // a MATE found no more levels needed
	}

    if (Stack2[treeDepth].flagsOfGame > 0) break;
	
    if (kmax > 5 && abs(OptimalMoveMoveValue) < 19000 && Stack[treeDepth].nPieces > 10 && Stack2[treeDepth].movesOnLevel == 1) break;
	
  } // for levels
 if(ksup >= 8 && gameDepth > 10 && nodes < 5000000)
 {
	 int xx = 0;
	 xx = 2;
 }
	noTime = 0;
} // for ind_pv

  
  MoveToDo = OptimalMove;
  lastOptimalMove = OptimalMove;
  MoveToDoMoveValue = OptimalMoveMoveValue;

  savedLevel = MoveNumber;

Stack2[gameDepth].BestLineDepth = lgBestWay;
Stack2[gameDepth].MaxDepth = lgMax;

if( m > lgBestWay) lgBestWay = m;

  
// save context of the best search

for(i = 0;i<100;i++)
{
	int su = alphanodes[i] + betanodes[i] + fulllevel[i];
if(su > 0) factor[i] = sumwidth[i] / (alphanodes[i] + betanodes[i] + fulllevel[i]);
}


 char strinfo[20];
  
 float xnodes = nodes / 1000000.;
 int aa = Stack2[treeDepth].rankOnLevel + 1;
 int bb = Stack2[treeDepth].movesOnLevel;
 if(aa > bb) aa = bb;
 
 sprintf(Stack2[treeDepth].info,"");
 if(Stack2[treeDepth].movesOnLevel != 0)
	 sprintf(Stack2[treeDepth].info,"[%2d %2d/%2d %6.1fM] ",kmax, aa,bb,xnodes); 

 time(&rawtime1);

	sprintf(stime,"Timp(sec): %d ",rawtime1 - rawtime0);

int   deltatime = time(&rawtime1) - rawtime0;
      
   if(who == WHITE)
   {
	   sumatimpialb += deltatime;
	   nodesmutarialb += nodes / 1000;
	   timpmediualb = (sumatimpialb) / ((treeDepth + 1.0)/ 2.0);
	   nodesmediualb = (nodesmutarialb) / ( (treeDepth + 1.0) / 2.0);	   
   }
   else
   {
	   sumatimpinegru += deltatime;	
	   nodesmutarinegru += nodes / 1000;
	   timpmediunegru = (sumatimpinegru) / ((treeDepth + 1.0) / 2.0);
	   nodesmediunegru = (nodesmutarinegru) / ((treeDepth + 1.0) / 2.0);	   
	   
   }
 
// calculate branching factor
double tfactor = 0;
double tscut = 0;
double tscor = 0;
double factorint[100];
double sumfactor = 0;

for(i=0;i<100;i++)
{
	// factor[i] = rankscut[i] / rankcount[i];
	factorint[i] = rankcount[i+1] / (rankcount[i] + 1);

	if(i <= 10) 
	{
		tscut += rankscut[i];
		tscor += rankcount[i];
		if(i > 0) sumfactor += factorint[i];		
	}
}
tfactor = sumfactor / 10;

return ShowMate();

     
} // end GetOptimalMove



//
// GENTREE ALGORITHM
//

// pm este setat ca pointer in masiv unde se va genera lista de mutari
int GenTree()
{
int m;
int aa;

int eval;

int totPower;

int value;

int nodesprec;

max_checks_depth = kmax + 2;

int i;
for(i = 0;i<100;i++)
{
alphanodes[i];
betanodes[i];
fulllevel[i];
alphanull[i];
betanull[i];
sumwidth[i];
}

nodes1 = 0;
nodes2 = 0;

time_t tim;

Stack2[gameDepth].sumranks = 0;

treeDepth = gameDepth;
  
RebuildPiecesPositions();
MakePieceList();
ClearVariante();
// refresh taend table


if(Options.level == 8) trace = 1;

// set a random oppening move

  time(&tim);
 

  flags = 0x1000;
  anotherMoveOnLevel = 0;

  SetStruct();
 
  eval = EvaluateBoard ( Alpha,  Beta);

  InitGenTree (); // iteration initialization
 
  MakePieceList();

  totPower = Stack[gameDepth].whitePower + Stack[gameDepth].blackPower;

  CurrentBest = 0; 

  aa = 0;
  if(Stack[gameDepth].nPieces <= 3) 
	{

		aa = ExtendedConsultEndGame();
		if( aa > 0)
		{
			Stack[gameDepth].mateDetected = 1;
			if(kmax < 4)
				kmax = 4;
		}
		
	}
	queenAttacked = 0;
  
	no1 = 0;
	no2 = 0;

	extensie_PV = -1;
	reducere = -1;
	
  zerowindow = 0;
  
  value = AlphaBetaAlgorithm( who, 0, kmax, Alpha, Beta, nBestmove, 1);

  nodes5++;
  baseMiniMaxValue = value;
 
  if(value == NOTIME) return NOTIME;

  // lasati Hash in pace, foloseste la test repetitie
  
  if (value != NOTIME) // Store this move in the hash
		{
		int nHashKey = Stack[gameDepth].HashKey ^ HashFunction[Stack2[gameDepth].enPassant] ^ HashFunction[Stack[gameDepth].WCastle + (Stack[gameDepth].BCastle<<3)];
		}

  return value;

}

int PawnHash;

void InitGenTree ()
{

	MaxDepth = 3 * kmax + 6;

	 if ( (MaxDepth%2) == 0) MaxDepth++;

	 if (MaxDepth >= MAXDEPTH -3) MaxDepth = MAXDEPTH-4;
  
	 Stack2[gameDepth].nbAttacks = 0;
	 Stack2[gameDepth].IsCheckOwn = SetIsCheck(who);
	 Stack[gameDepth].depthReduced = 0;
	 
	 // More Move Stack
	 suplimDepth[0] = 0;
	 recap[0] = NO_EXT;
	 eChecks[1] = 2;

	 
	 // Calculate initial hash values
	 Stack[gameDepth].HashKey  = Hash_Board ();
       Stack[gameDepth].HashCheck = Hash_Board2 ();
       strcmp(Stack2[gameDepth].info, "");
	 strcmp(Stack2[gameDepth + 1].info , "");	 
	 Stack[gameDepth].PawnHash = (int)Pawn_HashBoard ();
	 

}


int inline AlphaBetaAlgorithm(int whoCall, int ahead, int depth,  int alpha, int beta,  int &bestmove,int indNull)
{

	
if(ahead > lgMaxKmax ) lgMaxKmax = ahead;

if((ahead - 1) <= kmax )
	total_abserch++;
else
	total_absearch_peste_kmax++;

int xx;
int yy;

int nextbest = 0;

int nextDepth = 0;

int SqFrom ;
int SqTo ;
int PieceMoved;
int PieceTaken;

int u1, u2, nHashKeyAndFlags;

int nSearched;
int nn;

int MoveToDoOnLevel;

struct StackElement  *wstack;
struct StackElement  *wstackxx;
struct StackElement2  *wstack2;
struct StackElement2  *wstackxx2;

int pond;
int Max;
int eval;

int value ;
int moveDo;
int aaaa;
int foundValidMove = 0;
int rrrr;

int valMat;

int i;
int Ext;

  if(gameDepth > treeDepth)
	Stack[gameDepth].depthReduced = Stack[gameDepth - 1].depthReduced; // progagate depth reduced in the stack

  if (lgMax < ahead) lgMax = ahead;

  horizontNMoves = 0;

  Stack2[gameDepth + 1].killers[0] = 0;
  Stack2[gameDepth + 1].killers_count[0] = 0;	
  Stack2[gameDepth + 1].killers[1] = 0;	  
  Stack2[gameDepth + 1].killers_count[1] = 0;

wstack   = (struct StackElement *)&Stack[gameDepth];
wstackxx = (struct StackElement *)&Stack[gameDepth+1];

wstack2   = (struct StackElement2 *)&Stack2[gameDepth];
wstackxx2 = (struct StackElement2 *)&Stack2[gameDepth+1];


ahead++;
rankcount[ahead-1]++;
foundValidMove = 0;

if(buttonPressed == 6)
{
	noTime = 1;
//	return NOTIME;
}
  
  // if too late then set noTime flag
 
int limsup = limMax[ksup];
if(Stack[treeDepth].nPieces <= 10) limsup = limsup + limsup  / 3 ;



  if(ksup <= 6  && nodes > 2000000) noTime = 1;

  if ((kmax >= 5) && (nodes >= limsup)  )	noTime = 1;  // normal cut of time
  
  if((kmax >= 5) && ( nodes >= (limsup * 0.25)) && (Stack2[treeDepth].rankOnLevel <= 1) && (Stack2[treeDepth+1].rankOnLevel <= 2))
			    noTime = 1; // no time to do all from a new level
  if((kmax >= 5) && ( nodes >= (limsup * 0.28)) && (Stack2[treeDepth].rankOnLevel <= 1) && (Stack2[treeDepth+1].rankOnLevel <= 4))
			    noTime = 1; // no time to do all from a new level

  if((kmax >= 5) && ( nodes >= (limsup * 0.33)) && (Stack2[treeDepth].rankOnLevel <= 1))
			    noTime = 1; // no time to do all from a new level
  
  if((kmax >= 5) && ( nodes >= (limsup * 0.66)) && (Stack2[treeDepth].rankOnLevel <= Stack2[treeDepth].movesOnLevel / 3))
			    noTime = 1; // no time to do all from a new level
 
  if( kmax < 5 && wstack->mateDetected > 0)	noTime = 0;

  if (noTime > 0 ) return NOTIME;

  // Level initialization

  SetStruct(); // SetStruct is setting  IsCheckOwn flags
 

  eval = Stack[gameDepth].ponder + Stack[gameDepth].positionVal;


  pond = eval;
  
  Stack2[gameDepth].flagsOfGame = 0;
  Stack2[gameDepth].bestmove = 0;
// Update thinking at every 100000 nodes generated
  if(( (nodes - gTotal) > 1000000) && 	(nodes > 100) || (buttonPressed > 0))
	{
		gTotal = nodes;

		indRedraw = 3;

		if(buttonPressed > 0)
		{
			gameDepth = treeDepth;
			noTime = 1;
			return NOTIME;
			
		}
		for(;;)
			{
			  if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
				{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			    }
			  break;
			  if(buttonPressed > 0) noTime = 0; // abandon thinking
		} // end tor
	
		CallRedrawChessBoard();

	} // generatedTotal > ...


if( ahead <= 1 || (gameDepth == treeDepth))
{
	MakePieceList();
}

wstack2->IsCheckOwn = SetIsCheck(who);

if(whoCall == 0)
    Max = -MAT ;
else
	Max = MAT ;
 
if(ahead == 1) MiniMax = Max;

anotherMoveOnLevel = 0;

wstack2->BestWay[0] = 0;



		
wstack2->movesOnLevel = 0;

wstack2->Attacks[0] = 0;

nn = 0;

if (whoCall == WHITE && (MAT-ahead)  <= alpha) return alpha; // we already know white can mate sooner 
if (whoCall == BLACK && (-MAT+ahead) >= beta ) return beta;

//
// cut extremes
//


//if(kmax < 6 || depth > 0)
{
	EvaluateAttacks(0,0,0,0,0,0);	  // setup attacks tables and estimate exchanges
}
value = GenerateAndSortMoves ( whoCall, ahead, alpha, beta);
 
if(wstack2->reps > 100) return DRAWSCORE;

if (whoCall == BLACK && (ahead < 2 || pond > beta + 350) )  eChecks[ahead]--;
if (whoCall == WHITE && (ahead < 2 || pond < alpha - 350))  eChecks[ahead]--;

if (ahead > 1 && value !=0) return value;

if(wstack2->flagsOfGame > 0) return 0;

wstack2->sortedlevel = 0;
foundValidMove = 0;
				
int val_history2;
int gasit_history2 = 0;

// Generate the move list from current position

// Analyze all moves on this level and evaluate them (if final), or call recursivelly AlphaBetaAlgorithm

wstack2->rankOnLevel = 0;
wstack2->rankAttacks   = 0;

wstack2->EffectiveAnalysed = 0;

evaluare_ok = 0;

agresivity = Options.agresivity;

int analyzed_moves = 0;

for (;;)
{
	int value_ok; // static evaluation
	if(noTime > 0) return NOTIME;


	sortbestmove = bestmove; // sortbestmove is a extern variable
	anotherMoveOnLevel = TakeOneMove(alpha,beta,ahead,depth,wstack2->sortedlevel); // MoveToDo receives this selected move

	SqFrom = (MoveToDo >> 8) & 255;
	SqTo = MoveToDo & 255;
	
	PieceMoved = (MoveToDo >> 20) & 15;
	PieceTaken = (MoveToDo >> 16) & 15;
	 
	int Indicators = (MoveToDo >> 24) & 15;

	score_must = 0;
	if(ahead == 1)
	{ 
		int xx = 0;
		if(Stack[treeDepth].nPieces < 10)
				xx = 20;
		if(Indicators == 0 && PieceMoved == WP )
			score_must += xx ;
		else if ( Indicators == 0 && PieceMoved == BP )
			score_must += - xx ;
		if(anotherMoveOnLevel == lastOptimalMove && PieceMoved < BP)
			score_must = score_must + 20;
		if(anotherMoveOnLevel == lastOptimalMove && PieceMoved >= BP)
			score_must = score_must - 20;
		
	}
	 MoveToDoOnLevel = MoveToDo;
	 if(anotherMoveOnLevel == 0) break; // no more moves
		
	 if(wstack2->IsCheckOwn == 0)
	 {
		if(PieceMoved == WK || PieceMoved == BK)
			{if( IsAttacked(SqTo,whoCall) != 0) continue;}
	
		else if ( DiscoveredCheck(whoCall,MoveToDo)) continue;
	 
	 }

	analyzed_moves++;
	
	 wstackxx2->IsCheckOwn = CheckingMove (whoCall,MoveToDo);

	 int bb = wstackxx2->IsCheckOwn;
	 MoveToDoIsCheck = bb;

	 Ext = PlayMove(MoveToDo);

	 wstackxx2->IsCheckOwn = bb;

	 recap[ahead] = Ext;
	 wstackxx2->BestWay[0] = 0;
	 MoveToDoOnLevel = MoveToDo;

	 nodes++;
		 
	 moveDo = MoveToDo;
		 
	 value = wstackxx->ponder; // MoveToDo.MoveValue;

	 nSearched = 1;
		
	 if (gameDepth > 50 && wstackxx2->IsCheckOwn == 0 && (wstackxx->nWP > 1 || wstackxx->nBP > 1))
	{
		if (whoCall == WHITE )
		{
			if (TouchMatesB (depth)  ) 
			{
				value = -MAT+ahead+3; 
				nSearched = 4; 
				nodes++;
			}
					
			if (Ext <= NO_EXT)
			{
				if (recap[ahead - 1] == SqTo &&	wstackxx->pieceAPosition[WQ][0] == SqTo ) 
					Ext = QUEENRECAP;
				
				if (depth > 1 && TouchMatesW (depth) ) 
					  Ext = MATETHREAT;
			}
							

		} // WHITE
		else
		{
			if (TouchMatesW (depth) ) 
			{
				value = MAT-ahead-3; 
				nSearched = 4; 
				nodes++; 
			}
						
			if (Ext <= NO_EXT)
			{
				if (recap[ahead - 1] == SqTo && wstackxx->pieceAPosition[BQ][0] == SqTo ) 
								  Ext = QUEENRECAP;
				if (depth > 1 && TouchMatesB (depth)  ) 
					  Ext = MATETHREAT;
			}
			
                            
		} // BLACK
	} // if
 
	 int mv = PieceMoved * 120 + SqTo;

	if (nSearched == 1)
	{  

		// Update HashValues

		HashUpdate ();
		nHashKeyAndFlags = Stack[gameDepth].HashKey ^ HashFunction[Stack2[gameDepth].enPassant] ^ HashFunction[Stack[gameDepth].WCastle + (Stack[gameDepth].BCastle<<3)];

		wstackxx2->flagsOfGame = 0;
		wstackxx2->reps = TestRepeat();
		 
		if ( wstackxx2->reps > 2 || (ahead > 2 && wstackxx2->reps > 1)  ) 
		{
			 value = DRAWSCORE; nSearched = 2;
		}
		else if ( wstackxx2->IsCheckOwn == 0 && wstackxx->nPieces <= 6 &&
			 (wstackxx->pieceCounts[WP] + wstackxx->pieceCounts[BP] == 0) &&
			 (wstackxx->pieceCounts[WQ] + wstackxx->pieceCounts[WR] + 
			  wstackxx->pieceCounts[BQ] + wstackxx->pieceCounts[BR]== 0) && abs(wstackxx->ponder) < 340 &&
			  (wstackxx->pieceCounts[WN] + wstackxx->pieceCounts[WB] <= 2) 
			  && (wstackxx->pieceCounts[BN] + wstackxx->pieceCounts[BB] <= 2))
		{
				// only minor pieces
			 value = DRAWSCORE;  nSearched = 2;
		}  

		 else if ( depth <= 1 ||  ahead > kmax + 20)
		   {
			nSearched = 0;

			if ( whoCall==WHITE && alpha > (MAT-200) ) value = alpha; // white mate detected
			else if ( whoCall==BLACK && beta < (-MAT+200) ) value = beta; // black mate detected
			else 		
			{
  
				int nextbest = NO_BEST;
				value = NOVALUE;

				if (whoCall == WHITE) 
					value = -Exchange( BLACK, -beta, -alpha, ahead+1, 0, 1);
				else 
					value =  Exchange( WHITE,  alpha, beta, ahead+1, 0, 1);
			} // else

		} // else if
	else 
		{  
			// searched = 0
			value = NOVALUE;
			nextbest = NO_BEST;
			int gg = Stack[treeDepth].nPieces;
			if(Stack[treeDepth].nPieces <= 3)
			{
				aaaa = MateRepository();
				if(aaaa != 0)
					value = aaMat;				
			}
			else	aaaa = 0;
		
		if(aaaa == 0)
		{
		ComputeExtensions(depth,ahead,Ext, nextDepth,SqTo);

		if(extensie_PV > 0)
		{
			nextDepth = nextDepth + extensie_PV;
			extensie_PV = 0;
		}
if (foundValidMove > 1 ) 
{
		
//     criterii de reducere nextdepth  la mutari ineficiente din lista Attacks

int po = wstack2->ponders[wstack2->rankOnLevel];

if(po >= 100) over_100++;
 
nrand--;

int nrand1 = (nrand)  & 7;
 

if (po < 102 && ahead == 1)
{
  cmr_penalty = 0;
  if(whoCall == WHITE  && ahead == 1) cmr_penalty =  10 - wstack2->rankOnLevel / 2;
  if(whoCall == BLACK  && ahead == 1) cmr_penalty =  - 10 + wstack2->rankOnLevel / 2;
  if ( treeDepth < 10 && whoCall == WHITE ) 	cmr_penalty += nrand & 31; 
  if ( treeDepth < 10 && whoCall == BLACK ) 	cmr_penalty -= nrand & 31; 
} 

	if (whoCall == WHITE) 
		{

			value = AlphaBetaAlgorithm( (whoCall + 1) & 1 , ahead, nextDepth, alpha, alpha+1,  nextbest, FALSE ); 

			if (value > alpha && value!=TIMEOUT )  // Fail High
			{							
				wstackxx2->bestmove = MoveToDoOnLevel;
				if (ahead == 1) 
				{ 
					MiniMax = value; 
					nBestmove = MoveToDoOnLevel;
					CurrentBest = MoveToDoOnLevel;
				}
			
				value = AlphaBetaAlgorithm(( whoCall + 1) & 1 , ahead, nextDepth, alpha, beta,  nextbest, FALSE ); 
						
				if (value > alpha || value == TIMEOUT) 
				{
					bestmove = MoveToDoOnLevel;
					wstackxx2->bestmove = MoveToDoOnLevel;
				} 
				else if  (ahead==1) 
				{
					MiniMax = value;
					CurrentBest = bestmove;
				}
			}
		}  
	else 
		{
			value = NOVALUE;
			nextbest = NO_BEST;
			int gg = Stack[treeDepth].nPieces;
			if(Stack[treeDepth].nPieces <= 3)
			{
				aaaa = MateRepository();
				if(aaaa != 0)
					value = aaMat;				
			}
			else	aaaa = 0;
		
		if(aaaa == 0)	
		{
			value = AlphaBetaAlgorithm((whoCall + 1) & 1 , ahead, nextDepth, beta-1, beta,  nextbest, FALSE ); 
		}				
			if (value < beta && value!=TIMEOUT ) 
			{
				if (ahead == 1) 
				{ 
					MiniMax = value;
					nBestmove = MoveToDoOnLevel;
					CurrentBest = MoveToDoOnLevel;
				}
				value = AlphaBetaAlgorithm((whoCall + 1) & 1 , ahead, nextDepth, alpha, beta,  nextbest,FALSE );
				  			 
				if (value < beta || value == TIMEOUT) 
				{
					bestmove = MoveToDoOnLevel;
					wstackxx2->bestmove = MoveToDoOnLevel;
				} 
				else if (ahead==1) 
				{
					MiniMax = value;
					CurrentBest = bestmove;
				}
			}
		
		} // else BLACK
	} // if found valid move
	else 
	{   
			value = NOVALUE;
			nextbest = NO_BEST;
			int gg = Stack[treeDepth].nPieces;
			if(Stack[treeDepth].nPieces <= 4)
			{
				aaaa = MateRepository();
				if(aaaa != 0)
					value = aaMat;				
			}
			else	aaaa = 0;	// when not found a valid move
		//if(aaaa == 0)
			value = AlphaBetaAlgorithm(( whoCall + 1) & 1 , ahead, nextDepth, alpha, beta,  nextbest, FALSE ); 
	}
  } // if aaaa = 0 ( nu e mat)
} // if searched = 0
     
	if (value == TIMEOUT) return TIMEOUT; // Time limit reached, fail out

	nSearched = 0;
			
	// reduce a 2nd repetition to 7/8ths
	if (wstackxx2->reps == 2 && value != DRAWSCORE && abs(value)<9000 ) 
		value = (value>>1) + (value>>2) + (value>>3); // value = 7/8 of value
                  
	} // if nsearced == 1
	// DepuneVarianta( MoveToDoOnLevel,1);
  
	MoveBack();
	foundValidMove++;
#define EPSILON 0	
		// Keep Track of Best Move and operate Alpha-Beta Prune
      if (whoCall == WHITE &&  (value + EPSILON) > Max) 
	{

		 Max = value + EPSILON;
		 bestmove = MoveToDoOnLevel;
		 Stack2[gameDepth].MaxOnLevel = Max;
		 wstackxx2->bestmove = MoveToDoOnLevel;
		 if(ahead == 1) {BestVal = Max;	MiniMax = Max;}

			 // Alpha Beta Prune
		if ( Options.level > 0 && (value + 0) >= beta  ||  value == (MAT - 1 - ahead) ) 
		{
			// DepuneVarianta( MoveToDoOnLevel,2);
			 SetHistory ( MoveToDoOnLevel, depth, ahead, whoCall, value,2);	
			 if (ahead == 1) {CurrentEval = value;}
			 if(ahead == 1) {BestVal = Max;	MiniMax = Max;}
			 rankscut[ahead-1] += wstack2->rankOnLevel;

			 return value;  // Alpha Beta cut
		}
             if (value > alpha ) 
		{
			 SaveBestWay(MoveToDoOnLevel);
			 foundValidMove++; // find a move on level
			 alpha = value; // new value as alpha, but not Alpha Beta cut
		}
	}// who == 0 (WHITE)
      
       else if (whoCall == BLACK &&  (value - EPSILON) < Max) 
       {
	
             Max = value - EPSILON;
		if(ahead == 1) {BestVal = Max;	MiniMax = Max;}

		bestmove = MoveToDoOnLevel;
		wstackxx2->bestmove = MoveToDoOnLevel;
		Stack2[gameDepth].MaxOnLevel = Max;
		// Alpha Beta Prune
		if ( Options.level > 0 && value <= alpha || value == (-MAT + 1 + ahead )  )  
		{
			// DepuneVarianta( MoveToDoOnLevel,2);
			SetHistory ( MoveToDoOnLevel, depth, ahead, whoCall, value,2);
			
			if (ahead == 1) CurrentEval = value;
			if(ahead == 1) {BestVal = Max;	MiniMax = Max;}

			rankscut[ahead-1] += wstack2->rankOnLevel;	
	
			return value; // Alpha Beta cut
		}
             if (value < beta ) 
		{
			SaveBestWay(MoveToDoOnLevel);
			foundValidMove++;
			beta = value;
		}
	} // who == 1 (BLACK)


} // end For moves list on lvel

// When no valid moves generated on level list
if (wstack2->movesOnLevel == 0 || foundValidMove == 0)
	  {
		
		// StaleMate testing: if no moves but not checked
        // Nu stiu de ce, dar aici e bine "== 1" !!!!!!!
		if(whoCall == 1)
			  {
			  if(foundValidMove == 0)
				{ 
					if(wstack2->IsCheckOwn == 0)
				  {
						Max = DRAWSCORE; // STALEMATE
						wstack2->flagsOfGame = 55; // STALEMATE
				  }
 				  return 0;  // no more analyze the Level
				}
			  }
		 else
			  {
  			  if(foundValidMove == 0)
				{
				  if(wstack2->IsCheckOwn == 0)
				  {
						Max = value = (value>>1) + (value>>2) + (value>>3); // STALEMATE
						wstack2->flagsOfGame = 55; // STALEMATE
					}

				return 0;  // no more analyze the Level
				}
			  } // else who
  
	} // end if Stack[gameDepth].movesOnLevel == 0)

	 // This return when no-alpha Beta cuts occured while move list analyzing

 if (ahead==1) {CurrentEval = Max;}
 else // StaleMate? (No moves found)
     {
	 if ( abs(Max) == 19999 ) return DRAWSCORE;
     }
	 
 SetHistory ( bestmove,  depth + 1, ahead, whoCall, Max, 1); 

return Max;
}

  

//
// Mate Reposiroty
//
int MateRepository()
{
   // test if endgame with max MaxEndPieces pieces
   aaMat = 0;

int a;
if(Stack[treeDepth ].nPieces > 4) return 0;
 
if((gameDepth - treeDepth) > 1 ) 
	{
  
	  MakePieceList();

	  aaMat = ExtendedConsultEndGame();


	   nodes++;		
 

	   if(aaMat <= 0)
		   return 0;

		consultingEnd++;
		int minadverspowerB = 0;
		int minadverspowerW = 0;
		int po = Stack[gameDepth].ponder / 100;
		  
			int k1 = Stack[gameDepth].whiteKingSquare;
			int k2 = Stack[gameDepth].blackKingSquare;
			int q1 = Stack[gameDepth].pieceAPosition[5][0];
			if(tabla[q1] != WQ) q1 = 0;
			piece1 = 5;
			if(q1 == 0)
			{ 
				q1 = Stack[gameDepth].pieceAPosition[2][0];
				if(tabla[q1] != WR) q1 = 0;
				piece1 = 2;
				minadverspowerB = 0;
			}
			int q2 = Stack[gameDepth].pieceAPosition[11][0];
			if(tabla[q2] != BQ) q2 = 0;
			int piece1n = 5;
			if(q2 == 0)
			{
				q2 = Stack[gameDepth].pieceAPosition[8][0];
				if(tabla[q2] != BR) q2 = 0;
				piece1n = 2;
				minadverspowerW = 0;
			}  
			piece2 = 0;
			int pw = Stack[gameDepth].whitePower / 100;
			int pb = Stack[gameDepth].blackPower / 100;
		if(aaMat <= 0 )
		{
return 0; 
		  if(  q2 > 0 && pw <= minadverspowerW  && DistKing[q2][k1] > 2) 
		  { // aprox mate with some pawns
			piece1 = piece1n; 
			aaMat = AccessMat4(0,Simetric[q2],Simetric[k2],Simetric[k1],0) ;
			if(( aaMat > 0) && ( aaMat < 200) ) // full mate
			{
			 	
				aaMat =  -MAT + aaMat + (gameDepth - treeDepth)  + 500 + po ; //- 1;
				return 1;
			}
		  } 
		  else if( q1 > 0 && pb <= minadverspowerB  && DistKing[q1][k2] > 2 )
		  { 
 			aaMat = AccessMat4(0,q1,k1,k2,0) ;
			if(( aaMat > 0) && ( aaMat < 200) ) // full mate
			{
				aaMat =  MAT - aaMat - (gameDepth - treeDepth)  - 500 + po; //- 1;
				return 1;
			}
		  }
		} 
		 else
		 {
			if(( aaMat > 0) && ( aaMat < 200) && who == WHITE  ) // full mate
			{
				aaMat =  MAT - aaMat - (gameDepth - treeDepth) ; //- 1;
				return 1;
			}
			if(( aaMat > 0) && ( aaMat < 200) && who == BLACK  ) // full mate
			{
				
				aaMat =  -MAT + aaMat + (gameDepth - treeDepth)  ; //- 1;
				return 1;
			}			 
		 }
return 0;		 
		 // not known MAT, but try to estimate MAT
		int di = 8 - DistKing[Stack[gameDepth].whiteKingSquare][Stack[gameDepth].blackKingSquare];
		if( di <= 1) return 0;
		
		int sqw = Stack[gameDepth].pieceAPosition[5][0];
		int diqw = 0;
		if(sqw > 0) diqw = 8 - DistKing[sqw][Stack[gameDepth].blackKingSquare];
		
		int sqb = Stack[gameDepth].pieceAPosition[11][0];
		int diqb = 0;
		if(sqb > 0) diqb = 8 - DistKing[sqb][Stack[gameDepth].whiteKingSquare];
 
		// aaMat = Stack[gameDepth].ponder + Stack[gameDepth].positonVal;
   
		if(Stack[gameDepth].blackPower == 0 && Stack[gameDepth].whitePower > 1000 ) 
			{aaMat = MAT - 1000 + po - 2* diqw - 4 * di -(gameDepth - treeDepth); return 1;}
		else if(Stack[gameDepth].blackPower == 0 && Stack[gameDepth].whitePower > 700 ) 
			{aaMat = MAT - 1000 + po - 8 * di - (gameDepth - treeDepth); return 1;}	

		if(Stack[gameDepth].whitePower == 0 && Stack[gameDepth].blackPower > 1000 ) 
			{aaMat = -MAT  + 1000 + po + 2 * diqb + 4 * di + (gameDepth - treeDepth); return 1;}	
		else if(Stack[gameDepth].whitePower == 0 && Stack[gameDepth].blackPower > 700 ) 
			{aaMat = -MAT + 1000 + po + di * 8 + (gameDepth - treeDepth); return 1;}
		   
			
	}   // if <= 4 pieces
   
	return 0;

} // MatRepository()

int pownPremiumW[11] = {0, 0, 0,   0,0,0,90,850, 0,0};
int pownPremiumB[11] = {0, 0, 850,90,0,0, 0,  0, 0,0};

// 
//  Execute Exchanges Entire Board  
//

int Exchange( int who1, int alpha, int beta, int ahead, int depth, int mod)
{
int PozitionValue;
int neededVal;
int nP;
int aaaa;
int Value;
int i;
int eval;

int xx;
int yy;
int aa;

mutari_excenge++;

if(Options.level == 0) return 100;

  if (lgMax < ahead) lgMax = ahead;
//int alpha,beta;

//alpha = alpha;
//beta  = beta;

  
int gain;

int Ext;

int SqTo;
int SqFrom;
int PieceTaken;
int PieceMoved;

int foundMoves;

struct StackElement  *wstack;
struct StackElement  *wstackxx;
struct StackElement2  *wstack2;
struct StackElement2  *wstackxx2;

wstack   = (struct StackElement *)&Stack[gameDepth];
wstackxx = (struct StackElement *)&Stack[gameDepth + 1];

wstack2  = (struct StackElement2 *)&Stack2[gameDepth];
wstackxx2= (struct StackElement2 *)&Stack2[gameDepth + 1];

wstack2->rankOnLevel = 0;

// Equivalent call for SetStruct()
if(gameDepth == treeDepth)
  CompStructBase();

if(Stack[gameDepth ].nPieces <= 4)
{
 aaaa = MateRepository();
}
else
   aaaa = 0;


if(aaaa != 0)
{
//return Value;
	pond = aaMat;
	if(who1 == BLACK) pond = -pond;
	if( pond > alpha) return pond;
	else if (pond > beta) return beta;
	else return pond;
}



if (who1 == WHITE)		eval =  EvaluateBoard ( alpha, beta );
				else	eval = -EvaluateBoard ( -beta, -alpha );

if (ahead >= MAXDEPTH) return eval;

pond = eval;

Value = pond;
//if (ahead >= MAXDEPTH || max_checks_depth >= MAXDEPTH) return Value;

// e facut la apel Quiesce ???? Stack[gameDepth].IsCheckOwn = IsCheck(who);
wstack2->BestWay[0] = 0; // no move in Best Way


//	Stack[gameDepth].IsCheckOwn = IsCheck(who1);

//	testxx();

	foundMoves = 0;
	 
	if(wstack2->IsCheckOwn > 0)
	{
		if(who1 == WHITE) 
			InCheckMoveGenW();
		else			  
			InCheckMoveGenB();
		if (wstack2->movesOnLevel == 1) max_checks_depth += 2;
		if (wstack2->movesOnLevel == 2) max_checks_depth += 1;	

		if(wstack2->movesOnLevel < 3) return CheckEvade (who1, alpha, beta, ahead, depth);
		
		if (Value >= beta ) return Value;
	 
	}
	else
	{
		if ( Value >= beta) return Value;

//		indExchange = 33;
		if(who1 == WHITE) FindMovesWhite(33);
		else			  FindMovesBlack(33);
 //aa = GenLevelMoves(33);  // aa is rezult for Alpha-Beta testing inside Ge

//		indExchange = 0;

	}
   
	if (Value > alpha) alpha = Value;

	if(wstack2->movesOnLevel == 0) return alpha;

	neededVal = (alpha - Value) - 130;
	 // If capturing to less than 2 pieces, don't do futility
	if(who1 == WHITE)
		nP =  wstack->nBP ;
	else
		nP =  wstack->nWP ;

	if (nP == 2 && neededVal > 230) neededVal = 230;
	if (nP < 2) neededVal = 0;
  

	foundMoves = 0;
	for (wstack2->rankOnLevel = 0;wstack2->rankOnLevel < wstack2->movesOnLevel;)
		{
		sortbestmove = 0; // in exchange routine don't use bestmove
		anotherMoveOnLevel = TakeOneMove(alpha,beta,ahead,depth,wstack2->sortedlevel); // MoveToDo receives this selected move
		 
		if(anotherMoveOnLevel == 0)  break; // no more moves
		nodes++;
		no2++;
		
	int mo = MoveToDo;
   
	 	
	int SqFrom = (mo >> 8) & 255;
	int SqTo = mo & 255;
	int PieceMoved = (mo >> 20) & 15;
	int PieceTaken = (mo >> 16) & 15;
	int Indicators = (mo >> 24) & 15;
		
	gain = abs(ValPiesa[PieceTaken]);
		 
	if(PieceMoved == WP) gain += pownPremiumW[ SQRANK [SqTo]];
	if(PieceMoved == BP) gain += pownPremiumB[ SQRANK [SqTo]];


	if( (gain == 100) && (gain < neededVal)) continue;


		if(wstack2->IsCheckOwn == 0)
			 {
				if((PieceMoved == WK) || (PieceMoved == BK))
					{if( IsAttacked(SqTo,who1) != 0) continue;}
				else if ( DiscoveredCheck(who1,MoveToDo)) continue;
			 }
		wstackxx2->IsCheckOwn = CheckingMove (who1, MoveToDo );
		int caz = 0;
		if(wstackxx2->IsCheckOwn == 0 && gain < neededVal) continue;
		int bb = wstackxx2->IsCheckOwn;
		MoveToDoIsCheck = bb;
		int po = wstack2->ponders[wstack2->rankOnLevel - 1]; // Stack2[gameDepth].ponders[ikk]

		// if(po < 104 && wstack2->IsCheckOwn == 0 && MoveToDoIsCheck == 0) continue; // no continue more
		
		Ext = PlayMove(MoveToDo);

//		wstack2->IsCheck = bb;
		wstackxx2->IsCheckOwn = bb;

		Stack2[gameDepth].BestWay[0] = 0;


 /*
		if(wstackxx2->IsCheckOwn == 0 && wstack2->IsCheckOwn == 0 && 
			abs(ValPiesa[PieceMoved]) > abs(ValPiesa[PieceTaken]) + 20) 
			{
			MoveBack();
			continue; // only main excenges
			}
			
*/
		foundMoves++;	
		PozitionValue = -Exchange( (who1 + 1) & 1, -beta, -alpha, ahead+1, depth+1,0);

		MoveBack();
		if (  PozitionValue >= beta) 
			{
			return PozitionValue;
			}

		if ( PozitionValue > alpha) 
			{
			alpha = PozitionValue;
			}

 	} // for

	  return alpha; 
}

//
// Check Evade
//
int CheckEvade (int color, int &alpha, int &beta, int &ahead, int &depth)
{
int PozitionValue;


struct StackElement  *wstack;
struct StackElement2  *wstack2;

wstack   = (struct StackElement *)&Stack[gameDepth];
wstack2  = (struct StackElement2 *)&Stack2[gameDepth];

	if (wstack2->movesOnLevel == 0) 
		return (-MAT + ahead + 1);

	wstack2->rankOnLevel = 0;
	for (wstack2->rankOnLevel = 0;wstack2->rankOnLevel < wstack2->movesOnLevel;)
      {
		sortbestmove = 0; // in exchange routine don't use bestmove

		 anotherMoveOnLevel = TakeOneMove(alpha,beta,ahead,depth,1); // MoveToDo receives this selected move

		// Mates

		 if(anotherMoveOnLevel == 0) break;

		 nodes++;
		 no2++;


		 Stack2[gameDepth + 1].IsCheckOwn = CheckingMove (color,MoveToDo );
		 MoveToDoIsCheck = Stack2[gameDepth + 1].IsCheckOwn;


//		 PozitionValue = MoveToDo.MoveValue;

		 PlayMove(MoveToDo);
	
		  Stack2[gameDepth].BestWay[0]= 0;

		 PozitionValue = -Exchange( (color + 1) & 1, -beta, -alpha, ahead+1, depth+1,0);

		 MoveBack();

		 if ( PozitionValue >= beta) 
		 	{
			return beta;
			}
		 if ( PozitionValue > alpha) 
			{
			alpha = PozitionValue;
			}

	  } // for

	  return alpha; 

}


int inline GenerateAndSortMoves (  const int color, const int ahead, const int alpha, const int beta)
{

// In Check, discard illegal moves
if (color == WHITE) 
		{if ( (MAT- ahead + 1) <= alpha) return alpha; // we already know white can mate sooner (value can't be > alpha)
		if (Stack2[gameDepth].IsCheckOwn==0) FindMovesWhite(0);
			else InCheckMoveGenW ();
        }
     else  {
         	if ( (-MAT+ahead - 1) >= beta) return beta; // (value can't be < beta)
			if (Stack2[gameDepth].IsCheckOwn==0) FindMovesBlack(0);
				else InCheckMoveGenB ();
           }


	if (Stack2[gameDepth].IsCheckOwn!=0) 
	{
		if ((Stack2[gameDepth].movesOnLevel) == 0) // 0 moves == CheckMate
		{
		return (color == WHITE) ? (-MAT+ahead + 1) : (MAT-ahead - 1);
		}
//	AddAtkMove(0,bestmove); NU
	return 0;
	}

// Sort Attacks by MVVLVA, put Hash move first and Killer move after the attacks. NU
// SortAttacks (); NU
// AddUnderPromos (); NU
//SortMoveList (bestmove, Stack[gameDepth].killers[0]); NU
return 0;
}
