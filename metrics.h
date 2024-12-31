#pragma once
#include <vector>
#include <string>

class Metrics {
public:
    // Calculate Sharpe Ratio
    static double calculateSharpeRatio(const std::vector<double>& returns, double riskFreeRate = 0.0);

    // Calculate Maximum Drawdown
    static double calculateMaxDrawdown(const std::vector<double>& equityCurve);

    // Calculate Total Return
    static double calculateTotalReturn(const std::vector<double>& equityCurve);

    // Calculate Annualized Return
    static double calculateAnnualizedReturn(const std::vector<double>& equityCurve, int periodsPerYear);

    // Calculate Win Rate
    static double calculateWinRate(const std::vector<double>& returns);

    // Calculate Profit Factor
    static double calculateProfitFactor(const std::vector<double>& returns);

    // Calculate Average Trade Return
    static double calculateAverageTradeReturn(const std::vector<double>& returns);

    // Calculate Sortino Ratio
    static double calculateSortinoRatio(const std::vector<double>& returns, double riskFreeRate = 0.0);

    // Calculate Calmar Ratio
    static double calculateCalmarRatio(const std::vector<double>& equityCurve, int periodsPerYear);

    // Calculate Expectancy
    static double calculateExpectancy(const std::vector<double>& returns);

    // Helper function to calculate rolling returns
    static std::vector<double> calculateRollingReturns(const std::vector<double>& equityCurve, int windowSize);
};
