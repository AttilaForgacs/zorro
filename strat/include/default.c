/////////////////////////////////////////////////////////////////////
// Default trading headers and functions
// (c) oP group 2012
/////////////////////////////////////////////////////////////////////
#ifndef default_c
#define default_c

#define PRAGMA_ZERO	// initialize local variables to zero
#define PRAGMA_POINTER // no pointer autodetection

#include <litec.h>	// common language header
#include <trading.h> // trading-specific structs
#include <variables.h> // trading variable re-definitions
#include <functions.h> // trading functions
#include <ta.h> // TA-Lib technical analysis functions

extern GLOBALS* g; // global variables struct

//////////////////////////////////////////////////////////
// often used functions
var max(var a,var b) { if(a>b) return a; return b; }
var min(var a,var b) { if(a<b) return a; return b; }
int max(int a,int b) { if(a>b) return a; return b; }
int min(int a,int b) { if(a<b) return a; return b; }

var sign(var a) { if(a>0.) return 1.; if(a<0.) return -1.; return 0.; }
int sign(int a) { if(a>0) return 1; if(a<0) return -1; return 0; }
int abs(int a) { if(a>=0) return a; return -a; }

// limit a value to a min and max value
var clamp(var a,var l,var h) { if(a<l) return l; if(a>h) return h; return a; }
var clamp(int a,int l,int h) { if(a<l) return l; if(a>h) return h; return a; }

// check if a value is inside a range
BOOL between(var a,var l,var h) { return (a>=l) && (a<=h); }
BOOL between(int a,int l,int h) { return (a>=l) && (a<=h); }
var betweenF(var a,var l,var h) { return andF(aboveF(a,l),belowF(a,h)); }
/////////////////////////////////////////////////////////////////////
// overloaded functions
var* series(var value);	
var* series();	

BOOL crossOver(var* a,var b); 
BOOL crossUnder(var* a,var b);
var crossOverF(var* a,var b); 
var crossUnderF(var* a,var b);

var price() { return price(0); }
var priceOpen() { return priceOpen(0); }
var priceClose() { return priceClose(0); }
var priceLow() { return priceLow(0); }
var priceHigh() { return priceHigh(0); }

int DominantPeriod(int Period) {	return DominantPeriod(series(price()),Period); }

void exitLong(string Name) { exitLong(Name,0); }
void exitLong() { exitLong(0,0); }
void exitShort(string Name) { exitShort(Name,0); }
void exitShort() { exitShort(0,0); }
void exitTrade(TRADE* t) { exitTrade(t,0); }
int adviseLong() { return adviseLong(0,0); }
int adviseShort() { return adviseShort(0,0); }

var optimize(var val,var start,var end,var step) { return optimize(val,start,end,step,0); }
var optimize(var val,var start,var end) { return optimize(val,start,end,0,0); }

var slider(int num) { return slider(num,-1,0,0,0,0); }
var random(var limit) { return limit*0.5*(1+random()); }
var strvar(string str,string name) { return strvar(str,name,0); }
void quit() { quit(NULL); }
var round(var val) { return round(val,0.); }

int ldow(int zone) { return ldow(zone,0); }
int lhour(int zone) { return lhour(zone,0); }
int year() { return year(0); }
int month() { return month(0); }
int week() { return week(0); }
int day() { return day(0); }
int dom() { return dom(0); }
int tdm() { return tdm(0); }
int tom() { return tom(0); }
int dow() { return dow(0); }
int hour() { return hour(0); }
int minute() { return minute(0); }
int minutesAgo() { return minutesAgo(0); }
BOOL workday() { return workday(0); }

bool is(long flag);
bool mode(long flag);
void set(long flag);
void reset(long flag);

/////////////////////////////////////////////////////////////////////
#define INDICATORS_H	// include headers only
#include <indicators.c>	// new indicator library

// overloaded indicators 
var ATR(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var MinusDI(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var PlusDI(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var MinusDM(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var PlusDM(var *Open,var *High,var *Low,var *Close,int TimePeriod);
var StdDev(var* Data,int TimePeriod);
var Variance(var* Data,int TimePeriod);
var ALMA(var *Data, int TimePeriod);

var genSine(var Period) { return genSine(Period,Period); }
var genSquare(var Period) { return genSquare(Period,Period); }

/////////////////////////////////////////////////////////////////////
int scriptVersion() { return SCRIPT_VERSION; }

// optimizing objective based on PRR
var objective()
{
	if(!NumWinTotal && !NumLossTotal) return 0.;
	var wFac = 1./sqrt(1.+NumWinTotal); 
	var lFac = 1./sqrt(1.+NumLossTotal);
	var win = WinTotal, loss = LossTotal;
// remove single outliers
	if(NumWinTotal > 2) win -= (NumWinTotal-2)*WinMaxTotal/NumWinTotal;
	if(NumLossTotal > 2) loss -= (NumLossTotal-2)*LossMaxTotal/NumLossTotal;
// return PRR
	return (1.-wFac)/(1.+lFac)*(1.+win)/(1.+loss);
}

TRADE* reverseLong(int MaxTrades)
{
// update the stops and profit targets of any open trades
	var Price = priceClose();
	if(Stop > 0 && Stop < Price/2) // stop distance 
		exitLong(0,Price-Stop);
	else if(Stop >= Price/2) // stop limit
		exitLong(0,Stop); 
	if(TakeProfit > 0 && TakeProfit < Price/2) 
		exitLong(0,-(Price+TakeProfit));
	else if(TakeProfit >= Price/2) 
		exitLong(0,-TakeProfit);

// if MaxTrades is not reached, open a new trade
	if(NumOpenLong < MaxTrades || is(TRAINMODE))
		return enterLong();
// otherwise, close any opposite trades
	else if(Hedge <= 1)
		exitShort();
	return 0;
}

TRADE* reverseShort(int MaxTrades)
{
	var Price = priceClose();
	if(Stop > 0 && Stop < Price/2) 
		exitShort(0,Price+Stop); 
	else if(Stop >= Price/2)
		exitShort(0,Stop); 
	if(TakeProfit > 0 && TakeProfit < Price/2) 
		exitShort(0,-(Price-TakeProfit));
	else if(TakeProfit >= Price/2) 
		exitShort(0,-TakeProfit);
	if(NumOpenShort < MaxTrades || is(TRAINMODE))
		return enterShort();
	else if(Hedge <= 1)
		exitLong();
	return 0;
}

TRADE* reverseLong(int MaxTrades,function f)
{
	reverseLong(0);
	if(NumOpenLong < MaxTrades)
		return enterLong(f);
	return 0;
}

TRADE* reverseShort(int MaxTrades,function f)
{
	reverseShort(0);
	if(NumOpenShort < MaxTrades)
		return enterShort(f);
	return 0;
}

#endif
