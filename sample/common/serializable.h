#ifndef COMMON_SERIALIZABLE_H_
#define COMMON_SERIALIZABLE_H_

namespace common {

class Serializable {
 public:
  // return byte size
  virtual size_t ByteArraySize() const = 0;

  // load from byte array size
  virtual bool LoadFromByteArray(const char* byte, size_t len) const = 0;

  // save to byte array
  virtual bool SaveToByteArray(char** byte, size_t* len) const = 0;
};

}  // namespace common

#endif  // COMMON_SERIALIZABLE_H_
