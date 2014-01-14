// Spectral Analysis ///////////////////

function run()
{
	BarPeriod = 60;
	StartDate = 20130101;
	EndDate = 20130201;
	LookBack = 300; // 2 x highest Cycle
	asset("SPX500");
	
	vars Price = series(price());
	int Cycle;
	for(Cycle = 5; Cycle < 150; Cycle += 1)
		plotBar("Spectrum",Cycle,Cycle,Spectrum(Price,Cycle),
		BARS+AVG+LBL2,RED);
}
