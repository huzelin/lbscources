#include <iostream>
#include <fstream>
#include <cstdlib>

#include "common/properties.h"

namespace common {

Properties::Properties()
    : propmap() {
}

Properties::Properties(const Properties &p)
  : propmap(p.propmap) {
}

Properties::~Properties() {
}

int Properties::LoadProperties(const char* filename, char delimiter, bool verbose, bool multiline ) {
  std::ifstream input(filename);
  std::string line;

  if (!input.is_open()) {
    std::cerr <<  "Properties::LoadProperties() Could not open the file:" << filename << std::endl;
    return -1;
  }
  LoadProperties(input, delimiter, verbose, multiline);
  input.close();
  return 0;
}

inline void RemoveLTSpaces(std::string& str){
  char const* const delims = " \t\r\n";

  // trim leading whitespace
  std::string::size_type const notwhite = str.find_first_not_of(delims);
  str.erase(0,notwhite);
  if (notwhite != std::string::npos) { // Do not assume that string::npos+1 == 0
    // trim trailing whitespace
    str.erase(str.find_last_not_of(delims)+1);
  }
}

int Properties::LoadProperties(std::istream &ist, char delimiter, bool verbose, bool multiline) {
  std::string line;
  
  while (ist) {
    getline(ist, line);                                  // read one line at a time
    if (line.find('#') == 0)
      continue;                                          // ignore comments
    std::string::size_type pos = line.find(delimiter);   // find the delimiter
    if (pos == line.npos)
      continue;                                          // ignore if no delimiter is found
    std::string key = line.substr(0,pos);                // get the key
    RemoveLTSpaces(key);
    std::string value = line.substr(pos + 1);            // get the value
    RemoveLTSpaces(value);
    
    if (multiline)
      propmap[key] += value;                  // allow properties to be spread across multiple lines
    else
      propmap[key] = value;
    
    if (verbose)
      std::cout << "Loading key " << key  << " with value " << propmap[key] << std::endl;
  }
  return 0;
}

void Properties::SetValue(const std::string key, const std::string value) {
  propmap[key] = value;
  return;
}

std::string Properties::GetValue(std::string key, std::string def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : i->second);
}

const char* Properties::GetValue(std::string key, const char* def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : i->second.c_str());
}

int Properties::GetValue(std::string key, int def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : atoi(i->second.c_str()));
}

unsigned int Properties::GetValue(std::string key, unsigned int def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : atoi(i->second.c_str()));
}

long Properties::GetValue(std::string key, long def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : atol(i->second.c_str()));
}

unsigned long Properties::GetValue(std::string key, unsigned long def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : atol(i->second.c_str()));
}

long long Properties::GetValue(std::string key, long long def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : atoll(i->second.c_str()));
}

unsigned long long Properties::GetValue(std::string key, unsigned long long def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : atoll(i->second.c_str()));
}

double Properties::GetValue(std::string key, double def) const {
  PropMap::const_iterator const i = propmap.find(key);
  return (i == propmap.end() ? def : atof(i->second.c_str()));
}

void Properties::GetList(std::string &outbuf, std::string lineprefix, std::string linesuffix) const {
  PropMap::const_iterator iter;
  for (iter = propmap.begin(); iter != propmap.end(); iter++) {
    if (iter->first.size() > 0) {
      outbuf += lineprefix;
      outbuf += iter->first;
      outbuf += '=';
      outbuf += iter->second;
      outbuf += linesuffix;
    }
  }
  return;
}

void Properties::CopyWithPrefix(std::string prefix, Properties& props) const {
  PropMap::const_iterator iter;
  for (iter = propmap.begin(); iter != propmap.end(); iter++) {
    const std::string& key = iter->first;
    if (key.compare(0, prefix.length(), prefix) == 0) {
      props.propmap[key] = iter->second;
    }
  }
}

}  // namespace common
