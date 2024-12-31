#include "Portfolio.h"
#include <iomanip>
#include <stdexcept>

// Constructor
Portfolio::Portfolio() : cash(0.0) {}

// Destructor
Portfolio::~Portfolio() {}

// Set initial cash amount
void Portfolio::setCash(double amount) {
    if (amount < 0) throw std::invalid_argument("Cash amount cannot be negative.");
    cash = amount;
}

// Buy shares of a symbol
void Portfolio::buy(const std::string& symbol, int quantity, double price) {
    if (quantity <= 0 || price <= 0) throw std::invalid_argument("Quantity and price must be positive.");
    double cost = quantity * price;
    if (cost > cash) throw std::runtime_error("Insufficient cash to complete purchase.");

    cash -= cost;
    positions[symbol] += quantity;

    // Update average cost basis
    if (avgCostBasis.find(symbol) == avgCostBasis.end()) {
        avgCostBasis[symbol] = price;
    }
    else {
        double totalCost = avgCostBasis[symbol] * (positions[symbol] - quantity) + cost;
        avgCostBasis[symbol] = totalCost / positions[symbol];
    }
}

// Sell shares of a symbol
void Portfolio::sell(const std::string& symbol, int quantity, double price) {
    if (quantity <= 0 || price <= 0) throw std::invalid_argument("Quantity and price must be positive.");
    if (positions.find(symbol) == positions.end() || positions[symbol] < quantity) {
        throw std::runtime_error("Insufficient shares to sell.");
    }

    double revenue = quantity * price;
    cash += revenue;
    positions[symbol] -= quantity;

    // If all shares are sold, remove the symbol from positions and avgCostBasis
    if (positions[symbol] == 0) {
        positions.erase(symbol);
        avgCostBasis.erase(symbol);
    }
}

// Print the current portfolio holdings
void Portfolio::printPortfolio() const {
    std::cout << "\nPortfolio Holdings:" << std::endl;
    std::cout << "-------------------" << std::endl;
    for (const auto& [symbol, quantity] : positions) {
        std::cout << symbol << ": " << quantity << " shares, Avg Cost: $"
            << std::fixed << std::setprecision(2) << avgCostBasis.at(symbol) << std::endl;
    }
    std::cout << "Cash: $" << std::fixed << std::setprecision(2) << cash << std::endl;
}

// Get the total net worth of the portfolio (cash + value of positions)
double Portfolio::getNetWorth() const {
    double netWorth = cash;
    for (const auto& [symbol, quantity] : positions) {
        // Assuming latest prices are known and managed elsewhere
        // Replace 'currentPrice' with actual price from data source
        double currentPrice = 0.0; // Placeholder
        netWorth += quantity * currentPrice;
    }
    return netWorth;
}

// Get the position (number of shares) for a specific symbol
int Portfolio::getPosition(const std::string& symbol) const {
    if (positions.find(symbol) == positions.end()) return 0;
    return positions.at(symbol);
}

void Portfolio::updateNetWorth(const std::unordered_map<std::string, double>& currentPrices) {
    // Calculate total value starting with cash
    double totalValue = cash;

    // Iterate through all positions to calculate their market value
    for (const auto& [symbol, quantity] : positions) {
        auto it = currentPrices.find(symbol);
        if (it != currentPrices.end()) {
            double currentPrice = it->second; // Get the latest price for the symbol
            totalValue += quantity * currentPrice;
        }
        else {
            std::cerr << "Warning: No current price available for symbol: " << symbol << std::endl;
        }
    }

    // Add the current net worth to the equity curve
    equityCurve.push_back(totalValue);

    // Calculate returns if there is more than one data point in the equity curve
    if (equityCurve.size() > 1) {
        double lastReturn = (equityCurve.back() - equityCurve[equityCurve.size() - 2]) / equityCurve[equityCurve.size() - 2];
        returns.push_back(lastReturn);
    }

    // Log the updated net worth (optional)
    std::cout << "Updated Net Worth: $" << std::fixed << std::setprecision(2) << totalValue << std::endl;
}


// Get the equity curve (historical net worth values)
const std::vector<double>& Portfolio::getEquityCurve() const {
    return equityCurve;
}

// Get the returns over time
const std::vector<double>& Portfolio::getReturns() const {
    return returns;
}

// Get the average cost basis for a specific symbol
double Portfolio::getAvgCostBasis(const std::string& symbol) const {
    if (avgCostBasis.find(symbol) == avgCostBasis.end()) {
        throw std::runtime_error("No cost basis found for the symbol.");
    }
    return avgCostBasis.at(symbol);
}
