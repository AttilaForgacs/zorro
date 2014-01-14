/////////////////////////////////////////////////////////////////////
// Trading functions
// (c) oP group 2010
/////////////////////////////////////////////////////////////////////
#ifndef functions_h
#define functions_h

// trade functions
TRADE* enterLong(function f,	// buy a long position
  var v0,var v1,var v2,var v3,var v4,var v5,var v6,var v7,...);
TRADE* enterShort(function f,	// buy a long position)
  var v0,var v1,var v2,var v3,var v4,var v5,var v6,var v7,...);
void exitLong(string name,var Limit);	// sell all open long positions
void exitShort(string name,var Limit); // sell all open short positions
void exitTrade(TRADE* tr,var Limit); // sell a particular trade position
TRADE* forTrade(int mode);	// find the next trade in a loop
TRADE* findTrade(string name);	// find the first trade with the given info
STATUS* statusLong();
STATUS* statusShort();

#define ALL		"*"			// all algos

int adviseLong(int method,var prediction,
	var s0,var s1,var s2,var s3,var s4,var s5,var s6,var s7,var s8,var s9,
	var s10,var s11,var s12,var s13,var s14,var s15,var s16,var s17,var s18,var s19,...);
int adviseShort(int method,var prediction,
	var s0,var s1,var s2,var s3,var s4,var s5,var s6,var s7,var s8,var s9,
	var s10,var s11,var s12,var s13,var s14,var s15,var s16,var s17,var s18,var s19,...);

#define DTREE		(1<<21)	// use a decision tree
#define PERCEPTRON (1<<22)	// use a perceptron
#define PATTERN	(1<<23)	// use pattern match
#define FAST		(1<<24)	// match fast
#define FUZZY		(1<<25)	// match fuzzy

int asset(string name);		// fill price curves from an asset with the given name
int algo(string name);		// set up an algo identifier

var price(int offset);
var priceOpen(int offset);
var priceClose(int offset);
var priceHigh(int offset);
var priceLow(int offset);

// date/time
int timeOffset(int zone,int days,int hour,int minute);	
#define UTC	24				// Coordinated Universal Time
#define WET	0				// Western European Time (London)
#define CET	1				// European time zone (Frankfurt)
#define ET	-5				// Eastern Time (New York)
#define AEST 10			// Australian Eastern Standard Time (Sydney, Tokyo)

#define MONDAY		1
#define TUESDAY	2
#define WEDNESDAY	3
#define THURSDAY	4
#define FRIDAY		5
#define SATURDAY	6
#define SUNDAY		7

int year(int offset);	// current year of the simulation
int month(int offset);	// current month of the simulation, 1 = January
int week(int offset);	// current week number 
int day(int offset);		// current day (1..31)
int dom(int offset);		// number of days of the current month, 28..31
int tdm(int offset);		// trading day of the current month, 1..23
int tom(int offset);		// number of trading days of the current month, 20..23
int dow(int offset);		// current day of the week: 1 = Monday, to 7 = Sunday.
int ldow(int zone, int offset); // local day of the week
int hour(int offset);	// current hour
int lhour(int zone,int offset);	// local hour in the given time zone
int minute(int offset);	// current minute
int second();				// current second
int dst(int offset);		// daylight saving (1 or 0)
BOOL workday(int offset);
BOOL frame(int offset);
int minutesAgo(int offset);

// chart
void plot(string name,var val,int type,int color); // plot a value into the chart
void plotBar(string name,int num,var label,var val,int type,int color);
void plotGraph(string name,var num,var val,int type,int color);

#define NEW		(1<<0)	// begin new chart
#define BARS	(1<<1)	// bars instead of curves
#define BAND1	(1<<2)	// upper band
#define BAND2	(BAND1+1)	// lower band
#define MAIN	(1<<3)	// print in main window
#define STATS	(1<<4)	// print statistics
#define AXIS2	(1<<5)	// plot on 2nd axis
#define SUM		(1<<6)	// add values
#define AVG		(1<<7)	// calculate average
#define LOG		(1<<8)	// use logarithmic scale
#define DEV		(1<<9)	// calculate standard deviation
#define NRM		(1<<10)	// normalize to 1
#define MINV	(1<<12)	// plot the bar minimum
#define MAXV	(1<<13)	// plot the bar maximum
#define LBL2	(1<<15)	// label only even bars
#define GRAPH	(1<<16)	// plot line/dot graph
#define LINE	(1<<17)	// plot a straight line
#define END		(1<<18)	// line end point
#define DOT		(1<<19)	// plot a colored dot
#define SQUARE	(DOT+(1<<20))
#define DIAMOND (DOT+(2<<20))
#define TRIANGLE (DOT+(3<<20))
#define TRIANGLE2 (DOT+(4<<20))
#define TRIANGLE3 (DOT+(5<<20))
#define TRIANGLE4 (DOT+(6<<20))
#define CROSS	(DOT+(8<<20))
#define CROSS2	(DOT+(9<<20))

#define RED			0xff0000
#define GREEN		0x00ff00
#define BLUE		0x0000ff
#define CYAN		0x00ffff
#define DARKBLUE	0x0000a0
#define LIGHTBLUE	0xadd8e6
#define PURPLE		0x800080
#define YELLOW		0xffff00
#define MAGENTA	0xff00ff
#define ORANGE		0xffa500
#define DARKGREEN	0x008000
#define OLIVE		0x808000
#define MAROON		0x800000
#define SILVER		0xc0c0c0
#define GREY		0x808080
#define BLACK		0x000000
#define LIGHT		0x606060
#define TRANSP		0x80000000

// filters
var Median(var* Data,int Length);
var Gauss(var* Data,int Length);
var DominantPeriod(var* Data,int Period);
var DominantPhase(var* Data,int Period);
extern var rDominantPeriod,rDominantPhase;
var HighPass1(var* Data,int Cutoff);
var HighPass(var* Data,int Cutoff);
var LowPass(var* Data,int Cutoff);
var BandPass(var* Data,int Period,var Delta);
var Spectrum(var* Data,int TimePeriod);

// fuzzy
BOOL fuzzy(var a);
BOOL eq(var a,var b);
var andF(var a,var b);
var orF(var a,var b);
var notF(var a);
var equalF(var a,var b);
var aboveF(var a,var b);
var belowF(var a,var b);
var peakF(var* a);
var valleyF(var* a);
var risingF(var* a);
var fallingF(var* a);
var crossOverF(var* a,var* b);
var crossUnderF(var* a,var* b);

// various analysis
var* series(var value,int length);	
var random();	// random value between -1 and 1
var timer();	// elapsed time in us
var round(var val,var step);
BOOL peak(var* a);
BOOL valley(var* a);
BOOL crossOver(var* a,var* b);
BOOL crossUnder(var* a,var* b);
BOOL rising(var* a);
BOOL falling(var* a);
var concavity(var* a);
void sortData(var* Data,int length);
int* sortIdx(var* Data,int length);
var polyfit(var* a,var* Data,int TimePeriod,int n,var weight);
var polynom(var* a,int n);
var detect(var* Data,int TimePeriod,var factor,var* pattern);

// various i/o
var slider(int num,int pos,int min,int max,char* name,char* tooltip);
var optimize(var val,var start,var end,var step,var tolerance);
void* loop(void* p1,...); // loop through the parameter list
void sound(string filename);	// play sound in trade mode
HWND window(string title);
void keys(string format,...);
int msg(string format,...);
int hit(int key);
void title(string format,...);
void info(string format,...);
void progress(int n1,int n2);
void quit(string text);
int memory();
int login(int mode);

// broker 
var brokerCommand(int command,DWORD parameter);
void loadHistory(string name,int period);

#define GET_TIME			5	// Last incoming tick time (last known server time).
#define GET_DIGITS		12	// Count of digits after decimal point 
#define GET_STOPLEVEL	14	// Stop level in points.
#define GET_STARTING		20	// Market starting date (usually used for futures).
#define GET_EXPIRATION	21	// Market expiration date (usually used for futures).
#define GET_TRADEALLOWED 22	// Trade is allowed for the symbol.
#define GET_MINLOT		23	// Minimum permitted amount of a lot.
#define GET_LOTSTEP		24	// Step for changing lots.
#define GET_MAXLOT		25	// Maximum permitted amount of a lot.
#define GET_MARGININIT	29	// Initial margin requirements for 1 lot.
#define GET_MARGINMAINTAIN	30	// Margin to maintain open positions calculated for 1 lot.
#define GET_MARGINHEDGED	31	// Hedged margin calculated for 1 lot.
#define GET_MARGINREQUIRED	32	// Free margin required to open 1 lot for buying.
#define GET_DELAY			41
#define GET_WAIT			42
#define GET_TYPE			50	// Asset type. 
#define GET_COMPLIANCE	51 // NFA compliance.
#define GET_NTRADES		52 // Number of open trades
#define SET_PATCH			128 // work around broker API bugs
#define SET_DELAY			128+GET_DELAY
#define SET_WAIT			128+GET_WAIT

// string/file functions
var strvar(string str,string name,var val);
int file_append(string name,string content);
int file_copy(string dest,string src);
int file_delete(string name);
long file_date(string name);
long file_length(string name);
string file_content(string name);
long file_read(string name,string content,long size);

// net access
string http_transfer(string url,string data);
int http_post(string url,string data);
int http_proxy(string proxy,int port);
long http_status(int id);
long http_result(int id,string content,long size);
int http_free(int id);
int ftp_download(string url, string path, string username, string password);
int ftp_upload(string url, string path, string username, string password);
int ftp_getdate(string url, string username, string password);
void ftp_stop();
long ftp_size();
long ftp_sent();
long ftp_timestamp();
int ftp_status();
int ftp_log(int mode);

// lite-C internal functions
int compareStrings(char* p1,char* p2);
void paraNumber(long n);
long checkLookBack(long num);
var xtest(var value);
int ytest(void* ptr);
//int tradetest(TRADE*,var,var);

#endif
