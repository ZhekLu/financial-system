#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton {
public:
  static T *GetInstance() {
      if (!obj)
          obj = new T();
      return obj;
  }
protected:
  static inline T* obj = nullptr;
  Singleton() = default;
private:
  ~Singleton();
  Singleton(const Singleton &) = delete;
  Singleton(Singleton &other) = delete;
  void operator=(const Singleton &) = delete;
};

#endif // SINGLETON_H

