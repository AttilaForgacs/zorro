#define PRAGMA_ZERO
//#include <profile.c>

bool findTrade(var Price,var Grid,bool IsShort)
{
	for(open_trades) {
		if((TradeIsShort == IsShort)
			and between(TradePriceOpen,Price-Grid,Price+Grid))
				return true;
	}
	return false;
}

function run()
{
	set(HEDGING|PARAMETERS|LOGFILE|OPENEND);	
	Capital = 10000;
	Weekend = 7; 
	
	StartDate = 20130101;	
	EndDate = 20131230;
	//NumYears = 1;
	
	BarPeriod = 30;
	LookBack = 30*100+100;
	//Verbose = 30;
	//asset("EUR/CHF");		
	Stop = 2000*PIP;	

	if(ProfitOpen+ProfitClosed < -250) { // margin call
	    //exitLong();
	    //exitShort();
	    //quit("MC!!");
   }


	
	var Grid = 20*PIP;
	TakeProfit = Grid;
	var Close = priceClose();
	
	vars Price = series(price());	
	var ma_slow = HMA(Price,60*(30/BarPeriod));
	var ma_fast = HMA(Price,12*(30/BarPeriod));	
	
	var diff = ma_fast - ma_slow;
	var mindiff = 0;
	var group_tp = 50;
	
	if(diff >= mindiff) {		
		var trades = NumOpenTotal+NumPendingTotal;
		if(trades==0) {		
			var l = 1;
			enterLong(l);
		} else if(!findTrade(Close,Grid,false)) {
			var l = 1*pow(1.618, clamp(trades-2,0,100));			 
		   enterLong(l);
		   
		   var sumlot =0.0;		   
		   var wgt_open_prices =0.0;
		   for(open_trades) {
		      sumlot += TradeLots;
		      wgt_open_prices += TradeLots * TradePriceOpen;
		   }
		   var exit_price = wgt_open_prices / sumlot;
		   
		   exitLong(0, -exit_price-group_tp*PIP);
		   
		   
		}
	}	
	

//	PlotWidth = 800;
//	PlotHeight1 = 320;
//	set(PLOTNOW|PLOTPRICE);
	
}