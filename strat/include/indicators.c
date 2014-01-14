// Useful Indicators ////////////////////////////
// (c) oP group 2010
/////////////////////////////////////////////////
#ifndef indicators_c
#define indicators_c

#ifdef INDICATORS_H
// Highest High, lowest Low
var HH(int Period);
var LL(int Period); 

// ATR based on SMA
var ATRS(int Period);

// Bill Williams Fractal Indicator
var FractalLow(var* Data,int Period);
var FractalHigh(var* Data,int Period);

// Donchian Channel, Donchian Oscillator
var DChannel(int Period);
var DCOsc(var* Data,int Period);

// Bollinger Bands Oscillator
var BBOsc(var* Data,int Period,var NbDev,int MAType);

// Trend Strength Index, from Engineering Returns 
var TSI(var* Data,int Period);

// High, Low, Open, Close of a trading day in a certain time zone
extern int StartMarket, EndMarket;
var dayHigh(int zone,int day);
var dayLow(int zone,int day);
var dayClose(int zone,int day);
var dayOpen(int zone,int day);
var dayPivot(int zone,int day);

// Filters 
var FIR3(var* Data);
var FIR4(var* Data);
var FIR6(var* Data);

// normalize a value to the -1..+1 range
var Normalize(var* Data,int Period);

// Fisher Transform
var Fisher(var* Data,int Period);

// Inverse Fisher Transform
var FisherInv(var* Data,int Period);

// 3-pole Butterworth filter, for super-smoothing
var Butterworth(var *Data,int Cutoff);

// 2-pole Highpass filter by John Ehlers
var HighPass2(var* Data,int CutoffPeriod); 

// Predictive Stochastic
var StochEhlers(var* Data,int TimePeriod,int CutOffLow,int CutOffHigh);

// Smoothed Momentum
var SMom(var *Data,int Period,int Cutoff);

// Zero-lag Moving Average by John Ehlers
extern var rError, rEMA;
var ZMA(var* Data,int Period);

// Center Of Gravity Oscillator by John Ehlers
var CGOsc(var* Data,int Period);

// Relative Vigor Index by John Ehlers
var RVI(int Period);

// Hull Moving Average by Alan Hull
var HMA(var* Data,int Period);

// return the number of rising/falling bars
int NumRiseFall(var* Data,int Period);
int NumWhiteBlack(var vBody,int Offset,int Period);

// Spearman trend indicator, S&C 2/2011
var Spearman(var* Data,int Period);

// Fractal Dimension
var FractalDimension(var* Data,int Period);

// statistical moment (1 = mean, 2 = variance, 3 = skewness, 4 = kurtosis)
var Moment(var* Data,int Period,int n);

// Awesome Oscillator
var AO(var* Data);

// Accelerator Oscillator: AO - SMA(AO,5)
var AC(var* Data);

// Laguerre filter
var Laguerre(var *Data, var alpha);

var Coral(var *Data);

var IBS();

var ALMA(var *Data, int TimePeriod, int sigma, var offset);

// Wave generators for filter testing
var genSine(var Period1, var Period2);
var genSquare(var Period1, var Period2);
var genNoise();

#else
// Helper functions and definitions /////////////

// percentage of a variable within a band
var percent(var a,var l,var h) { 
	if(l == h) return 50.;
	return 100.*(a-l)/(h-l); 
}

// clip away the center range of a percent value
var percentClip(var a,var border)
{
// at the lower end, return the difference to 0
	if(a < border) return a-border; 
// at the upper end, return the difference to 100
	if(a > 100.-border) return a-(100.-border);
	return 0.; // center area
} 

// convert a smoothing factor to a period
inline int smoothT(var a) { return (int)(2./a-1.); }
inline var smoothF(int period) { return 2./(period+1); }

// convert radians to degrees
#define PI	3.14159
inline var deg(var x) { return (180./PI)*x; }

// Indicators /////////////////////////////////////////
int g_count;

// Highest High
var HH(int Period) 
{ 
	var vHH = 0;
	for(g_count = 0; g_count < Period; g_count++)
		vHH = Max(vHH,priceHigh(g_count));
	return vHH;
}

//Lowest Low
var LL(int Period) 
{ 
	var vLL = 999999;
	for(g_count = 0; g_count < Period; g_count++)
		vLL = Min(vLL,priceLow(g_count));
	return vLL;
}

// Simple ATR based on SMA
var ATRS(int Period)
{
	Period = max(1,Period);
	var vATRS =  0;
	for(g_count = 0; g_count < Period; g_count++)
		vATRS = priceHigh(g_count)-priceLow(g_count);
	return vATRS/Period;
}

// Bill Williams Fractal Indicator
var FractalLow(var* Data,int Period)
{
	if(Period < 3) return *Data;
	int Center = (Period-1)/2;
	if(MinIndex(Data,Period) == Center)
		return Data[Center];
	else return 0.;
}

var FractalHigh(var* Data,int Period)
{
	if(Period < 3) return *Data;
	int Center = (Period-1)/2;
	if(MaxIndex(Data,Period) == Center)
		return Data[Center];
	else return 0.;
}

// Donchian Channel
var DChannel(int Period)
{
	rRealUpperBand = HH(Period);
	rRealLowerBand = LL(Period);
	return rRealUpperBand;
}

// Donchian Oscillator
var DCOsc(var* Data,int Period)
{
	DChannel(Period);
	return percent(Data[0],rRealLowerBand,rRealUpperBand);
}

// Bollinger Bands Oscillator
var BBOsc(var* Data,int Period,var NbDev,int MAType)
{
	BBands(Data,Period,NbDev,NbDev,MAType);
	return percent(Data[0],rRealLowerBand,rRealUpperBand);
}

// Trend Strength Index, from Engineering Returns 
var TSI(var* Data,int Period)
{
	var* ratio = series(abs(Data[0]-Data[Period])/ATR(Period),0);
	var* TSI = series(SMA(series(SMA(ratio,Period),0),10*Period),0);
	return *TSI;
}

int StartMarket = 930;
int EndMarket = 1600;

int skipWeekend(int day) 
{
	int d = dow(0)-day;
	while(d < 0) d += 7;
	if(d == SATURDAY) day += 1;
	if(d == SUNDAY) day += 2;
	return day;
}

// High, Low, Open, Close of a trading day in a certain time zone
var dayHigh(int zone,int day)
{
	if(is(INITRUN)) return 0;
	day = skipWeekend(day);
	int evening = timeOffset(zone,day,EndMarket/100,EndMarket%100);
	int morning = timeOffset(zone,day,StartMarket/100,StartMarket%100);
	if(evening >= morning) return priceHigh(evening); 
	var vHH = 0;
	for(g_count = evening; g_count <= morning; g_count++)
		vHH = Max(vHH,priceHigh(g_count));
	return vHH;
}

var dayLow(int zone,int day)
{
	if(is(INITRUN)) return 0;
	day = skipWeekend(day);
	int evening = timeOffset(zone,day,EndMarket/100,EndMarket%100);
	int morning = timeOffset(zone,day,StartMarket/100,StartMarket%100);
	if(evening >= morning) return priceLow(evening);
	var vLL = 999999;
	for(g_count = evening; g_count <= morning; g_count++)
		vLL = Min(vLL,priceLow(g_count));
	return vLL;
}

var dayClose(int zone,int day)
{
	if(is(INITRUN)) return 0;
	day = skipWeekend(day);
	return priceClose(timeOffset(zone,day,EndMarket/100,EndMarket%100)); 
}

var dayOpen(int zone,int day)
{
	if(is(INITRUN)) return 0;
	day = skipWeekend(day);
	return priceOpen(timeOffset(zone,day,StartMarket/100,StartMarket%100)); 
}

var dayPivot(int zone,int day)
{
	if(is(INITRUN)) return 0;
	return (dayHigh(zone,day)+dayLow(zone,day)+dayClose(zone,day))/3;
}

//"Internal Bar Strength"
var IBS()
{
	var Low = priceLow(0);

	if(priceHigh(0) == Low) return 1.;
	return (priceClose(0) - Low)/(priceHigh(0) - Low);
}

// Filters /////////////////////////////////////////////

var FIR3(var* Data)
{
	return (Data[0]+2.*Data[1]+Data[2])/4.;
}

var FIR4(var* Data)
{
	return (Data[0]+2.*Data[1]+2.*Data[2]+Data[3])/6.;
}

var FIR6(var* Data)
{
	return (Data[0]+2.*Data[1]+3.*Data[2]+3.*Data[3]+2.*Data[4]+Data[5])/12.;
}

// normalize a value to the -1..+1 range
var Normalize(var* Data,int Period)
{
	Period = max(2,Period);
	var vMin = MinVal(Data,Period);
	var vMax = MaxVal(Data,Period);
	if(vMax>vMin) 
		return 2.*(*Data-vMin)/(vMax-vMin) - 1.;
	else return 0.;
}

// Fisher Transform
var Fisher(var* Data,int Period)
{
	var *Value = series(0,2), *Fisher = series(0,2);
	Value[0] = 0.33*Normalize(Data,Period) + 0.67*Value[1];
	Value[0] = clamp(Value[0],-0.999,0.999);
	return Fisher[0] = 0.5*log((1.+*Value)/(1.-*Value)) + 0.5*Fisher[1];
}

// Inverse Fisher Transform
var FisherInv(var* Data)
{
	var Exp = exp(2.*Data[0]);
	return (Exp-1.)/(Exp+1.);
}

// 3-pole Butterworth filter, for super-smoothing
var Butterworth(var *Data,int Cutoff)
{
	if(!Cutoff) Cutoff = 8;
	var a = exp(-PI / Cutoff);
	var b = 2*a*cos(1.738*PI / Cutoff);
	var c = a*a;
	var c1 = b + c;
	var c2 = -(c + b*c);
	var c3 = c*c;
	var c0 = 1 - c1 - c2 - c3;

	var* Filt = series(*Data,4);
	return Filt[0] = c0*Data[0] + c1*Filt[1] + c2*Filt[2] + c3*Filt[3];
}

// Highpass filter by John F. Ehlers, converted by DdlV
// http://www.mesasoftware.com/technicalpapers.htm
var HighPass2(var* Data,int CutoffPeriod) 
{
// alpha1 = (Cosine(.707*360 / 48) + Sine (.707*360 / 48) - 1) / Cosine(.707*360 / 48);
// HP = (1 - alpha1 / 2)*(1 - alpha1 / 2)*(Close - 2*Close[1] + Close[2]) + 2*(1 - alpha1)*HP[1] - (1 - alpha1)*(1 - alpha1)*HP[2];
	if(!CutoffPeriod) CutoffPeriod = 48;
	
	var a	= (0.707*2*PI)/CutoffPeriod;
	var alpha1 = (cos(a)+sin(a)-1.)/cos(a);
	var b	= 1-alpha1/2.;
	var c	= 1-alpha1;
	
	var* HP = series(0,3);
	return HP[0] = b*b*(Data[0]-2*Data[1]+Data[2])
		+2*c*HP[1]
		-c*c*HP[2];
}

// Predictive Stochastic by John F. Ehlers, converted by DdlV
// http://www.mesasoftware.com/technicalpapers.htm
var StochEhlers(var* Data,int TimePeriod,int CutOffLow,int CutOffHigh) 
{
	if(!TimePeriod) TimePeriod = 20;
	if(!CutOffLow) CutOffLow = 10;
	if(!CutOffHigh) CutOffHigh = CutOffLow;
	checkLookBack(TimePeriod);

	var* HP = series(HighPass2(Data,CutOffHigh),3);
	var* RS = series(Butterworth(HP,CutOffLow),TimePeriod);
	
	var Highest = MaxVal(RS,TimePeriod);
	var Lowest = MinVal(RS,TimePeriod);
	var* SE = series(0,3);
	if(Highest == Lowest)
		SE[0] = 0;
	else
		SE[0] = (RS[0]-Lowest)/(Highest-Lowest);

	return Butterworth(SE,CutOffLow);
}

// Smoothed Momentum
var SMom(var *Data,int Period,int Cutoff)
{
	var *Diff = series(Data[0]-Data[Period],0);
	return Butterworth(Diff,Cutoff);
}

// Zero-lag Moving Average by John Ehlers
var rError;
var rEMA;
var ZMA(var* Data,int Period)
{
	var *vEMA = series(*Data,2);
	var *vZMA = series(*Data,2);
	var a = smoothF(Period);
	vEMA[0] = a*Data[0]+(1.-a)*vEMA[1];	
	rEMA = vEMA[0]; 

	rError = 1000000;
	var Gain,GainLimit=5,BestGain=0;
	for(Gain=-GainLimit; Gain<GainLimit; Gain += 0.1)
	{
		vZMA[0] = a*(vEMA[0] + Gain*(Data[0]-vZMA[1])) + (1-a)*vZMA[1];
		var Error = Data[0] - vZMA[0];
		if(abs(Error) < rError) {
			rError = abs(Error);
			BestGain = Gain;
		}
	}
	return vZMA[0] = a*(vEMA[0] + BestGain*(Data[0]-vZMA[1])) + (1-a)*vZMA[1];
}

// Center Of Gravity Oscillator
var CGOsc(var* Data,int Period)
{
	var Num=0., Denom=0.;
	for(g_count = 0; g_count < Period; g_count++) {
		Num += (g_count+1)*Data[g_count];
		Denom += Data[g_count];
	}
	if(Denom > 0.)
		return -Num/Denom + 0.5*(Period+1);
	else
		return 0.;	
}

// Relative Vigor Index
var RVI(int Period)
{
	var *CO = series(FIR4(series(priceClose(0)-priceOpen(0),0)),0);
	var *HL = series(FIR4(series(priceHigh(0)-priceLow(0),0)),0);
	var Denom = SMA(HL,Period);
	if(Denom == 0.) return 0.;
	return SMA(CO,Period) / Denom;
}

// Hull Moving Average
var HMA(var* data,int n)
{
	return WMA(series(2*WMA(data,0.5*n+0.5)-WMA(data,n),0),sqrt((var)n)+0.5);
}

// Awesome Oscillator
var AO(var* Data) 
{ 
	return SMA(Data,5)-SMA(Data,34); 
}

// Accelerator Oscillator: AO - SMA(AO,5)
var AC(var* Data) 
{ 
	return (4*AO(Data)-AO(Data+1)-AO(Data+2)-AO(Data+3)-AO(Data+4))/5.;
}

// Laguerre filter
var Laguerre(var *Data, var alpha)
{
   vars L = series(Data[0],8);

   L[0] = alpha*Data[0] + (1-alpha)*L[1];
   L[2] = -(1-alpha)*L[0] + L[1] + (1-alpha)*L[2+1];
   L[4] = -(1-alpha)*L[2] + L[2+1] + (1-alpha)*L[4+1];
   L[6] = -(1-alpha)*L[4] + L[4+1] + (1-alpha)*L[6+1];
   return (L[0]+2*L[2]+2*L[4]+L[6])/6;
}

var Coral(vars Data)
{
	return T3(Data,60,0.4);
}

// ALMA, Arnaud Legoux Moving Average 
// © 2013 acidburn @ Zorro Trader Forum
var ALMA(var *Data, int Period, int sigma, var offset)
{
	checkLookBack(Period);
	if(sigma == 0) sigma = 6;
	if(offset == 0.) offset = 0.85;
	Period = Max(Period,2);
	var m = floor(offset * (Period - 1));
	var s = Period / sigma;
	var alma = 0., wSum = 0.;
	int i;

	for (i = 0; i < Period; i++) {
		var w = exp(-((i - m) * (i - m)) / (2 * s * s));
		alma += Data[Period - 1 - i] * w;
		wSum += w;
	}
	
	return alma / wSum;
}

var ALMA1(var *Data, int TimePeriod) { 
	return ALMA(Data,TimePeriod,0,0.); 
}

///////////////////////////////////////////////////////////////////
// statistics

// return the number of rising/falling bars
int NumRiseFall(var* Data,int Period)
{
	checkLookBack(Period);
	int i,j;
	for(i=0; i<Period; i++)
		if(Data[i]>=Data[i+1]) break;
	for(j=0; j<Period; j++)
		if(Data[j]<=Data[j+1]) break;
	return j-i;
}

int NumWhiteBlack(var vBody,int Offset,int Period)
{
	checkLookBack(Period+Offset);
	int i,j;
	for(i=0; i<Period; i++)
		if(priceClose(i+Offset)+vBody > priceOpen(i+Offset)) break;
	for(j=0; j<Period; j++)
		if(priceClose(j+Offset)-vBody < priceOpen(j+Offset)) break;
	return j-i;
}

// Spearman trend indicator, S&C 2/2011
var Spearman(var* Data,int Period)
{
	Period = clamp(Period,2,256);
	checkLookBack(Period);
	int* Idx = sortIdx(Data,Period);
	var sum = 0;
	for(g_count = 0; g_count < Period; g_count++)
		sum += (g_count-Idx[g_count])*(g_count-Idx[g_count]);
	return 1. - 6.*sum/(Period*(Period*Period-1.));
}

// Fractal Dimension
var FractalDimension(var* Data,int Period)
{
	checkLookBack(2*Period);
	Period &= ~1;
	int Period2 = Max(1,Period/2);
	var N1 = (MaxVal(Data,Period2)-MinVal(Data,Period2))/Period2;
	var N2 = (MaxVal(Data+Period2,Period2)-MinVal(Data+Period2,Period2))/Period2;
	var N3 = (MaxVal(Data,Period)-MinVal(Data,Period))/Period;
	if(N1+N2 <= 0. || N3 <= 0.) return 1.;
	return (log(N1+N2)-log(N3))/log(2.);
}

// statistical moment (1 = mean, 2 = variance, 3 = skewness, 4 = kurtosis)
var Moment(var* Data,int Period,int n)
{
	checkLookBack(Period+1);
	var mean = SMA(Data,Period);
	if(n <= 1) return mean;
	var f,sum = 0.;
	int i,j;
	for(i=0; i<Period; i++) {
		f = Data[i]-mean;
		for(j=2; j<=n; j++)
			f *= Data[i]-mean;
		sum += f;
	}
	return sum;
}

//////////////////////////////////////////////////////////////////
// test functions

// Sine generator for filter testing
var genSine(var Period1, var Period2) 
{
	if(Period1 <= 1.) Period1 = 1.;
	if(Period2 == 0.) Period2 = Period1;
	var k = (Period2-Period1)/g->numBars;
	if(k == 0.) // constant frequency
		return 0.5 + 0.5*sin((2.*PI*g->nBar)/Period1);
	else // hyperbolic chirp
		return 0.5 + 0.5*sin(2*PI*log(1+k*g->nBar/Period1)/k);
}

// Square wave generator for filter testing
var genSquare(var Period1, var Period2) 
{
	return (genSine(Period1,Period2) < 0.5)? 0. : 1.;
}

// Noise generator for filter testing
var genNoise() 
{
	return random();
}

#endif //INDICATORS_H
#endif