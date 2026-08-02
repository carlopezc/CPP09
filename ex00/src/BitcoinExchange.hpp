/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:00:11 by carlopez          #+#    #+#             */
/*   Updated: 2026/08/01 18:00:13 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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