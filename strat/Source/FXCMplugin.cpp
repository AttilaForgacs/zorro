// FXCM.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


/////////////////////////////////////////////////////////////
typedef double DATE;
#include "..\include\trading.h"  // enter your path to trading.h
#import "C:\\Program Files\\CandleWorks\\FXOrder2Go\fxcore.dll"  // enter your path to fxcore.dll

#define PLUGIN_VERSION	2
#define DLLFUNC extern "C" __declspec(dllexport)
#define LOOP_MS	200	// repeat a failed command after this time (ms)
#define WAIT_MS	10000	// wait this time (ms) for success of a command
#define CONNECTED (g_pTradeDesk && g_pTradeDesk->IsLoggedIn() != 0)

/////////////////////////////////////////////////////////////
using namespace FXCore;
ICoreAutPtr g_pCore = 0;	// Trading desktop
ITradeDeskAutPtr g_pTradeDesk = 0;
static BOOL g_bCoreInstance = FALSE;
static BOOL g_bMarketOpen = FALSE;
static int g_nLoggedIn = 0;
_bstr_t g_bsAcctID = "";
_bstr_t g_bsAccount = "";

int (__cdecl *BrokerError)(const char *txt) = NULL;
int (__cdecl *BrokerProgress)(const int percent) = NULL;

void showComError(char* text,_com_error &e)
{
	_bstr_t bsError = e.Description();
	if (bsError.length() == 0) bsError = e.ErrorMessage();
	bsError = _bstr_t(text) + " - " + bsError;
	BrokerError((char*)bsError);
}

////////////////////////////////////////////////////////////////
DLLFUNC int BrokerOpen(char* Name,FARPROC fpError,FARPROC fpProgress)
{
	strcpy_s(Name,32,"FXCM");
	(FARPROC&)BrokerError = fpError;
	(FARPROC&)BrokerProgress = fpProgress;
	return PLUGIN_VERSION;
}

void FXCM_Logout() 
{
	try {
		g_pTradeDesk->Logout();
	}
	catch(_com_error e) {
		showComError("Logout",e);
	}
	g_pTradeDesk = 0;
	g_pCore = 0;
	g_bCoreInstance = FALSE;
	g_nLoggedIn = 0;
}

void FXCM_Login(char* User,char* Pwd,char* URL,char* Type,char* Account) 
{
	if(!CONNECTED) 
	{
#ifdef LOGIN_TOTAL
		g_pTradeDesk = 0;
		g_pCore = 0;
		g_bCoreInstance = FALSE;
#endif
		if(!g_bCoreInstance) {
			try {
				HRESULT hr = g_pCore.CreateInstance(__uuidof(CoreAut));
				if(FAILED(hr) || !g_pCore) {
					BrokerError("Can't open Core!");
					return;
				}
			}
			catch(_com_error e)
			{
				showComError("Core",e);
				return;
			}
			g_bCoreInstance = TRUE;
			Sleep(100);
		}
// create TradingDesktop
		if(!g_pTradeDesk) {
			try {
				g_pTradeDesk = g_pCore->CreateTradeDesk("trader");
				if(!g_pTradeDesk) {
					BrokerError("Can't open Trade Desk!");
					return;
				}
			}
			catch(_com_error e)
			{
				showComError("TradeDesk",e);
				return;
			}
			Sleep(100);
		}

		try {
// perform login
			g_pTradeDesk->Login(User,Pwd,URL,Type);
		}
		catch(_com_error e)
		{
			showComError("Login",e);
			return;
		}
		Sleep(100);
	}
	g_nLoggedIn = g_pTradeDesk->IsLoggedIn() != 0? 1 : 0;
	if(!g_nLoggedIn) return;

	try {
		FXCore::ITableAutPtr pAcctTable = g_pTradeDesk->FindMainTable("Accounts");
		unsigned long lRows = pAcctTable->GetRowCount();
		if(0 == lRows) {
// happens occasionally when the order2go login function runs amok
			BrokerError(" Can't open account");
			g_pTradeDesk->Logout();
			g_pTradeDesk = 0;
			g_nLoggedIn = 0;
			return;
		}
		size_t nPos = 0; 
		for(unsigned long l = 1; l <= lRows; ++l)
		{
			_bstr_t bsAccount = pAcctTable->CellValue(l, "AccountName");
			if(l == 1) {
				g_bsAccount = bsAccount;
				g_bsAcctID = pAcctTable->CellValue(l, "AccountID");
			}
			if(Account) {
				strcpy_s(Account+nPos,32,bsAccount.operator const char *());
				nPos += strlen(Account+nPos)+1;
			}
		}
		if(Account) Account[nPos] = 0;	// end mark
	}
	catch(_com_error e)
	{
		showComError("Account",e);
		return;
	}
}


////////////////////////////////////////////////////////////////
DLLFUNC int BrokerHistory(char* Asset,DATE tStart,DATE tEnd,int nTickMinutes,int nTicks,TICK* ticks)
{
	if(!CONNECTED || !Asset || !ticks || !nTicks) return 0;

	_bstr_t bsAsset(Asset);
	_bstr_t bsPeriod;
	if(1440 <= nTickMinutes) bsPeriod = "D1"; 
	else if(60 <= nTickMinutes) bsPeriod = "H1"; 
	else if(30 <= nTickMinutes) bsPeriod = "m30";
	else if(15 <= nTickMinutes) bsPeriod = "m15";
	else if(5 <= nTickMinutes) bsPeriod = "m5"; 
	else if(1 <= nTickMinutes) bsPeriod = "m1"; 
	else bsPeriod = "t";

	int nTry = 10;
	int nTick = 0;
	TICK* tick = ticks;
	DATE tTick = (double)nTickMinutes/(24.*60.);
	while(tStart < tEnd-tTick && nTick < nTicks)
	{
		FXCore::IMarketRateEnumAutPtr rates;
		FXCore::IMarketRateAutPtr rate;
		try {
			rates = g_pTradeDesk->GetPriceHistoryUTC(bsAsset,bsPeriod,tStart,tEnd,-1,false,true);
		}
		catch (_com_error e)
		{
			if (nTry-- > 0) continue;
			showComError("History",e);
			return 0;
		}
		int count = rates->Count; // max 300 for FXCM
		for(int i = 0; i < count && nTick < nTicks; i++, nTick++, tick++)
		{
			rate = rates->Item(count-i); // newest rate first
			if(!rate) break;
			tick->fOpen = (float)rate->AskOpen;
			tick->fClose = (float)rate->AskClose;
			tick->fHigh = (float)rate->AskHigh;
			tick->fLow = (float)rate->AskLow;
			tick->time = rate->StartDate;
		}
		if(!BrokerProgress(100*nTick/nTicks)) break;
		if(!rate || count <= 1) break;
		tEnd = (tick-1)->time;	// new end time
	}
	return nTick;
}


/////////////////////////////////////////////////////////////////////
DLLFUNC int BrokerTime(DATE *pTimeGMT)
{
	if(!CONNECTED) return 0;
	int result = 1;
	_variant_t vTrue = "Y";
	static DATE TimeOld = 0.f;
	DATE TimeGMT = 0.f;
	try {
		TimeGMT = g_pTradeDesk->ServerTime;
		if(vTrue == g_pTradeDesk->GetSystemProperty("MARKET_OPEN"))
			result = 2;
		TimeOld = TimeGMT;
	}
	catch (_com_error e) {
		result = 0;
	}
	if(pTimeGMT)
		*pTimeGMT = TimeGMT;
	return result;
}

DLLFUNC int BrokerAsset(char* Asset,double* pPrice,double* pSpread,
	double *pVolume, double *pPip, double *pPipCost, double *pMinAmount,
	double *pMargin, double *pRollLong, double *pRollShort)
{
	if(!CONNECTED) return 0;
	_bstr_t bsAsset(Asset);
	if(!pPrice) {
		try {
			g_pTradeDesk->SetOfferSubscription(bsAsset,"Enabled");
		}
		catch (_com_error e)
		{
			showComError("Subscribe",e);
			return 0;
		}
		Sleep(200);	// wait some time until the asset is subscribed
		return 1;
	} else
		*pPrice = 0.;

	try {
		ITableAutPtr pTbl = g_pTradeDesk->FindMainTable("offers");
		IRowsEnumAutPtr pOffers = pTbl->GetRows();
		for (int i = 0; i < pOffers->Count; i++)
		{
			FXCore::IRowAutPtr pRow = pOffers->Item(i+1);
			if(bsAsset != (_bstr_t)pRow->CellValue("Instrument")) continue;

			double dPrice = (double)pRow->CellValue("Ask");
			if(pPrice) *pPrice = dPrice; 
			if(pSpread) *pSpread = dPrice - (double)pRow->CellValue("Bid");
			if(pVolume) *pVolume = 0.;
			if(pPip) *pPip = (double)pRow->CellValue("PointSize");
			if(pMargin) *pMargin = (double)pRow->CellValue("MMR");
			if(pRollShort) *pRollShort = (double) pRow->CellValue("IntrS");
			if(pRollLong) *pRollLong = (double)pRow->CellValue("IntrB");
			if(pPipCost) {
				*pPipCost = (double)pRow->CellValue("PipCost");
//Hack: Order2Go has wrongly rounded PipCost for XAU
				if(bsAsset == (_bstr_t)"XAU/USD") *pPipCost *= 0.73; 
			}
			if(pMinAmount) {
				FXCore::ITradingSettingsProviderAutPtr Settings = g_pTradeDesk->TradingSettingsProvider;
				int nMinAmount = Settings->GetBaseUnitSize(bsAsset,g_bsAcctID);
				*pMinAmount = (double)nMinAmount;
			}
			return 1;
		}
	}
	catch (_com_error e) {
		showComError("Asset",e);
	}
// asset not found
	return 0;
}

DLLFUNC int BrokerAccount(char* Account,double *pdBalance,double *pdTradeVal,double *pdMarginVal)
{
	if(!CONNECTED) return 0;
	if(Account) g_bsAccount = _bstr_t(Account);
	try {
		FXCore::ITableAutPtr pAcctTable = g_pTradeDesk->FindMainTable("Accounts");
		unsigned long lRows = pAcctTable->GetRowCount();
		for (unsigned long l = 1; l <= lRows; ++l)
		{
			_bstr_t bstrName = pAcctTable->CellValue(l, "AccountName");
			if(bstrName != g_bsAccount) continue;
			g_bsAcctID = pAcctTable->CellValue(l, "AccountID");
			double dBalance = (double) pAcctTable->CellValue(l,"Balance");
			if(pdBalance) *pdBalance = dBalance;
			if(pdTradeVal) *pdTradeVal = (double)pAcctTable->CellValue(l,"Equity") - dBalance;
			if(pdMarginVal) *pdMarginVal = (double)pAcctTable->CellValue(l,"UsedMargin");
			return 1;
		}
	}
	catch (_com_error e) {
		showComError("Account",e);
	}
	Sleep(200);
	return 0;
}

DLLFUNC int BrokerBuy(char* Asset,int nAmount,double dStopDist,double *pPrice)
{
	if(!CONNECTED) return 0;
// set account ID
	double dPrice,dSpread,dPip;
	BrokerAsset(Asset,&dPrice,&dSpread,NULL,&dPip,NULL,NULL,NULL,NULL,NULL);
	if(pPrice) *pPrice = dPrice;
// calculate Stop Loss
	double dStopLimit = 0.; 
	int iTradeMode = 0;
	if(dStopDist > 0.) {
// buy order is rejected when stopdist < 10 pip or > 5000 pip
		FXCore::ITradingSettingsProviderAutPtr Settings = g_pTradeDesk->TradingSettingsProvider;
		int nMinStop = Settings->GetCondDistStopForTrade((LPCSTR)Asset);
		if(dStopDist < nMinStop*dPip)
			dStopDist = nMinStop*dPip;
		else if(dStopDist > 5000*dPip)
			dStopDist = 5000*dPip;
		if(nAmount < 0) // Short
			dStopLimit = *pPrice + dStopDist;
		else // Long
			dStopLimit = *pPrice - dStopDist - dSpread;
		iTradeMode += g_pTradeDesk->SL_STOP;
	}
   _variant_t vOrderID, vDealerInt;
	try {
		FXCore::IPermissionCheckerAutPtr checker = g_pTradeDesk->PermissionChecker;
		if(dStopLimit != 0. && checker->CanCreateStopLimitOrder(Asset) != checker->PERMISSION_ENABLED) {
			BrokerError("No stop limit permission");
			return 0;
		} else if(checker->CanCreateMarketOpenOrder(Asset) != checker->PERMISSION_ENABLED) {
			BrokerError("No market open permission");
			return 0;
		} else
			g_pTradeDesk->OpenTrade2(
				g_bsAcctID,(LPCSTR)Asset, 
				(nAmount < 0)? VARIANT_FALSE : VARIANT_TRUE, 
				abs(nAmount),0.,_bstr_t(""),1,iTradeMode,dStopLimit,0.,0, 
				&vOrderID, &vDealerInt);
   }
	catch(_com_error e) {
		showComError("Buy",e);
		return 0; 
	}

	if (vOrderID.vt != VT_BSTR) return 0;
	Sleep(LOOP_MS);

// verify the trade in the trade list
	for(int nTry = WAIT_MS/LOOP_MS; nTry; nTry--) 
	{
		try {    
			_bstr_t bsTradeID = g_pTradeDesk->GetTradeByOrder(vOrderID.bstrVal);
			ITableAutPtr pTable = g_pTradeDesk->FindMainTable("trades");
			IRowsEnumAutPtr pTrades = pTable->GetRows();
			for (int i = 0; i < pTrades->Count; i++)
			{
				IRowAutPtr pRow = pTrades->Item(i+1);
				if(bsTradeID == (_bstr_t)pRow->CellValue("TradeID")) {
					dPrice = (double)pRow->CellValue("Open");
					if(nAmount < 0) dPrice += dSpread; // Short
					return atoi((char*)bsTradeID);
				}
			}
		} 
		catch(_com_error e) {
			if(nTry > 1) { // sometimes the trades table is not yet updated
				Sleep(LOOP_MS); 
				continue;
			}
			showComError("Open",e);
			return 0;
		}
		Sleep(LOOP_MS);
	}
	return 0;
}

// returns negative amount when the trade was closed
DLLFUNC int BrokerTrade(int nTradeID,double *pOpen,double *pClose,double *pRoll,double *pProfit)
{
	if(!CONNECTED) return -1;
	int nAmount = 0;
	for(int j=0; j<20 && !nAmount; j++)
	{
		BOOL bClosed = j&1;
		try {
			ITableAutPtr pTable;
			if(bClosed) // must be either in trades or closed trades
				pTable = g_pTradeDesk->FindMainTable("closed trades");
			else 
				pTable = g_pTradeDesk->FindMainTable("trades");
			IRowsEnumAutPtr pCTrades = pTable->GetRows();
			for (int i = 0; i < pCTrades->Count; i++)
			{
				IRowAutPtr pRow = pCTrades->Item(i+1);
				_bstr_t bsTradeID = pRow->CellValue("TradeID");
				if(nTradeID == atoi((char*)bsTradeID)) {
					nAmount = (int)pRow->CellValue("Lot");
					if(bClosed) nAmount = -nAmount;
					if(pOpen) *pOpen = (double)pRow->CellValue("Open");
					if(pClose) *pClose = (double)pRow->CellValue("Close");
					if(pRoll) *pRoll = bClosed? 0. : // gives error on closed trades
						(double)pRow->CellValue("Int") + (double)pRow->CellValue("Com");
					if(pProfit) *pProfit = (double)pRow->CellValue("GrossPL");
					break;
				}
			}
			if(!nAmount) Sleep(100); // for next try; wait up to 2 seconds
		}
		catch(_com_error e) {
			showComError("Trade",e);
		}
	}
	return nAmount;
}

DLLFUNC int BrokerSell(int nTradeID,int nAmount)
{
	if(!CONNECTED) return 0;
	_bstr_t bsTradeID = _bstr_t(nTradeID);
	_variant_t vOrderID, vDealerInt;
	int nTry;
	for(nTry = 5; nTry; nTry--) 
	{
		try {    
			g_pTradeDesk->CloseTrade(bsTradeID,
				nAmount,0.,_bstr_t(""),1,&vOrderID,&vDealerInt);
		}
		catch(_com_error e) {
			if(nTry > 1) { // sometimes "There is no tradable price"
				Sleep(WAIT_MS);
				continue;
			}
			showComError("Sell",e);	// don't return here - maybe trade is already closed
		}
		break;
	}
	Sleep(LOOP_MS);

// verify closed trade in the close list
	for(nTry = WAIT_MS/LOOP_MS; nTry; nTry--) 
	{
		try {    
			ITableAutPtr pTable = g_pTradeDesk->FindMainTable("closed trades");
			IRowsEnumAutPtr pCTrades = pTable->GetRows();
			for (int i = 0; i < pCTrades->Count; i++)
			{
				IRowAutPtr pRow = pCTrades->Item(i+1);
				if(bsTradeID == (_bstr_t)pRow->CellValue("TradeID"))
					return 1;
			}
		} 
		catch(_com_error e) {
			if(nTry > 1) { // sometimes the closed trades table is not yet updated
				Sleep(LOOP_MS);
				continue;
			}
			showComError("Close",e);
			return 0;
		}
		Sleep(LOOP_MS);
	}
	return 0;
}

// 0 = test, 1 = relogin, 2 = login, -1 = logout
DLLFUNC int BrokerLogin(char* User,char* Pwd,char* Type,char* Account)
{
	if(!User)
		FXCM_Logout();
	else 
		FXCM_Login(User,Pwd,"http://www.fxcorporate.com",Type,Account);
	return g_nLoggedIn == 1? 1 : 0;
}
