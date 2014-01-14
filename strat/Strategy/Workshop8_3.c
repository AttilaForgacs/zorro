// Workshop 8: Scam Robot /////////////////////

function run()
{
	BarPeriod = 1440;
	StartDate = 2012;
	NumYears = 1;
	LookBack = 0;
	asset("EUR/USD");

	TakeProfit = 10*PIP;
	Stop = 200*PIP;
	
	var ProfitGoal = 100*PIPCost*Bar;
	Lots = clamp((ProfitGoal-ProfitClosed)/(7*PIPCost),1,200); 
	
	if(NumOpenTotal == 0) {
		if(random() < 0)
			enterShort();
		else 
			enterLong();
	}
	
	set(PLOTNOW);
	ColorUp = ColorDn = ColorDD = 0;
	PlotWidth = 800;
	PlotHeight1 = 320;
}