#ifndef ISYSTEMOBJECT_H
#define ISYSTEMOBJECT_H

#include <QString>

class ISystemObject {
public:
  ISystemObject(size_t id) : id(id) {}
  virtual QString get_values_query() = 0;

  size_t id;
};

#endif // ISYSTEMOBJECT_H
