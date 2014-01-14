// Workshop 5: Counter trend trading, optimized ////////////////
#include <profile.c>

function run()
{
	set(PARAMETERS);  // generate and use optimized parameters
	BarPeriod = 240;	// 4 hour bars
	LookBack = 500;	// required for Fisher()
	
// calculate the buy/sell signal
	vars Price = series(price());
	vars DomPeriod = series(DominantPeriod(Price,30));
	var LowPeriod = LowPass(DomPeriod,500);
	vars HP = series(HighPass(Price,LowPeriod*optimize(1,0.5,2)));
	vars Signal = series(Fisher(HP,500));
	var Threshold = optimize(1,0.5,1.5,0.1);

// buy and sell
	Stop = optimize(4,2,8) * ATR(100);
	Trail = 4*ATR(100);
	if(crossUnder(Signal,-Threshold))
		enterLong(); 
	else if(crossOver(Signal,Threshold))
		enterShort();

	PlotWidth = 600;
	PlotHeight1 = 300;
//	plotTradeProfile(-100); // plot the profit distribution
//	plotMAEGraph(-10); // plot the MAE graph
} 