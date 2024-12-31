#ifndef DATA_MODULE_H
#define DATA_MODULE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <stdexcept>

struct TimeSeriesData {
    double open;
    double high;
    double low;
    double close;
    int volume;

    // Convert to a market data format suitable for portfolio updates
    std::unordered_map<std::string, double> toMarketData() const {
        return {
            {"Open", open},
            {"High", high},
            {"Low", low},
            {"Close", close}
        };
    }
};

class DataModule {
public:
    // Function to load and parse time series data from CSV
    bool loadTimeSeriesCSV(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open CSV file: " << filePath << std::endl;
            return false;
        }

        std::string line;
        int lineNumber = 0;
        while (std::getline(file, line)) {
            lineNumber++;
            if (lineNumber == 1) {
                // Skip header line if present
                continue;
            }

            std::istringstream stream(line);
            std::string timestamp, open, high, low, close, volume;

            try {
                // Parse each column (comma-separated)
                std::getline(stream, timestamp, ',');
                std::getline(stream, open, ',');
                std::getline(stream, high, ',');
                std::getline(stream, low, ',');
                std::getline(stream, close, ',');
                std::getline(stream, volume, ',');

                // Validate and convert
                if (timestamp.empty() || open.empty() || high.empty() || low.empty() || close.empty() || volume.empty()) {
                    std::cerr << "Error: Malformed line (" << lineNumber << ") -> " << line << std::endl;
                    continue;
                }

                TimeSeriesData tsData = {
                    std::stod(open),
                    std::stod(high),
                    std::stod(low),
                    std::stod(close),
                    std::stoi(volume)
                };

                timeSeriesData[timestamp] = tsData;
            }
            catch (const std::exception& ex) {
                std::cerr << "Error parsing line (" << lineNumber << "): " << line << " -> " << ex.what() << std::endl;
            }
        }

        file.close();
        return true;
    }

    // Function to print time series data
    void printTimeSeriesData() const {
        for (const auto& [timestamp, data] : timeSeriesData) {
            std::cout << timestamp << " -> "
                << "Open: " << data.open << ", "
                << "High: " << data.high << ", "
                << "Low: " << data.low << ", "
                << "Close: " << data.close << ", "
                << "Volume: " << data.volume << std::endl;
        }
    }

    // Getter function to retrieve the time series data
    const std::map<std::string, TimeSeriesData>& getTimeSeriesData() const {
        return timeSeriesData;
    }

private:
    std::map<std::string, TimeSeriesData> timeSeriesData; // Maps timestamp to time series data
};

#endif // DATA_MODULE_H
