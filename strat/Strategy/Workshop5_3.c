// Workshop 5: Counter trend trading, optimized ////////////////

function run()
{
	set(PARAMETERS);  // generate and use optimized parameters
	BarPeriod = 240;	// 4 hour bars
	LookBack = 500;
	StartDate = 2002;
	NumWFOCycles = 5; // activate WFO

	if(ReTrain) {
		UpdateDays = -1;	// update price data from the server 
		SelectWFO = -1;	// select the last cycle for re-optimization
	}
	
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
		//reverseLong(1); 
	else if(crossOver(Signal,Threshold))
		enterShort();
		//reverseShort(1);

	PlotWidth = 600;
	PlotHeight1 = 300;
	//set(TESTNOW+LOGFILE);
} 