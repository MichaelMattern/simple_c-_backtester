#include "strategyEngine.h"
#include <iostream>

// ---------------------  Backtesting Engine Methods  -------------------------------------------

void BacktestingEngine::runBacktest(DataModule& dataModule, Strategy& strategy, Portfolio& portfolio) {
    // Notify the strategy of the start of the backtest
    std::cout << "Backtesting started..." << std::endl;
    strategy.onStart();

    // Iterate over each data point in the time series
    for (const auto& [timestamp, timeSeriesData] : dataModule.getTimeSeriesData()) {
        // Pass the TimeSeriesData object directly to the strategy
        strategy.onData(timestamp, timeSeriesData);

        // Convert TimeSeriesData to market data and update the portfolio's net worth
        portfolio.updateNetWorth(timeSeriesData.toMarketData());
    }

    // Notify the strategy of the end of the backtest
    strategy.onEnd();
    std::cout << "Backtesting completed successfully." << std::endl;
}
