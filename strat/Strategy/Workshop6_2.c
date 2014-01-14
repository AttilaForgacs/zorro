// Workshop 6_1: Portfolio trading ///////////////////

function tradeCounterTrend()
{
	TimeFrame = 4;
	vars Price = series(price());
	vars DomPeriod = series(DominantPeriod(Price,30));
	var LowPeriod = LowPass(DomPeriod,500);
	vars HP = series(HighPass(Price,LowPeriod*optimize(1,0.5,2)));
	vars Signal = series(Fisher(HP,500));
	var Threshold = optimize(1,0.5,1.5,0.1);
	
	Stop = optimize(4,2,8) * ATR(100);
	Trail = 4*ATR(100);
	
	if(crossUnder(Signal,-Threshold)) 
		reverseLong(1); 
	else if(crossOver(Signal,Threshold)) 
		reverseShort(1);
}

function tradeTrend()
{
	TimeFrame = 1;
	vars Price = series(price());
	vars Trend = series(LowPass(Price,optimize(1000,500,1500)));

	Stop = optimize(4,2,8) * ATR(100);
	Trail = 0; 
	if(valley(Trend))
		enterLong();
	else if(peak(Trend))
		enterShort();
}

function run()
{
	set(PARAMETERS+FACTORS);  // generate and use optimized parameters and factors
	BarPeriod = 60;	// 1 hour bars
	LookBack = 2000;	// needed for Fisher()
	StartDate = 2002;
	NumWFOCycles = 10; // activate WFO
	Capital = 150;	   // reinvestment mode
	
	if(ReTrain) {
		UpdateDays = -1;	// update price data from the server 
		SelectWFO = -1;	// select the last cycle for re-optimization
	}
	
// portfolio loop
	while(asset(loop("EUR/USD","USD/JPY")))
	while(algo(loop("TRND","CNTR")))
	{
// method 1: invest the square root of the component profit		
		Margin = OptimalF * Capital * sqrt(1 + ProfitClosed/Capital);
		
// method 2: invest the square root of the portfolio profit		
		//Margin = OptimalF * Capital * sqrt(1 + (WinTotal-LossTotal)/Capital);
		
// method 3: invest 50% of OptimalF		
		//Margin = 0.5 * OptimalF * (Capital + WinTotal-LossTotal);
		
// method 4: invest 1% of your account		
		//Margin = 0.01 * (Capital + WinTotal-LossTotal);	

		if(Algo == "TRND") 
			tradeTrend();
		else if(Algo == "CNTR") 
			tradeCounterTrend();
	}
	
	PlotWidth = 600;
	PlotHeight1 = 300;
}