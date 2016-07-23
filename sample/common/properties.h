#ifndef COMMON_PROPERTIES_H_
#define COMMON_PEOPERTIES_H_

#include <stdint.h>
#include <istream>
#include <string>
#include <map>

namespace common {

class Properties {
 private :
  //Map that holds the (key,value) pairs
  typedef std::map<std::string, std::string> PropMap;
  PropMap propmap; 

 public:
  typedef PropMap::const_iterator iterator;
  
  iterator begin() const { return propmap.begin(); }
  iterator end() const { return propmap.end(); }
  
  // load the properties from a file
  int LoadProperties(const char* filename, char delimiter, bool verbose, bool multiline = false);
  // load the properties from an in-core buffer
  int LoadProperties(std::istream &ist, char delimiter, bool verbose, bool multiline = false);
  
  std::string GetValue(std::string key, std::string def) const;
  const char* GetValue(std::string key, const char* def) const;
  
  int GetValue(std::string key, int def) const;
  unsigned int GetValue(std::string key, unsigned int def) const;
  
  long GetValue(std::string key, long def) const;
  unsigned long GetValue(std::string key, unsigned long def) const;
  
  long long GetValue(std::string key, long long def) const;
  unsigned long long GetValue(std::string key, unsigned long long def) const;
  
  double GetValue(std::string key, double def) const;
  void SetValue(const std::string key, const std::string value);
  
  void GetList(std::string &outbuf, std::string lineprefix, std::string linesuffix = std::string("\n")) const;
  
  void clear() { propmap.clear(); }
  bool empty() const { return propmap.empty(); }
  size_t size() const { return propmap.size(); }
  
  void CopyWithPrefix(std::string prefix, Properties& props) const;
  void swap(Properties& props) { propmap.swap(props.propmap); }
  
  Properties();
  Properties(const Properties &p);
  ~Properties();
};

}  // namespace common

#endif // COMMON_PROPERTIES_H
