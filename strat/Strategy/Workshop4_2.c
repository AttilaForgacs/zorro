// Workshop 4: Trend Trading ///////////////////
#include <profile.c>

function run()
{
	vars Price = series(price());
	vars Trend = series(LowPass(Price,1000));
	vars Signals = series(0);
	
	Stop = 4*ATR(100);
	
	if(valley(Trend)) {
		Signals[0] = 1;
		if(Sum(Signals+1,3) == 0)  // no signals in the previous 3 bars?
			enterLong();
	} else if(peak(Trend)) {
		Signals[0] = 1;
		if(Sum(Signals+1,3) == 0)
			enterShort();
	}
	
	PlotWidth = 600;
	PlotHeight1 = 300;
//	plotTradeProfile(50);
	set(LOGFILE); // log all trades
}