// Update Assets.dta and/or download price history of a certain asset (or all)
// Select the account (Demo/Real) before

//#define ADD_ASSET "Copper"
// comment this in for adding a certain asset to Assets.dta

//#define ALL_ASSETS
// comment this in for updating the price history of all assets in the loop

#define PRICE_HISTORY
// comment this in for downloading the price history

#define MAX_GAP 2
// comment this in for checking the price history for 2 day gaps

function run()
{	
	if(!is(TESTMODE)) 
		quit("Click [Test] to download assets!");
	StartDate = 2013; // set date for downloading price history
	EndDate = 2013;
#ifdef MAX_GAP	
	GapDays = MAX_GAP;		// detect gaps
#else	
#ifndef PRICE_HISTORY
	NumYears = -1; // don't load prices, only update Assets.dta
#endif
#endif

	string name;
#ifdef ADD_ASSET // add single asset
	name = ADD_ASSET;	
#else 
#ifdef ALL_ASSETS // update all assets	
	while(name = loop("AUD/USD","EUR/USD","EUR/CHF","GBP/USD","GER30","NAS100",
		"SPX500","UK100","US30","NZD/USD","USD/CAD","USD/CHF","USD/JPY",
		"USOil","XAG/USD","XAU/USD"))
#endif		
#endif		
		loadHistory(name,1);
}