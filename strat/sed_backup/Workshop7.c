// Advise Test ////////////////////////////
#include <profile.c>

function run()
{
	StartDate = 2002;
	BarPeriod = 1440; // 1 day
	Weekend = 1;      // don't merge Friday and Sunday bars
	LookBack = 3;		// only 3 bars needed
	NumWFOCycles = 5;

	set(RULES+TESTNOW);
	if(Train) {
		Hedge = 2;     // for training long + short
		TimeExit = 6;  // one week
	} else {
		Hedge = 1; 
		Stop = 100*PIP; // set exit limit
	}
	
	if(adviseLong(PATTERN+2,0,
		priceHigh(2),priceLow(2),priceClose(2),
		priceHigh(1),priceLow(1),priceClose(1),
		priceHigh(1),priceLow(1),priceClose(1),
		priceHigh(0),priceLow(0),priceClose(0)) > 50)
		reverseLong(1);
	
	if(adviseShort() > 50)
		reverseShort(1);

	PlotWidth = 600;
	PlotHeight1 = 300;
	//plotTradeProfile(40);
}
