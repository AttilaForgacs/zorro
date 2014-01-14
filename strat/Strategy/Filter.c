// Filter test ///////////////////

function run()
{
	BarPeriod = 60;
	MaxBars = 1000;
	asset(""); // dummy asset
	ColorUp = ColorDn = 0; // don't plot a price curve
	PlotHeight1 = PlotHeight2 = 200;
	PlotWidth = 800;
	LookBack = 100;
	set(PLOTNOW);
	
	vars Chirp = series(genSine(5,60));
	plot("Chirp",Chirp[0],0,BLACK);
	plot("LeadingCycle",sin(DominantPhase(Chirp,30)+PI/4),NEW,BLUE);
	plot("DominantPhase",DominantPhase(Chirp,30),NEW,BLUE);
	plot("DominantPeriod",DominantPeriod(Chirp,30),NEW,BLUE);
	plot("BandPass",BandPass(Chirp,30,0.1),NEW,BLUE);
	plot("HighPass",HighPass(Chirp,30),NEW,BLUE);
	plot("HighPass1",HighPass1(Chirp,30),NEW,BLUE);
	plot("HighPass2",HighPass2(Chirp,30),NEW,BLUE);
	plot("LowPass",LowPass(Chirp,30),NEW,BLUE);
	plot("Butterworth",Butterworth(Chirp,30),NEW,BLUE);
}

