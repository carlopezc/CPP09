#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::~BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : _db(other._db) {}
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
  if (this != &other)
    _db = other._db;
  return *this;
}

BitcoinExchange::BitcoinExchange(const std::string &dbFile) {
  std::ifstream file(dbFile);
  std::string line;

  if (file.is_open()) {
    // Skipping first line with the header
    std::getline(file, line);
    while (std::getline(file, line)) {
      size_t pos = line.find(',');
      if (pos == std::string::npos)
        continue;
      std::string date = line.substr(0, pos);
      std::string value = line.substr(pos + 1);
      double rate;
      std::istringstream iss(value);
      if (!(iss >> rate))
        continue;
      _db[date] = rate;
    }
  } else {
    throw std::runtime_error("Error opening file");
  }
}

bool BitcoinExchange::isValidDate(const std::string &date) const {
  if (date.size() != 10)
    return false;
  if (date[4] != '-' || date[7] != '-')
    return false;
  return true;
}

bool BitcoinExchange::isValidValue(const std::string &value) const {
  double val;
  std::istringstream iss(value);
  if (!(iss >> val))
    return false;
  if (val < 0) {
    std::cerr << "Error: not a positive number." << std::endl;
    return false;
  }
  if (val > 1000) {
    std::cerr << "Error: too large a number." << std::endl;
    return false;
  }
  return true;
}

void BitcoinExchange::processInput(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  if (file.is_open()) {
    std::getline(file, line);
    while (std::getline(file, line)) {
      size_t pos = line.find('|');
      if (pos == std::string::npos) {
        std::cerr << "Error: bad input => " << line << std::endl;
        continue;
      }

      std::string date = line.substr(0, pos - 1);
      std::string value = line.substr(pos + 2);

      if (!isValidDate(date)) {
        std::cerr << "Error: bad input => " << line << std::endl;
        continue;
      }
      if (!isValidValue(value))
        continue;

      double val;
      std::istringstream iss(value);
      iss >> val;
      float rate = getRate(date);
      std::cout << date << " => " << value << " = " << rate * val
                << std::endl;
    }
  } else {
    throw std::runtime_error("Error opening file");
  }
}

float BitcoinExchange::getRate(const std::string &date) const {
  std::map<std::string, float>::const_iterator it = _db.find(date);
  if (it != _db.end())
    return it->second;
  it = _db.lower_bound(date);
  if (it == _db.begin())
    return 0;
  --it;
  return it->second;
}
