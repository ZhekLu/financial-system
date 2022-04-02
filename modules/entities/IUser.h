#ifndef IUSER_H
#define IUSER_H

#include "ISystemObject.h"
#include "modules/modes.h"

#include <QString>
#include <string>
#include <vector>

class IUser : public ISystemObject {
public:
  IUser(size_t id, LoginMode mode, std::string full_name)
      : ISystemObject(id), name(std::move(full_name)), mode(mode) {}
  virtual ~IUser() = default;

  // methods
  std::string get_name() const { return name; }
  LoginMode get_role() const { return mode; }

protected:
  std::string name;
  LoginMode mode;
};

#endif // IUSER_H
