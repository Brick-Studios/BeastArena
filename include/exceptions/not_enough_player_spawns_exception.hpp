#ifndef FILE_NOT_ENOUGH_PLAYER_SPAWNS_EXCEPTION_HPP
#define FILE_NOT_ENOUGH_PLAYER_SPAWNS_EXCEPTION_HPP

#include <iostream>
#include <exception>

class NotEnoughPlayerSpawnsException : public std::exception {
private:
  std::string m_msg;
public:
  NotEnoughPlayerSpawnsException() : m_msg(std::string("The level does not contain 4 player spawns")){}

  virtual const char* what() const throw() {
    return m_msg.c_str();
  }
};

#endif // FILE_NOT_ENOUGH_PLAYER_SPAWNS_EXCEPTION_HPP