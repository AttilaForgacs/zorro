// Workshop 4: Trend Trading ///////////////////
#include <profile.c>

function run()
{
	vars Price = series(price());
	vars Trend = series(LowPass(Price,1000));
//	vars Trend = series(EMA(Price,500)); // experiment with other MAs
	
	Stop = 4*ATR(100);
	
	if(valley(Trend))
		enterLong();
	else if(peak(Trend))
		enterShort();
	
// zoom in a certain date
//	StartDate = 20080920;
//	EndDate = 20081125;
//	ColorEquity = ColorDD = 0;
//	plot("Trend",Trend[0],0,RED);
//	set(PLOTNOW);

	PlotWidth = 600;
	PlotHeight1 = 300;
//	plotTradeProfile(-100); // plot the trade profit distribution
	set(LOGFILE); // log all trades
}