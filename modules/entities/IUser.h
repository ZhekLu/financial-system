#ifndef IUSER_H
#define IUSER_H

#include "ISystemObject.h"
#include "modules/modes.h"

#include <QString>
#include <string>
#include <vector>

class IUser : public ISystemObject {
public:
  IUser(size_t id, LoginMode mode) : ISystemObject(id), mode(mode) {}
  virtual ~IUser() = default;

  // methods
  LoginMode get_role() const { return mode; }

protected:
  LoginMode mode;
};

#endif // IUSER_H
