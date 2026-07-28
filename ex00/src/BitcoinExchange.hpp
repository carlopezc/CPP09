
#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange {
private:
  std::map<std::string, float> _db;

  float getRate(const std::string &date) const;
  bool isValidDate(const std::string &date) const;

public:
  BitcoinExchange();
  ~BitcoinExchange();
  BitcoinExchange(const BitcoinExchange &other);
  BitcoinExchange &operator=(const BitcoinExchange &other);

  BitcoinExchange(const std::string &dbFile);

  void processInput(const std::string &filename);
};

#endif