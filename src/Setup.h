
int aaSoundOnOff();
int ShowMate();


//
// SETUP Procedure
//

int ProcSetup()
{


   float x;

   int xcol, ylin, xd, yd;
   int p;

   int n,n1, xa,xb,ya,yb;

   int xc1,yc1;
 
    
	    xc1 = xc;
		yc1 = yc;
		
		// Test if Mouse click on some pieces on Taken Pieces, for SETUP Position
			x = ICONSIZE / 2;
			p = ICONSIZE / 2;
			if( (setup > 0 )  && 
				( xc > X0Taken) && (xc < (X0Taken + TaWidth)) &&
				( yc > (Y0Taken - 20 * ay)) && (yc < (Y0Taken + 25 * ay + TaHeight)))
			{
	// Convert from xc,yc to square number
				xc1 = xc - X0Taken;
				yc1 = yc - Y0Taken + 20 * ay;

if(xc1 < TaWidth / 3)
{
				xcol = (xc1 - (xc1  % p ) ) / x;
				xd =(xc1 % p )  ;
				if ((xd < 1) || (xd > ( p - 1))) xcol = -1;
			
				ylin = (yc1 - ( yc1 % p ) ) / x ;
				yd = (yc1 % p )  ;
				if ((yd < 1) || (yd > (p - 1))) xcol = -1;

                if((xcol < 0) || (xcol > 8) || (ylin > 1) || (ylin < 0))
				{
					Beep(400,200);
				}
				else
				{
					if(xcol == 1) pieceType = 0;
					if(xcol >= 3)
						pieceType = xcol - 2;
					if((ylin == 0)  && ( pieceType > 0))
						pieceType = pieceType + 6; // black piece
				}
}
else
{

	if( (xc > X0Taken + TaWidth / 2.6 * ax) && 
		(xc < X0Taken + TaWidth / 2.6 * ax + 100 * ax) &&
		(yc > Y0Taken - 10 * ay) && 
		(yc < Y0Taken + 10 * ay))
	{
		// Change Pieces Set
		ChangePiecesSet();
	}

	if( (xc > X0Taken + TaWidth / 2.6 * ax + 140 * ax) && 
		(xc < X0Taken + TaWidth / 2.6 * ax + 260 * ax ) &&
		(yc > Y0Taken - 10 * ay) && 
		(yc < Y0Taken + 10 * ay))
	{
		// Start Position Setup
		EnterSetupProcedure();
	}

	if( (xc > X0Taken + TaWidth / 2.6 * ax + 300 * ax) && 
		(xc < X0Taken + TaWidth / 2.6 * ax + 420 * ax ) &&
		(yc > Y0Taken - 10 * ay) && 
		(yc < Y0Taken + 10 * ay))
	{
		// Clear Chess Board
		ExecuteLoadPiecesClear();
	}

	if( (xc > X0Taken + TaWidth / 2.6 * ax + 440 * ax) && 
		(xc < X0Taken + TaWidth / 2.6 * ax + 580 * ax ) &&
		(yc > Y0Taken - 10 * ay) && 
		(yc < Y0Taken + 10 * ay))
	{
		// Setup with initial position
		ExecuteLoadPiecesInitial();
	}

	if( (xc > X0Taken + TaWidth / 2.6 * ax + 600 * ax) && 
		(xc < X0Taken + TaWidth / 2.6 * ax + 720 * ax ) &&
		(yc > Y0Taken - 10 * ay) && 
		(yc < Y0Taken + 10 * ay))
	{
		// Setup OK - End Setup Procedure
		ExecuteStopLoadPieces();
	}



}
	            InvalidateRect(hWnd, NULL, FALSE);    
	
		        return 0L;

			} // xc, yc - end pieces types detection for Chess Board Setup



            // Test if some setup buttons are pressed
			x = SQBUTTON / 2;
			if(( xc > X0Buttons) && (xc < (X0Buttons + ButtWidth)) &&
				( yc > (Y0Buttons)) && (yc < (Y0Buttons + ButtHeight)))
			{

				// Take Sound OF/OFF
				if((xc > (X0Buttons + 5 * ax )) && 
				   (xc < (X0Buttons + 10 * ax + 1 * x)) &&
				   (yc > (Y0Buttons + 100 * ay)) && 
				   (yc < (Y0Buttons + 100 * ay + 20 * ay)))
				{
					aaSoundOnOff();
				}

				// Human Play With Blacks
				if((xc > (X0Buttons + 5 * ax )) && 
					(xc < (X0Buttons + 10 * ax + 1 * x)) &&
				   (yc > (Y0Buttons + 130 * ay)) && (yc < (Y0Buttons + 130 * ay + 20 * ay)))
				{
					HumanPlayWithBlacks();
				}
				// Set Thinking On
				if((xc > (X0Buttons + 5 * ax )) && (xc < (X0Buttons + 10 * ax + 1 * x)) &&
				   (yc > (Y0Buttons + 160 * ay)) && (yc < (Y0Buttons + 160 * ay + 20 * ay)))
				{
					SetThinkingOnOff();
				}

				//  Display Taken Pieces On Off
				if((xc > (X0Buttons + 5 * ax)) && (xc < (X0Buttons + 10 * ax + 1 * x)) &&
				   (yc > (Y0Buttons + 190 * ax)) && (yc < (Y0Buttons + 190* ax + 20 * ax)))
				{
					DisplayTakenPiecesOnOff();
				}


				InvalidateRect(hWnd, NULL, FALSE);    
	
		        return 0L;
			}
    
			// Test if mouse touch in models area and pointing to an model
			x = SQBUTTON / 2;
			if(( xc > X0MoveList) && (xc < (X0MoveList + MvLstWidth)) &&
				( yc > (Y0MoveList)) && (yc < (Y0MoveList + MvLstHeight)))
			{
	
			   for(n=0;n<=nModels;n=n++)
			   {
				n1 = n % 2;

				 xa = X0MoveList + 40 * ax + n1 * 50 * ax;
				 ya = Y0MoveList + 10 * ay + 40 * (n - (n % 2)) / 2 * ay;
				 xb = X0MoveList + 40 * ax + 40 + n1 * 50 * ax;
				 yb = Y0MoveList + 10 * ay + 40 * (n - (n % 2)) / 2 * ay + 35 * ay;

				 if(( xc > xa ) && ( xc < xb) && (yc > ya) && ( yc < yb)) break;
  
			   } // end for				// Chess Board Setup Button

			   if(n >= nModels)
			   {
				   Beep(400,500);
			   }
			   else
			   {
				   Options.nModel = n;
				   sta = 1;
			   }

				InvalidateRect(hWnd, NULL, FALSE);    
	
		        return 0L;
			} // Select Models of BeckGrounds

		return 1;

}

//
// Change Pieces Set
//
int		ChangePiecesSet()
{
	IconSet++;
	if(IconSet > nrIconSet)
		IconSet = 1;
	InvalidateRect(hWnd, NULL, FALSE);    
	
	return 0L;

}


//
// Execute Enter Setup Position
//
int		EnterSetupProcedure()
{
int aa;

	aa = MessageBox(hWnd,  " Doriti Modificarea Pozitiei pe Tabla de Sah ? ", " SETARE POZITIE ",MB_YESNO);

	if(aa == 6)
	{
	// Yes, modification accepted
		setup = 1;
		setup1 = 1;
		pieceType = -1;
	}
	InvalidateRect(hWnd, NULL, FALSE);    
	
	return 0L;
}

//
// Execute Load Pieces
//
int ExecuteLoadPiecesClear()
{
int i;
int aa;


if(setup1 == 0)
{
	Beep(100,800);
	InvalidateRect(hWnd, NULL, FALSE);    
	return 0;
}
	aa = MessageBox(hWnd,  " Setati Pozitie Pornind de la Tabla Fara Piese ? ", " SETARE POZITIE ",MB_YESNO);

	if(aa == 6)
	{
		gameDepth = 0;
		treeDepth = 0;

		InitGame();

		for(i=0;i < 120; i++) 
			tabla[i] = tabla0[i];
    
		for(i=0;i < 120; i++) 
			tablaRoot[i] = tabla[i];
	
		Stack[0].RocadeIndicators = 0;

		for(i=0;i < 120; i++) 
		{
		    if( tabla[i] >= 0)
			{
				tabla[i] = 0; // clear square
				tablaRoot[i] = tabla[i];
				lastDrawnBoard[i] = 111;
			}

		}
		indRedraw = 1;
		setup1 = 1; // begins load pieces
		pieceType = -1;
	}
	else
	{
//		return 0;
	}


	InvalidateRect(hWnd, NULL, FALSE);    
	
    return 0L;

}

//
// Execute Load Pieces Initial
//
int ExecuteLoadPiecesInitial()
{
int i;
int aa;

if(setup1 == 0)
{
	Beep(100,800);
	InvalidateRect(hWnd, NULL, FALSE);    
	return 0;
}

	aa = MessageBox(hWnd,  " Setati Pozitie de la Pozitia Initiala ? ", " SETARE POZITIE ",MB_YESNO);

	if(aa == 6)
	{
		gameDepth = 0;
		treeDepth = 0;

		InitGame();

		for(i=0;i < 120; i++) 
			tabla[i] = tabla0[i];
    
		for(i=0;i < 120; i++) 
			tablaRoot[i] = tabla[i];
	
		Stack[0].RocadeIndicators = 63;

		for(i=0;i < 120; i++) 
		{
		    if( tabla[i] >= 0)
			{
				tablaRoot[i] = tabla[i];
				lastDrawnBoard[i] = 111;
			}

		}
		indRedraw = 1;
		setup1 = 1; // begins load pieces
		pieceType = -1;
	}
	else
	{
//		return 0;
	}


	InvalidateRect(hWnd, NULL, FALSE);    
	
    return 0L;

}

//
// Execute Stop Load Pieces
//
int ExecuteStopLoadPieces()
{

	OptimalMoveMoveValue = 0;


	setup = 0;
	setup1 = 0;
	pieceType = -1;
	gameDepth = 0;
	treeDepth = 0;
	Stack[gameDepth].RocadeIndicators = 0;

	WhiteToMove = 1;
	BlackToMove = 0;
	who = 0;

	return 1;
}

//
// Take BackGround
//
int aaSoundOnOff()
{
	Options.sound = (Options.sound + 1) & 1;

	bellOnOff = (bellOnOff + 1) & 1;

	if(bellOnOff == 1) Beep(1000,200);

	return 1;
}

//
// Human Play With Blacks
//
int HumanPlayWithBlacks()
{
	WhiteIsMan =  0;
	BlackIsMan =  1;

	GamePlaying(1);

	setup = 0;
	setup1 = 2;

	return 1;
}

//
// Set Thinking ON/OFF
//
int SetThinkingOnOff()
{
	Options.thinking = (Options.thinking + 1 ) & 1 ;
//	setup = 0;
	return 1;
}

//
// Diplay Taken Pieces ON/OFF
//
int DisplayTakenPiecesOnOff()
{
	Options.taken = (Options.taken + 1 ) & 1 ;
//	setup = 0;
	return 1;
}


//
// Redraw SETUP Buttons
//
int RedrawSetupButtons()
{
COLORREF co1;

    X0Buttons = X0Board + 15 * SQSIZE + 50;
    Y0Buttons = Y0Board + 4 * SQSIZE;

    ButtWidth  = 4 * SQSIZE;
    ButtHeight = 5 * SQSIZE;

	MyBarRect(X0Buttons,Y0Buttons, X0Buttons + ButtWidth, Y0Buttons + ButtHeight, RGB(200,200,255));



	
	MyTextOut(X0Buttons + 15 * ax , Y0Buttons + 40 * ay + 2, " A L T E   S E T A R I ", RGB(0,0,255));


	co1 = RGB(100,100,100);
	if(Options.sound > 0)
		co1 = RGB(0,255,0);

	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 110 * ay, 8 * ax, co1);
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 100 * ay + 2, " Sunet ON/OFF ", RGB(0,0,0));

	co1 = RGB(100,100,100);
	if(Options.blacks > 0)
		co1 = RGB(0,255,0);


	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 140 * ay, 8 * ax, co1);
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 130 * ay + 2, " Jucati cu Negrele", RGB(0,0,0));

	co1 = RGB(100,100,100);
	if(Options.thinking > 0)
		co1 = RGB(0,255,0);


	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 170 * ay, 8 * ax, co1);
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 160 * ay + 2, " Gandire ON/OFF ", RGB(0,0,0));


	co1 = RGB(100,100,100);
	if(Options.taken > 0)
		co1 = RGB(0,255,0);

	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 200 * ay, 8 * ax, co1);
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 190 * ay + 2, " Piese Luate ON/OFF ", RGB(0,0,0));

	return 1;
}
