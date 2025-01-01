#pragma once
#include "DataModule.h"
#include "portfolio.h"
#include <deque>
#include <numeric>
#include <iostream>
#include <stdexcept>

class Strategy {
public:
    Strategy() = default;
    virtual ~Strategy() = default;

    virtual void onData(const std::string& timestamp, const TimeSeriesData& data) = 0;
    virtual void onStart() = 0;
    virtual void onEnd() = 0;
};

class MovingAverageStrategy : public Strategy {
private:
    std::deque<double> prices; // Stores recent closing prices
    size_t shortWindow;        // Period for the short moving average
    size_t longWindow;         // Period for the long moving average
    Portfolio& portfolio;      // Reference to the portfolio being managed

    // Helper function to calculate the moving average
    double calculateMovingAverage(const std::deque<double>& data, size_t windowSize) const {
        return std::accumulate(data.end() - windowSize, data.end(), 0.0) / windowSize;
    }

public:
    MovingAverageStrategy(size_t shortW, size_t longW, Portfolio& port)
        : shortWindow(shortW), longWindow(longW), portfolio(port) {
        if (shortWindow <= 0 || longWindow <= 0 || shortWindow > longWindow) {
            throw std::invalid_argument("Invalid window sizes for moving averages");
        }
    }

    void onData(const std::string& timestamp, const TimeSeriesData& data) override {
        prices.push_back(data.close);

        // Ensure the deque only keeps the required number of prices
        if (prices.size() > longWindow) {
            prices.pop_front();
        }

        // Perform calculations only when we have enough data
        if (prices.size() >= longWindow) {
            double shortMA = calculateMovingAverage(prices, shortWindow);
            double longMA = calculateMovingAverage(prices, longWindow);

            std::cout << timestamp << ": Short MA = " << shortMA << ", Long MA = " << longMA << std::endl;

            // Generate buy or sell signals based on moving averages
            if (shortMA > longMA) {
                executeBuySignal(timestamp, data.close);
            }
            else if (shortMA < longMA) {
                executeSellSignal(timestamp, data.close);
            }
        }
    }

    void onStart() override {
        std::cout << "Starting backtest with Moving Average Strategy..." << std::endl;
    }

    void onEnd() override {
        std::cout << "Backtest complete." << std::endl;
        portfolio.printPortfolio();
    }

private:
    void executeBuySignal(const std::string& timestamp, double price) {
        const int quantity = 10; // Number of shares to trade
        if (portfolio.getCash() >= price * quantity) {
            portfolio.buy("SPY", quantity, price);
            std::cout << timestamp << ": Buy signal executed." << std::endl;
        }
        else {
            std::cout << timestamp << ": Buy signal skipped due to insufficient cash." << std::endl;
        }
    }

    void executeSellSignal(const std::string& timestamp, double price) {
        const int quantity = 10; // Number of shares to trade
        if (portfolio.getPosition("SPY") >= quantity) {
            portfolio.sell("SPY", quantity, price);
            std::cout << timestamp << ": Sell signal executed." << std::endl;
        }
        else {
            std::cout << timestamp << ": Sell signal skipped due to insufficient shares." << std::endl;
        }
    }
};
