















function tradeTrend()
{

	TimeFrame = 1;

	var *Price = series(price());
	var *Trend = series(LowPass(Price,optimize(250,100,1000)));

	Stop = optimize(2,1,10) * ATR(100);
	
	if(strstr(Algo,":L") and valley(Trend)) 
		enterLong();
	else if(strstr(Algo,":S") and peak(Trend)) 
		enterShort();
}




function tradeCounterTrend()
{
	var *Price = series(price());
	var Threshold = optimize(1.0,0.5,2,0.1);
	var *DomPeriod = series(DominantPeriod(Price,30));
	var LowPeriod = LowPass(DomPeriod,500);
	var *HP = series(HighPass(Price,LowPeriod*optimize(1,0.5,2)));
	var *Signal = series(Fisher(HP,500));
	Stop = optimize(2,1,10) * ATR(100);

	if(strstr(Algo,":L") and crossUnder(Signal,-Threshold)) 
		enterLong(); 
	else if(strstr(Algo,":S") and crossOver(Signal,Threshold)) 
		enterShort();
}

function HuckTrend()
{
	TimeFrame = 1;
	var *Price = series(price());
	var *LP5 = series(LowPass(Price,5));
	var *LP10 = series(LowPass(Price,optimize(10,6,20))); 
	var *RSI10 = series(RSI(Price,10)); 
	Stop = optimize(5,1,10)*ATR(30);
	
	static int crossed = 0;
	if(crossOver(LP5,LP10))
		crossed = Delay;
	else if(crossUnder(LP5,LP10))
		crossed = -Delay;
		
	if(strstr(Algo,":L") and (crossed > 0 && crossOver(RSI10,50)) {
		enterLong();
		crossed = 0;
	} else if(strstr(Algo,":S") and (crossed < 0 && crossUnder(RSI10,50)) {
		enterShort();
		crossed = 0;
 	} else
		crossed -= sign(crossed);
}

function run()
{
	set(PARAMETERS+FACTORS+LOGFILE); // use optimized parameters and reinvestment factors
	
	BarPeriod = 240;	// 4 hour bars
	LookBack = 500;	// needed for Fisher()
	NumWFOCycles = 8; // activate WFO
	NumBarCycles = 4;	// 4 times oversampling
	
	if(ReTrain) {
		UpdateDays = 30;	// reload new price data from the server every 30 days
		SelectWFO = -1;	// select the last cycle for re-optimization
	}
	
	// portfolio loop
	while(asset(loop("EUR/USD","USD/CHF","GBP/USD","AUD/USD","USD/CAD","USD/JPY","USD/NZD",)))
	while(algo(loop("TRND:L","TRND:S","CNTR:L","CNTR:S","HuckTrend:L","HuckTrend:S")))
	{
		// set up the optimal margin
		if(Train)
			Lots = 1;
		else if(strstr(Algo,":L") and OptimalFLong > 0) {
			Lots = 1;			 
			Margin = clamp((WinLong-LossLong) * OptimalFLong/2, 50, 10000);
		} else if(strstr(Algo,":S") and OptimalFShort > 0) {
			Lots = 1;
			Margin = clamp((WinShort-LossShort) * OptimalFShort/2, 50, 10000);
		} else
			Lots = 0; // switch off trading

		if(strstr(Algo,"TRND")) 
			tradeTrend();
		else if(strstr(Algo,"CNTR")) 
			tradeCounterTrend();
	}
	
	PlotWidth = 1000;
	PlotHeight1 = 320;
}
