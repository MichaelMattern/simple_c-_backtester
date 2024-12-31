#include "DataModule.h"
#include "strategyEngine.h"
#include "Portfolio.h"
#include "Metrics.h"
#include "Strategies.h"
#include <iostream>
#include <vector>

void displayPerformanceMetrics(const Portfolio& portfolio, const std::vector<double>& equityCurve) {
    // Ensure the equity curve has enough data for analysis
    if (equityCurve.empty()) {
        std::cerr << "Equity curve is empty. Check your backtest or data inputs." << std::endl;
        return;
    }

    try {
        // Calculate metrics
        double sharpeRatio = Metrics::calculateSharpeRatio(portfolio.getReturns());
        double maxDrawdown = Metrics::calculateMaxDrawdown(equityCurve);
        double totalReturn = Metrics::calculateTotalReturn(equityCurve);
        double annualizedReturn = Metrics::calculateAnnualizedReturn(equityCurve, 252); // 252 trading days in a year

        // Display metrics
        std::cout << "\nPerformance Metrics:" << std::endl;
        std::cout << "---------------------" << std::endl;
        std::cout << "Sharpe Ratio: " << sharpeRatio << std::endl;
        std::cout << "Maximum Drawdown: " << maxDrawdown * 100 << "%" << std::endl;
        std::cout << "Total Return: " << totalReturn * 100 << "%" << std::endl;
        std::cout << "Annualized Return: " << annualizedReturn * 100 << "%" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error calculating metrics: " << e.what() << std::endl;
    }
}

int main() {
    // Initialize DataModule and load data
    DataModule dataModule;
    const std::string filePath = "./datasets/spy_2024.csv";
    if (!dataModule.loadTimeSeriesCSV(filePath)) {
        std::cerr << "Failed to load data from file: " << filePath << std::endl;
        return 1;
    }

    // Set up Portfolio
    Portfolio portfolio;
    portfolio.setCash(100000.0); // Starting with $100,000 in cash

    // Set up Strategy (example: Moving Average Strategy)
    const size_t shortWindow = 5;
    const size_t longWindow = 20;
    MovingAverageStrategy strategy(shortWindow, longWindow, portfolio);

    // Set up and run the Backtesting Engine
    BacktestingEngine engine;
    try {
        engine.runBacktest(dataModule, strategy, portfolio);
    }
    catch (const std::exception& e) {
        std::cerr << "Error during backtest: " << e.what() << std::endl;
        return 1;
    }

    // Generate the equity curve from portfolio value over time
    std::vector<double> equityCurve = portfolio.getEquityCurve();

    // Display performance metrics
    displayPerformanceMetrics(portfolio, equityCurve);

    return 0;
}
