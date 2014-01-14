// simple gap trading system, 
// based on a publication by David Bean 
function run()
{
  BarPeriod = 30;
  LookBack = 100*24*60/BarPeriod; // 100 days lookback 

  asset("SPX500");  
  Stop = 200*PIP;

  vars Price = series(priceClose());  
  var High = dayHigh(ET,1);
  var Low = dayLow(ET,1); 
  var Close = dayClose(ET,1);

// enter a trade when the NYSE opens 
  if(High > 0 && Low > 0 && Close > 0 
    && timeOffset(ET,0,9,30) == 0)
  {
    var Avg = SMA(Price,LookBack);
    if(Price[0] > Close 
      && Price[0] < High
      && Price[0] < Avg)
      reverseShort(1);
          
    if(Price[0] < Close 
      && Price[0] > Low
      && Price[0] > Avg)
      reverseLong(1);
  }
}