// Workshop 5: Counter trend trading ///////////////////

function run()
{
	BarPeriod = 240;	// 4 hour bars
	
// calculate the buy/sell signal
	vars Price = series(price());
	vars DomPeriod = series(DominantPeriod(Price,30));
	var LowPeriod = LowPass(DomPeriod,500);
	vars HP = series(HighPass(Price,LowPeriod));
	vars Signal = series(Fisher(HP,500));
	var Threshold = 1.0;

// buy and sell
	Stop = 4*ATR(100);
	Trail = 4*ATR(100);
	if(crossUnder(Signal,-Threshold))
		enterLong(); 
	else if(crossOver(Signal,Threshold))
		enterShort();

// plot signals and thresholds
	plot("DominantPeriod",LowPeriod,NEW,BLUE);
	plot("Signal",Signal[0],NEW,RED);
	plot("Threshold1",Threshold,0,BLACK);
	plot("Threshold2",-Threshold,0,BLACK);
	PlotWidth = 600;
	PlotHeight1 = 300;
} 