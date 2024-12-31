#include "Metrics.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>

// Calculate Sharpe Ratio
double Metrics::calculateSharpeRatio(const std::vector<double>& returns, double riskFreeRate) {
    if (returns.empty()) throw std::invalid_argument("Returns vector cannot be empty.");
    double meanReturn = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double variance = 0.0;
    for (double r : returns) variance += (r - meanReturn) * (r - meanReturn);
    variance /= returns.size();
    double stdDev = std::sqrt(variance);
    return stdDev == 0.0 ? 0.0 : (meanReturn - riskFreeRate) / stdDev;
}

// Calculate Maximum Drawdown
double Metrics::calculateMaxDrawdown(const std::vector<double>& equityCurve) {
    if (equityCurve.empty()) throw std::invalid_argument("Equity curve cannot be empty.");
    double maxDrawdown = 0.0, peak = equityCurve[0];
    for (double value : equityCurve) {
        if (value > peak) peak = value;
        double drawdown = (peak - value) / peak;
        if (drawdown > maxDrawdown) maxDrawdown = drawdown;
    }
    return maxDrawdown;
}

// Calculate Total Return
double Metrics::calculateTotalReturn(const std::vector<double>& equityCurve) {
    if (equityCurve.size() < 2) throw std::invalid_argument("Equity curve must have at least two values.");
    return (equityCurve.back() - equityCurve.front()) / equityCurve.front();
}

// Calculate Annualized Return
double Metrics::calculateAnnualizedReturn(const std::vector<double>& equityCurve, int periodsPerYear) {
    double totalReturn = calculateTotalReturn(equityCurve);
    double years = static_cast<double>(equityCurve.size()) / periodsPerYear;
    return std::pow(1.0 + totalReturn, 1.0 / years) - 1.0;
}

// Calculate Win Rate
double Metrics::calculateWinRate(const std::vector<double>& returns) {
    if (returns.empty()) throw std::invalid_argument("Returns vector cannot be empty.");
    int wins = std::count_if(returns.begin(), returns.end(), [](double r) { return r > 0.0; });
    return static_cast<double>(wins) / returns.size();
}

// Calculate Profit Factor
double Metrics::calculateProfitFactor(const std::vector<double>& returns) {
    if (returns.empty()) throw std::invalid_argument("Returns vector cannot be empty.");
    double grossProfit = 0.0, grossLoss = 0.0;
    for (double r : returns) {
        if (r > 0.0) grossProfit += r;
        else grossLoss += std::abs(r);
    }
    return grossLoss == 0.0 ? 0.0 : grossProfit / grossLoss;
}

// Calculate Average Trade Return
double Metrics::calculateAverageTradeReturn(const std::vector<double>& returns) {
    if (returns.empty()) throw std::invalid_argument("Returns vector cannot be empty.");
    return std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
}

// Calculate Sortino Ratio
double Metrics::calculateSortinoRatio(const std::vector<double>& returns, double riskFreeRate) {
    if (returns.empty()) throw std::invalid_argument("Returns vector cannot be empty.");
    double meanReturn = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double downsideVariance = 0.0;
    for (double r : returns) {
        if (r < riskFreeRate) downsideVariance += (r - riskFreeRate) * (r - riskFreeRate);
    }
    double downsideDeviation = std::sqrt(downsideVariance / returns.size());
    return downsideDeviation == 0.0 ? 0.0 : (meanReturn - riskFreeRate) / downsideDeviation;
}

// Calculate Calmar Ratio
double Metrics::calculateCalmarRatio(const std::vector<double>& equityCurve, int periodsPerYear) {
    double annualizedReturn = calculateAnnualizedReturn(equityCurve, periodsPerYear);
    double maxDrawdown = calculateMaxDrawdown(equityCurve);
    return maxDrawdown == 0.0 ? 0.0 : annualizedReturn / maxDrawdown;
}

// Calculate Expectancy
double Metrics::calculateExpectancy(const std::vector<double>& returns) {
    if (returns.empty()) throw std::invalid_argument("Returns vector cannot be empty.");
    double wins = 0.0, losses = 0.0, winCount = 0, lossCount = 0;
    for (double r : returns) {
        if (r > 0.0) {
            wins += r;
            ++winCount;
        }
        else if (r < 0.0) {
            losses += r;
            ++lossCount;
        }
    }
    double avgWin = winCount > 0 ? wins / winCount : 0.0;
    double avgLoss = lossCount > 0 ? losses / lossCount : 0.0;
    double winRate = winCount / static_cast<double>(returns.size());
    double lossRate = 1.0 - winRate;
    return (winRate * avgWin) + (lossRate * avgLoss);
}

// Calculate Rolling Returns
std::vector<double> Metrics::calculateRollingReturns(const std::vector<double>& equityCurve, int windowSize) {
    if (equityCurve.size() < windowSize) throw std::invalid_argument("Equity curve size must be greater than or equal to the window size.");
    std::vector<double> rollingReturns;
    for (size_t i = 0; i <= equityCurve.size() - windowSize; ++i) {
        double start = equityCurve[i];
        double end = equityCurve[i + windowSize - 1];
        rollingReturns.push_back((end - start) / start);
    }
    return rollingReturns;
}
