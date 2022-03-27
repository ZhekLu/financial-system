#ifndef IUSER_H
#define IUSER_H

#include "ISystemObject.h"

#include <QString>
#include <string>
#include <vector>

class IUser : public ISystemObject {
public:
  IUser(size_t id, std::string full_name)
      : ISystemObject(id), name(std::move(full_name)) {}
  virtual ~IUser() = default;

  // methods
  std::string get_name() const { return name; }
  size_t get_id() const { return id; }

protected:
  std::string name;
};

#endif // IUSER_H
