// Price Analyis ///////////////////

var vGain, vMean, vVariance, vSkew, vKurtosis;
int N;

#define FIRST_BAR	2 // no less than 2 because we need last bar's price
#define LAST_BAR	(NumBars-1) // bar number of last run() call

function run()
{
	
	NumYears = 1;
	LookBack = FIRST_BAR;
	UnstablePeriod = 0;
	set(PEEK);

	if(is(INITRUN)) {
// reset global vars, otherwise they keep their old content when restarting
      vGain = vMean = vVariance = vSkew = vKurtosis = 0;
		return;
	}
	
	int N = NumBars-FIRST_BAR;
	var vReturn = (price(0)-price(1))/PIP;
	if(Bar == FIRST_BAR) vMean = (price(-N)-price(0))/PIP/N;

	vGain += abs(vReturn);
	var vDiff = vReturn-vMean;
	vVariance += vDiff*vDiff;
	vSkew += vDiff*vDiff*vDiff;
	vKurtosis += vDiff*vDiff*vDiff*vDiff;

	if(is(EXITRUN)) {
		vVariance /= N-1;
		var vStdDev = sqrt(vVariance);
		vSkew /= (N-1)*vStdDev*vStdDev*vStdDev;
		vKurtosis /= (N-1)*vStdDev*vStdDev*vStdDev*vStdDev;
		
		printf("\nSamples = %i",N);
		printf("\nMax possible profit = %.2f pips",vGain);
		printf("\nMean change per bar = %f pips",vMean);
		printf("\nDeviation = %f",vStdDev);
		printf("\nSkewness = %f",vSkew);
		printf("\nKurtosis = %f",vKurtosis);
	}
}