// Workshop 8: Scam Robot /////////////////////

function strategy1()
{
	if(random() > 0)
		enterLong();
	else 
		enterShort();
}

function strategy2()
{
	Stop = 200*PIP;
	TakeProfit = 10*PIP;

	if(NumOpenTotal == 0) {
		if(random() < 0)
			enterShort();
		else 
			enterLong();
	}
}

// generate a profit distribution chart
function run()
{
	BarPeriod = 1440;
	StartDate = 2012;
	NumYears = 1;
	LookBack = 0;
	asset("EUR/USD");

// run this simulation 3000 times	
	NumTotalCycles = 3000;
	strategy1();	

// plot the result of every run in a bar graph	
	if(is(EXITRUN)) {
		int Step = 250;
		int Result = floor(ProfitClosed/PIPCost/Step);
		plotBar("Profit",Result,Step*Result,1,SUM|BARS|LBL2,RED);	
	}
	
	PlotHeight1 = 320;
}