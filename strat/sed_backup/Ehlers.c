// Strategy from "Predictive Indicators for Effective Trading Strategies"
// by John Ehlers

function run() 
{
	BarPeriod	= 1440;
	//Detrend = 8; // does it also work with the inverse curve?
	asset("SPX500");
	
	vars Osc = series(StochEhlers(series(price()),20,10,10));
	
	if(crossOver(Osc,0.8)) 
		reverseShort(1);
	if(crossUnder(Osc,0.2))
		reverseLong(1);
	
	PlotWidth = 800;
	plot("StochEhlers",Osc[0],NEW,RED);
	plot("Threshold1",.2,0,BLACK);
	plot("Threshold2",.8,0,BLACK);
	set(PLOTNOW);
}
