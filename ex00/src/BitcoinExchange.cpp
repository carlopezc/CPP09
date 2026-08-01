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

  // Verificar que todas las posiciones que no son guiones sean dígitos
  for (int i = 0; i < 10; i++) {
    if (i == 4 || i == 7)
      continue;
    if (date[i] < '0' || date[i] > '9')
      return false;
  }

  // Extraer año, mes y día
  int year, month, day;
  std::istringstream y_ss(date.substr(0, 4));
  std::istringstream m_ss(date.substr(5, 2));
  std::istringstream d_ss(date.substr(8, 2));

  if (!(y_ss >> year) || !(m_ss >> month) || !(d_ss >> day))
    return false;

  // Validar rangos básicos de año y mes
  if (year < 2008 || month < 1 || month > 12 || day < 1)
    return false;

  // Determinar los días máximos permitidos para el mes
  int maxDays = 31;
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    maxDays = 30;
  } else if (month == 2) {
    // Regla de año bisiesto (leap year)
    bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    maxDays = isLeap ? 29 : 28;
  }

  if (day > maxDays)
    return false;

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
      double val;
      std::istringstream iss(value);
      if (!(iss >> val)) {
        std::cerr << "Error: bad input => " << line << std::endl;
        continue;
      }
      if (val < 0) {
        std::cerr << "Error: not a positive number." << std::endl;
        continue;
      }
      if (val > 1000) {
        std::cerr << "Error: too large a number." << std::endl;
        continue;
      }
      float rate = getRate(date);
      std::cout << date << " => " << value << " = " << rate * val << std::endl;
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
