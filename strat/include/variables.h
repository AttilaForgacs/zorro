// Zorro Beginner's Interface //////////////////////////
// redefine variables to less scary names //////////////

#define and &&
#define or ||
#define not !

#define PI	3.14159265359

#define Slider1 g->vSlider[1]
#define Slider2 g->vSlider[2]
#define Slider3 g->vSlider[3]

#define PlotScale g->nPlotScale	
#define PlotWidth g->nPlotWidth
#define PlotHeight1 g->nPlotHeight1
#define PlotHeight2 g->nPlotHeight2
#define PlotBars	g->nPlotBars
#define ColorDn	g->dwColorDn
#define ColorUp	g->dwColorUp
#define ColorCandle	g->dwColorCandle
#define ColorEquity	g->dwColorEquity
#define ColorDD	g->dwColorDD
#define ColorWin	g->dwColorWin
#define ColorLoss	g->dwColorLoss

#define PIP			g->asset->vPIP
#define PIPCost	g->asset->vPIPCost
#define MarginCost g->asset->vMargin
#define LotAmount	g->asset->vMinAmount
#define Spread		g->asset->vSpread
#define RollLong	g->asset->vRollLong
#define RollShort	g->asset->vRollShort
#define Slippage	g->vSlippage
#define Commission g->vCommission
#define Hedge		g->nHedge
#define Round		g->vRound

#define Lots		g->vLots
#define Margin		g->vMargin
#define Risk		g->vRisk
#define Entry		g->vEntry
#define Stop		g->vStop
#define Trail		g->vTrail
#define TrailSlope g->vTrailSlope
#define TrailStep g->vTrailStep
#define TrailLock g->vTrailLock
#define TakeProfit g->vTakeProfit
#define TimeExit	g->nTimeExit
#define TimeWait	g->nTimeWait
#define ExitTime	g->nTimeExit
#define EntryTime	g->nTimeWait
#define EntryDelay g->vEntryDelay

#define LookBack	g->nLookBack
#define UnstablePeriod	g->nUnstablePeriod
#define DataSkip	g->nDataSkip
#define DataSplit	g->nDataSplit
#define DataSlope	g->vDataSlope
#define BarPeriod g->nBarPeriod
#define BarOffset	g->nBarOffset
#define FrameOffset g->nFrameOffset
#define TimeFrame	g->nTimeFrame
#define NumBarCycles	g->numSampleCycles
#define NumSampleCycles	g->numSampleCycles
#define NumOptCycles	g->numOptCycles
#define NumWFOCycles	g->numWFOCycles
#define NumTotalCycles g->numTotalCycles
#define NumDays	g->numDays
#define NumYears	g->numYears
#define StartDate	g->nStartDate
#define EndDate	g->nEndDate
#define StartWeek g->nStartWeek
#define EndWeek	g->nEndWeek
#define UpdateDays g->nUpdateDays
#define GapDays	g->nGapDays
#define MaxBars	g->nMaxBars
#define NumBars	g->numBars
#define NumMinutes g->numMinutes
#define Bar			g->nBar
#define BarCycle	g->nSampleCycle
#define SampleCycle	g->nSampleCycle
#define OptCycle	g->nOptCycle
#define WFOCycle	g->nWFOCycle
#define TotalCycle	g->nTotalCycle
#define TradesPerBar g->nTradesPerBar

#define LoopNum1	g->nLoop[0]	
#define LoopNum2	g->nLoop[1]	
#define Loop1		g->pLoopPar[0]	
#define Loop2		g->pLoopPar[1]	
#define Cycle1		g->nLoopCycle[0]	
#define Cycle2		g->nLoopCycle[1]	
#define SelectWFO	g->nSelectWFO
#define NumParameters g->numParTrain
#define ParCycle	g->nParCycle
#define Detrend	g->nDetrend
#define Weekend	g->nWeekend
#define FuzzyRange g->vFuzzyRange
#define FuzzyLevel g->vFuzzyLevel

#define Mode		g->dwMode
#define Train		is(TRAINMODE)
#define Test		is(TESTMODE)
#define ReTrain	(is(COMMAND) && is(TRAINMODE))
#define Verbose	g->nVerbose
#define BoxState	g->nBoxState

#define Capital	g->vCapital
#define Balance	g->vBalance
#define Equity		g->vEquity
#define MarginVal	g->vMarginSum
#define RiskVal	g->vRiskSum
#define MarginMax	g->w.vMarginMax
#define TradeVal	(g->vWinVal-g->vLossVal)

#define Asset		g->asset->sName
#define AssetList	g->sAssetList
#define Algo		g->sAlgo
#define Script		g->sScript
#define Factors	g->sFactors
#define Broker		g->sBroker

#define WinLong			g->statLong->vWin
#define WinShort			g->statShort->vWin
#define WinTotal			g->w.vWin
#define LossLong			g->statLong->vLoss
#define LossShort			g->statShort->vLoss
#define LossTotal			g->w.vLoss
#define WinValLong		g->statLong->vWinVal
#define WinValShort		g->statShort->vWinVal
#define WinValTotal		g->vWinVal
#define LossValLong		g->statLong->vLossVal
#define LossValShort		g->statShort->vLossVal
#define LossValTotal		g->vLossVal
#define NumWinLong		g->statLong->numWin
#define NumWinShort		g->statShort->numWin
#define NumWinTotal		g->w.numWin
#define NumLossLong		g->statLong->numLoss
#define NumLossShort		g->statShort->numLoss
#define NumLossTotal		g->w.numLoss
#define WinStreakLong	g->statLong->nWinStreak
#define WinStreakShort	g->statShort->nWinStreak
#define WinStreakTotal	g->nWinStreak
#define LossStreakLong	g->statLong->nLossStreak
#define LossStreakShort	g->statShort->nLossStreak
#define LossStreakTotal	g->nLossStreak
#define WinStreakValLong	g->statLong->vWinStreakVal
#define WinStreakValShort	g->statShort->vWinStreakVal
#define WinStreakValTotal	g->vWinStreakVal
#define LossStreakValLong	g->statLong->vLossStreakVal
#define LossStreakValShort	g->statShort->vLossStreakVal
#define LossStreakValTotal	g->vLossStreakVal
#define NumWinningLong	g->statLong->numWinning
#define NumWinningShort	g->statShort->numWinning
#define NumLosingLong	g->statLong->numLosing
#define NumLosingShort	g->statShort->numLosing
#define NumShortTotal	g->numShort
#define NumLongTotal		g->numLong
#define NumOpenLong		(NumWinningLong+NumLosingLong)
#define NumOpenShort		(NumWinningShort+NumLosingShort)
#define NumOpenTotal		(NumShortTotal+NumLongTotal)
#define NumPendingLong	g->statLong->numPending
#define NumPendingShort	g->statShort->numPending
#define NumPendingTotal	g->numPending
#define NumTrades			g->numTrades
#define WinMaxLong		g->statLong->vWinMax
#define WinMaxShort		g->statShort->vWinMax
#define WinMaxTotal		g->w.vWinMax
#define LossMaxLong		g->statLong->vLossMax
#define LossMaxShort		g->statShort->vLossMax
#define LossMaxTotal		g->w.vLossMax
#define BalanceLong		(WinLong-LossLong)
#define BalanceShort		(WinShort-LossShort)
#define EquityLong		(BalanceLong+WinValLong-LossValLong)
#define EquityShort		(BalanceShort+WinValShort-LossValShort)
#define ProfitClosed		(WinLong-LossLong+WinShort-LossShort)
#define ProfitOpen		(WinValLong-LossValLong+WinValShort-LossValShort)
#define OptimalFLong		g->statLong->vOptimalF
#define OptimalFShort	g->statShort->vOptimalF
#define OptimalF			g->statShort->vOptimalF2
#define TrainPFLong		g->statLong->vTrainPF
#define TrainPFShort		g->statShort->vTrainPF
#define SkillLong			(g->statLong->Skill)
#define AlgoVar			(g->statShort->Skill)

#define ThisTrade			g->tr
#define TradePriceOpen	g->tr->fEntryPrice
#define TradePriceClose	g->tr->fExitPrice
#define TradeResult		g->tr->fResult
#define TradeStopLimit	g->tr->fStopLimit
#define TradeStopDiff	g->tr->fStopDiff
#define TradeProfitLimit g->tr->fProfitLimit
#define TradeTrailDiff	g->tr->fTrailDiff
#define TradeTrailSlope g->tr->fTrailSlope
#define TradeTrailStep	g->tr->fTrailStep
#define TradeTrailLock	g->tr->fTrailLock
#define TradeEntryLimit g->tr->fEntryLimit
#define TradeUnits		g->tr->fUnits
#define TradeExitTime	g->tr->nTimeExit
#define TradeTimeExit	g->tr->nTimeExit
#define TradeTime			(g->nBar-g->tr->nBarOpen)
#define TradeLots			g->tr->nLots
#define TradeID			g->tr->nID
#define TradeBarOpen		g->tr->nBarOpen
#define TradeBarClose	g->tr->nBarClose
#define TradeMAE			g->tr->fMAE
#define TradeMFE			g->tr->fMFE
#define TradeVar			(g->tr->Skill)
#define TradeAlgo			g->tr->sAlgo
#define TradeAsset		g->tr->asset->sName
#define TradeSpread		g->tr->asset->vSpread
#define TradePIP			g->tr->asset->vPIP

// for() macros
#define open_trades		g->tr = forTrade(0); g->tr; forTrade(1) // open trades only
#define all_trades		g->tr = forTrade(2); g->tr; forTrade(3) // all trades

#define TradeFlag(flag)	((g->tr->flags&(flag)) != 0)
#define TradeIsOpen		TradeFlag(TR_OPEN)
#define TradeIsPending	TradeFlag(TR_WAITBUY)
#define TradeIsClosed	TradeFlag(TR_STOPPED|TR_PROFIT|TR_TIME|TR_SOLD|TR_EXIT|TR_REMOVED|TR_REVERSED)
#define TradeIsShort		TradeFlag(TR_BID)
#define TradeIsLong		(!TradeFlag(TR_BID))
#define TradeIsPhantom	(TradeFlag(TR_PHANTOM))
#define TradeNewBar		(g->tr->nBarClose != g->nBar)

