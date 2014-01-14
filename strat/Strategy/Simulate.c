//script for simulating a strategy 
//by reading trades from a CSV file 

string Name = "Log\\testtrades.csv";  // name of the CSV file
#define STARTDATE 2011  // simulation start date

void replace(string str,char c1,char c2)
{
	while(*str) { 
		if(*str == c1) *str = c2;
		str++;
	}
}

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;

API(SystemTimeToVariantTime,oleaut32)
int _stdcall SystemTimeToVariantTime(SYSTEMTIME* lpSystemTime, double* pvtime);

DATE ConvertTime(int Year,int Month,int Day,int Hour,int Minute)
{
	SYSTEMTIME Time;
	memset(&Time,0,sizeof(SYSTEMTIME));
	if(Year < 50) Year += 2000;
	else if(Year < 100) year += 1900;
	Time.wYear = Year;
	Time.wMonth = Month;
	Time.wDay = Day;
	Time.wHour = Hour;
	Time.wMinute = Minute;
	DATE vTime;
	SystemTimeToVariantTime(&Time,&vTime);	
	return vTime;
}

string readTrade(string csv,
	string*	tAsset,
	string*	tType,
	int*		tLots,
	DATE*		tOpen,
	DATE*		tClose,
	var*		tProfit)
{
	string nextline = strstr(csv,"\n");
	if(nextline) nextline++;
	
	string separator = strstr(csv,";");
	if(separator) separator = ";";
	else separator = ",";
	
	*tLots = 0;	// no valid trade
	string s = strtok(csv,separator);
	if(!s) return nextline;
	if(s != "Long" && s != "Short")  // s = Algo name?
		s = strtok(0,separator);
	if(s != "Long" && s != "Short")  // invalid line?
		return nextline;

	*tType = s; 		 
	*tAsset = strtok(0,separator);
	strtok(0,separator); // ID	
	sscanf(strtok(0,separator),"%u",tLots);
	
	int Year,Month,Day,Hour,Minute;
	sscanf(strtok(0,separator),"%2u.%2u.%2u %2u:%2u",
		&Day,&Month,&Year,&Hour,&Minute);
	*tOpen = ConvertTime(Year,Month,Day,Hour,Minute);
	
	sscanf(strtok(0,separator),"%2u.%2u.%2u %2u:%2u",
		&Day,&Month,&Year,&Hour,&Minute);
	*tClose = ConvertTime(Year,Month,Day,Hour,Minute);

	strtok(NULL,separator); // Entry	
	strtok(NULL,separator); // Exit
	s = strtok(NULL,separator); // Profit
	replace(s,',','.');		// if saved with COMMA, convert to decimal point
	*tProfit = strvar(s,0,0);

//	printf("\n%s %s %i %i.%i.%i %i:%i %.2f",
//		*tAsset,*tType,*tLots,Day,Month,Year,Hour,Minute,*tProfit);

	return nextline;
}
	
void simulateTrade(
	string tAsset,string tType,int tLots,DATE tOpen,DATE tClose,var tProfit)
{
	Hedge = 2;
	if(!tLots) return;
	if(!asset(tAsset)) return;
	Lots = tLots;
	EntryDelay = 9999; // don't enter immediately
	if(tType == "Short")
		ThisTrade = enterShort();
	else if(tType == "Long")
		ThisTrade = enterLong();
	else return;
	
	if(ThisTrade) {
		ThisTrade->fProfit = tProfit;
		ThisTrade->tEntryDate = tOpen;
		ThisTrade->tExitDate = tClose;
	}
}

function run()
{
	StartDate = STARTDATE;
	LookBack = 0;
	if(is(LOOKBACK) || is(TRADING))
		return;
		
	if(!file_date(Name))
		quit("File not found!");
	string content = file_content(Name);
	string tAsset = "",tType = "";
	int tLots;
	DATE tOpen,tClose;
	var tProfit;
	
	while(content) {
		content = readTrade(content,&tAsset,&tType,&tLots,&tOpen,&tClose,&tProfit);
		simulateTrade(tAsset,tType,tLots,tOpen,tClose,tProfit);
	}

	PlotWidth = 700;
	PlotHeight1 = 320;
}