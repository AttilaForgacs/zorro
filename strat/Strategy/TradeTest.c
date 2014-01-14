// Trade Every Minute for Test Purposes ///////////////////

function run()
{
	BarPeriod = 1;
	LookBack = 0;
	//StartDate = 20121201;
	//asset("EUR/USD");
	//set(NFA);
	Verbose = 14; // black box recorder
	//printf("\nBalance %.2f  Equity %.2f",Balance,Equity);

	Lots = 1;
	Stop = TakeProfit = 5*PIP;
	if(NumOpenTotal > 0) {
		exitLong();
		exitShort();
	} else {
		if(random() > 0) 
			enterLong();
		else 
			enterShort();
	} 
}