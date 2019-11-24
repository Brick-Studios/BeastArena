#ifndef FILE_SIZE_MISMATCH_EXCEPTION_HPP
#define FILE_SIZE_MISMATCH_EXCEPTION_HPP

#include <iostream>
#include <exception>

class SizeMismatchException : public std::exception {
private:
  std::string m_msg;
public:
  SizeMismatchException() : m_msg(std::string("Level is not 16:9 and cannot be transformed to game size")){}

  virtual const char* what() const throw() {
    return m_msg.c_str();
  }
};

#endif // FILE_SIZE_MISMATCH_EXCEPTION_HPP