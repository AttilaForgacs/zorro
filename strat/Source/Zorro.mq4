//+------------------------------------------------------------------+
//|                                                        Zorro.mq4 |
//|                            Copyright 2013, oP group Germany GmbH |
//|                                          http://zorro-trader.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2013, oP group Germany GmbH"
#property link      "http://zorro-trader.com"

#include <stdlib.mqh>

#import "ZorroMT4.ex4"
   int ZorroInit();
   void ZorroExit();
   int ZorroRequest(double& arr[]);
   string zstring();
   void ZorroRespond(int cmd,double& arr[]);
#import

// Zorro commands
#define CMD_COMMENT	160
#define CMD_PRINT		161
#define CMD_ASSET		170
#define CMD_HISTORY	172
#define CMD_TICK		173
#define CMD_BUY		174
#define CMD_TRADE		175
#define CMD_SELL		176
#define CMD_BALANCE	177

// price slippage tolerance in pips
#define SLIPPAGE 5

// convert a real asset name (f.i. "EURUSD") to a MT4 specific name (f.i. "EURUSDi")
string assetFix(string Asset)
{
// replace asset name directly (example)
//	if(Asset == "XAGUSD") return("SILVER");
//	if(Asset == "XAUUSD") return("GOLD");

// no direct replacement -> add suffix automatically
	string s = Symbol();
	if(StringLen(s) > 6 && StringLen(Asset) <= 6) {
		s = StringSubstr(s,6,StringLen(s)-6);
		Asset = StringConcatenate(Asset,s); 
      static bool once = true;
      if(once) {
         Print("Adding ",s," to asset names");
         once = false;
      }
	}
	return(Asset);
}

// preload the asset from the MT4 server
int assetLoad(string Asset,int tf,int shift)
{
	static int attempts = 0;
   for(int i=0; i < 10; i++) {
      int bars = iBars(Asset,tf);
      if(shift > 1) {
			double c = iClose(Asset,tf,shift-1);
			if(c > 0) 
				return(bars); 
		}
      else if(bars > 0)
         return(bars);

      if(attempts < 1 || (attempts < 100 && GetLastError() == 4066)) { // ERR_HISTORY_WILL_UPDATED
      	attempts++;
         Sleep(1000); 
      } else
         return(0); 
   }
   return(0); 
}

// generate the name of a trade
string tradeName(string Asset,int op,int id)
{
	string ls = "L";
	if(op == OP_SELL) ls = "S";
	return(StringConcatenate("[",Asset,":",ls,id%10000,"]"));
}

double pointFactor(string Asset)
{
	int DigitSize = MarketInfo(Asset,MODE_DIGITS); // correction for brokers with 5 or 6 digits
	if(DigitSize == 3 || DigitSize == 5) 
		return(10);
	else if(DigitSize == 6) 
		return(100);
	else
		return(1);
}
//+------------------------------------------------------------------+
//| expert initialization function                                   |
//+------------------------------------------------------------------+
int init()
{
   if(!ZorroInit())
      return(-1);
   return(0);
}
//+------------------------------------------------------------------+
//| expert deinitialization function                                 |
//+------------------------------------------------------------------+
int deinit()
{
	ZorroExit();
   return(0);
}

//+------------------------------------------------------------------+
//| expert start function                                            |
//+------------------------------------------------------------------+
int start()
{
	static int count = 0;
	count++;
	//Comment("Tick ",count); 
   static double arr[10];
   int cmd = 1;
   while(cmd != 0) {
      cmd = ZorroRequest(arr);
      switch(cmd) 
      {
         case CMD_COMMENT: 
            Comment(zstring()); 
         case CMD_PRINT: 
            Print(zstring()); 
            break;         

         case CMD_ASSET:
         	string asset = assetFix(zstring());
         	if(!assetLoad(asset,1,0)) {
         		arr[0] = 0;
         	} else {
            	arr[0] = MarketInfo(asset,MODE_ASK); // price
            	arr[1] = arr[0] - MarketInfo(asset,MODE_BID); // spread
            	arr[2] = 0; // Volume
					double LotFactor = MarketInfo(asset,MODE_MINLOT); // correction for different lot scale
           		double Factor = pointFactor(asset); // correction for brokers with 5 digits
            	arr[3] = MarketInfo(asset,MODE_POINT) * Factor; // pip
            	arr[4] = MarketInfo(asset,MODE_TICKVALUE) * LotFactor * Factor; // pipcost
	         	double LotSize = MarketInfo(asset,MODE_LOTSIZE) * LotFactor;
	         	if(LotSize == 0.)
	              Print("Asset ",asset," Lot ",MarketInfo(asset,MODE_LOTSIZE)," Min ",LotFactor);
            	arr[5] = LotSize;
            	arr[6] = MarketInfo(asset,MODE_MARGINREQUIRED) * LotFactor; // margin
            	arr[7] = MarketInfo(asset,MODE_SWAPLONG);
            	arr[8] = MarketInfo(asset,MODE_SWAPSHORT);
					if(MarketInfo(asset,MODE_SWAPTYPE) == 0.) {
						arr[7] *= arr[4];
						arr[8] *= arr[4];
					}
				}
            ZorroRespond(cmd,arr);
            break;

         case CMD_HISTORY:
         	asset = assetFix(zstring());
            int timeframe = arr[0];
            int start = arr[1];
            int end = arr[2];
            int nTicks = arr[3];
            int shift = MathMax(0,iBarShift(asset,timeframe,start,false));
            int num = 0;
         	if(!assetLoad(asset,timeframe,shift)) {
					Print(asset," history missing (",GetLastError(),")");
       		} else {
            	shift = MathMax(0,iBarShift(asset,timeframe,end,false));
       			for(;;shift++) {
               	arr[0] = iOpen(asset,timeframe,shift);
               	arr[1] = iHigh(asset,timeframe,shift);            
               	arr[2] = iLow(asset,timeframe,shift);
               	arr[3] = iClose(asset,timeframe,shift);            
               	arr[4] = iTime(asset,timeframe,shift);
               	if(arr[0] == 0 || arr[4] == 0) {
               		Print("Missing bar at idx ",shift);
               		break; 
               	}
               	if(arr[4] < start)
               		break;
               	num++;
               	ZorroRespond(CMD_TICK,arr);
               	if(num >= nTicks)
               		break;
            	}
            }
            if(num > 1)
            	Print("Send ",num," ",asset," bars at ",TimeToStr(end,TIME_DATE|TIME_MINUTES));//," idx ",shift);
            arr[0] = num;
            ZorroRespond(cmd,arr);
            break;

         case CMD_BALANCE:
           	arr[0] = AccountBalance();
	   		arr[1] = AccountEquity() - arr[0]; 
   			arr[2] = AccountMargin();
   			arr[3] = TimeCurrent();
   			ZorroRespond(cmd,arr);
            break;

         case CMD_BUY:
            int op; 
            color c;
            double price, stop;
         	asset = assetFix(zstring());
            if(arr[1] == 0.) 
            	arr[1] = MarketInfo(asset,MODE_ASK) * 0.2; 
            if(arr[0] > 0.) {
            	op = OP_BUY; 
            	c = MediumBlue;
            	price = MarketInfo(asset,MODE_ASK);
            	stop = price - arr[1];
            } else {
               op = OP_SELL;
            	c = RoyalBlue;
               price = MarketInfo(asset,MODE_BID);
            	stop = price + arr[1];
            }
            double vol = MathAbs(arr[0]) / MarketInfo(asset,MODE_LOTSIZE);
        		double factor = pointFactor(asset); // correction for brokers with 5 digits
            int magic = arr[3];
            Print("Open ",asset," ",vol,"@",price," stop ",stop);
            arr[0] = OrderSend(asset,op,vol,price,SLIPPAGE*factor,stop,0,"Zorro",magic,0,c);
            if(arr[0] < 0.) {
             	int err = GetLastError();
            	Print("Order failed - ", ErrorDescription(err));
            	arr[0] = 0;
            } else
            	Comment("Trade ",tradeName(asset,op,arr[0])," entered");
            ZorroRespond(cmd,arr);
            break;

         case CMD_TRADE: 
         	int ticket = arr[0];
         	arr[0] = 0;
         	if(OrderSelect(ticket,SELECT_BY_TICKET)) {
         		int amount = OrderLots() * MarketInfo(OrderSymbol(),MODE_LOTSIZE);
         		if(OrderCloseTime() != 0) amount = -amount;
         		arr[0] = amount;
         		arr[1] = OrderOpenPrice();
         		arr[2] = OrderClosePrice();
         		arr[3] = OrderSwap();
         		arr[4] = OrderProfit();
	            //Print("amt ",arr[0]," op ",arr[1]," cl ",arr[2]," com ",arr[3]," pr ",arr[4]);
         	}
            ZorroRespond(cmd,arr);
            break;         

         case CMD_SELL:
         	ticket = arr[0];
     			arr[0] = 0;
            c = MediumBlue;
         	if(OrderSelect(ticket,SELECT_BY_TICKET)) {
         		double Price = OrderClosePrice();
         		if(OrderType() == OP_SELL)
            		c = RoyalBlue;
           		factor = pointFactor(OrderSymbol()); // correction for brokers with 5 digits
	            Print("Close ",OrderSymbol()," ",OrderLots(),"@",Price);
            	if(OrderCloseTime() > 0)
         			arr[0] = 1; // already closed by stop loss
         		else if(OrderClose(ticket,OrderLots(),Price,SLIPPAGE*factor,c)) {
         			arr[0] = 1;
            		Comment("Trade ",tradeName(OrderSymbol(),OrderType(),OrderLots())," closed");
         		} else {
             		err = GetLastError();
            		Print("Close at ",Price," failed - ", ErrorDescription(err));
         			arr[0] = 0;
            	}
         	}
            ZorroRespond(cmd,arr);
            break;         
      }
   }
   return(0);
}
//+------------------------------------------------------------------+

