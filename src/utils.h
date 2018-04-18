#pragma once

#include <exception>
#include <memory>
#include <sstream>
#include <string>

class GameException: public std::exception {
private:
  std::string mError;
public:
  GameException(std::string error, std::string sdlError) {
    std::ostringstream s;
    s << error << ": " << sdlError << "\n";
    mError = std::move(s.str());
  }
  const char* what() { return mError.c_str(); }
};

