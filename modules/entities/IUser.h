#ifndef IUSER_H
#define IUSER_H

#include <QString>
#include <string>

class IUser {
public:
  IUser(size_t id, std::string full_name)
      : id(id), name(std::move(full_name)) {}
  virtual ~IUser() = default;

  // methods
  virtual QString get_info() = 0;
  std::string get_name() { return name; }
  size_t get_id() { return id; }

protected:
  size_t id;
  std::string name;
};

#endif // IUSER_H
