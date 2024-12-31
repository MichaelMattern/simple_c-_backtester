#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

// ---------------------  Portfolio  -------------------------------------------
class Portfolio {
private:
    double cash; // Available cash balance
    std::unordered_map<std::string, int> positions; // Symbol -> Quantity
    std::unordered_map<std::string, double> avgCostBasis; // Symbol -> Average cost basis per share
    std::vector<double> equityCurve; // Tracks portfolio net worth over time
    std::vector<double> returns; // Stores returns over time

public:
    Portfolio();
    ~Portfolio();

    // Set initial cash amount
    void setCash(double amount);

    // Buy shares of a symbol
    void buy(const std::string& symbol, int quantity, double price);

    // Sell shares of a symbol
    void sell(const std::string& symbol, int quantity, double price);

    // Print the current portfolio holdings
    void printPortfolio() const;

    // Get the total net worth of the portfolio (cash + positions value)
    double getNetWorth() const;

    // Get the current cash balance
    double getCash() const { return cash; }

    // Get the position (number of shares) for a specific symbol
    int getPosition(const std::string& symbol) const;

    // Update net worth based on the latest price data
    void updateNetWorth(const std::unordered_map<std::string, double>& currentPrices);

    // Get the equity curve (historical net worth values)
    const std::vector<double>& getEquityCurve() const;

    // Get the returns over time
    const std::vector<double>& getReturns() const;

    // Get the average cost basis for a specific symbol
    double getAvgCostBasis(const std::string& symbol) const;
};
