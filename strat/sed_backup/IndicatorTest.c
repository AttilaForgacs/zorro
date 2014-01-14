// Indicatortest ///////////////////

function run()
{
	set(PLOTPRICE+PLOTNOW);
	PlotBars = 500;
	var* Price = series(price());
	
// plot Bollinger bands
	BBands(Price,30,2,2,MAType_SMA);
	plot("Bollinger1",rRealUpperBand,BAND1,0x0000CC);
	plot("Bollinger2",rRealLowerBand,BAND2,0x800000FF);
	
// plot some other indicators	
	plot("ATR (PIP)",ATR(10)/PIP,NEW,RED);
	plot("Doji",CDLDoji(),NEW,BLUE);
	plot("Fractal",FractalDimension(Price,20),NEW,RED);
}
