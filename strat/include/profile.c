// Season & Distribution Analysis //////////////////////////
#ifndef profile_c
#define profile_c

#include <default.c>

#define PDIFF	(1<<0) 
#define PMINUS	(1<<1) 
#define COLOR_AVG		0xee0000
#define COLOR_DEV		0x8888ff

function plotSeason(int n,int label,int season,var value,int type)
{
	if(!is(TESTMODE)) return; // [Test] only

	static int lastseason = 0;
	static var value0 = 0;
	if(season != lastseason) {
		value0 = value;
		lastseason = season;
	} 

	plotBar("Value",n,label,value-value0,NEW|AVG|BARS|LBL2,COLOR_AVG);	
	plotBar("StdDev",n,0,(value-value0)/4,DEV|BARS,COLOR_DEV);	

	if(type&PDIFF) value0 = value;
}

function plotYear(var value,int type)
{
	int periods = 52*5;
	LookBack = max(LookBack,periods);
	int n = (week()-1)*5 + dow()-1;
	if(n > periods) return;
	plotSeason(n,month(),year(),value,type);
}

function plotMonth(var value,int type)
{
	int periods = 22;
	LookBack = max(LookBack,periods);
	int n = (periods*tdm())/max(periods,tom());
	//int n = (periods*day())/max(periods,dom());
	if(n > periods) return;
	plotSeason(n,n,month(),value,type);
}

function plotWeek(var value,int type)
{
	int periods = (4*24 + 22);
	LookBack = max(LookBack,periods);
	int h = hour() + 24*(dow()-1);
	if(h > periods) return;
	plotSeason(h,hour(),week(),value,type);
}

function plotDay(var value,int type)
{
	int periods = 2*24;
	LookBack = max(LookBack,periods);
	int h = 2*hour() + minute()/30;
	if(h > periods) return;
	plotSeason(h,hour(),dow(),value,type);
}

// plot price difference profile
void plotPriceProfile(int bars,int type)
{
	if(!is(TESTMODE)) return; // [Test] only
	if(!bars) bars = 50;
	set(PLOTNOW+PEEK); // peek in the future 
	var vProfit;
	int count;
	for(count = 1; count < bars; count++) 
	{
		if(type&PDIFF) 
			vProfit = (price(-count-1)-price(-count))/PIP;
		else
			vProfit = (price(-count-1)-price(-1))/PIP;
		if(type&PMINUS) 
			vProfit = -vProfit;
		plotBar("Price",count,count,vProfit,NEW|AVG|BARS|LBL2,COLOR_AVG);
		plotBar("StdDev",count,0,vProfit/4,DEV|BARS,COLOR_DEV);
	}
}

// convert trade profit to pips
var toPIP(var x) { return x/TradeUnits/TradePIP; }

void plotTradeProfile(int bars)
{
	if(!is(TESTMODE) || !is(EXITRUN)) return; 
	if(!bars) bars = 50;

	var vWinMax = 0, vLossMax = 0;
	for(all_trades) { // calculate minimum & maximum profit in pips
		vWinMax = max(vWinMax,toPIP(TradeResult));
		vLossMax = max(vLossMax,-toPIP(TradeResult));
	}
	if(vWinMax == 0 && vLossMax == 0) return;
	
	var vStep;
	if(bars < 0) // bucket size in pips
		vStep = -bars;
	else
		vStep = 10*(int)((vWinMax+vLossMax)/bars/10);
	
	int n0 = ceil(vLossMax/vStep);
	for(all_trades) 
	{
		var vResult = toPIP(TradeResult);
		int n = floor(vResult/vStep);
		plotBar("Profit",2*(n+n0),n*vStep,abs(vResult),SUM|BARS|LBL2,COLOR_AVG);
		plotBar("Number",2*(n+n0)+1,0,1,SUM|BARS|AXIS2,COLOR_DEV);
	}
}

void plotMAEGraph(int bars)
{
	if(!is(TESTMODE) || !is(EXITRUN)) return; 
	if(!bars) bars = 50;

	var vMaxMAE = 0;
	for(all_trades) // calculate maximum MAE in pips
		vMaxMAE = max(vMaxMAE,TradeMAE/TradePIP);
	//printf("\nMaxMAE: %.0f",vMaxMAE);

	var vStep;
	if(bars < 0) // bucket size in pips
		vStep = -bars;
	else
		vStep = 10*(int)(vMaxMAE/bars/10);
	
	for(all_trades) 
	{
		var vResult = toPIP(TradeResult);
		var vMAE = TradeMAE/TradePIP/vStep;
		int n = floor(vMAE);
		plotBar("Profit",n,n*vStep,0,AVG|BARS|LBL2,COLOR_DEV);
		if(vResult > 0)
			plotGraph("Win",vMAE,vResult,DOT,GREEN);
		else
			plotGraph("Loss",vMAE,vResult,DOT,RED);
	}
}


//#define PTEST
#ifdef PTEST
function run()
{
	vars Price = series(price());
	vars Trend = series(LowPass(Price,1000));
	
	Stop = 100*PIP;
	TakeProfit = 100*PIP;
	if(valley(Trend)) {
		//plotPriceProfile(50,0);
		enterLong();
	} else if(peak(Trend)) {
		//plotPriceProfile(50,PMINUS);
		enterShort();
	}

	PlotWidth = 1000;
	PlotHeight1 = 320;
	PlotScale = 4;
	
	//plotDay(Equity,PDIFF); 
	//plotWeek(Equity,PDIFF); 
	//plotMonth(Equity,PDIFF); 
	//plotYear(Equity,PDIFF); 
	plotTradeProfile(50);
}
#endif
#endif