#include "lightgbm-serving/config_parser.hh"

#include <fstream>
#include <sstream>

#include "rapidjson/document.h"

namespace lgbm_serving {

ConfigParser::ConfigParser() {}
ConfigParser::~ConfigParser() {
  clear();
}

void ConfigParser::clear() {
  configs.clear();
}

void ConfigParser::parseModelConfig(const std::string path) {
  std::ifstream ifs(path);

  if (!ifs.good())
    throw std::runtime_error("Cannot open file or directory: " + path);

  std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  rapidjson::Document document;
  document.Parse(str.c_str());

  if (!document.HasMember("config") || !document["config"].IsArray())
    throw std::runtime_error("There is no config");

  for (auto& value : document["config"].GetArray()) {
    if (!value.HasMember("name") || !value.HasMember("path") || !value.HasMember("nClasses"))
      throw std::runtime_error("Cannot get attribute `nClasses`, `name` or `path`");

    if (!value["name"].IsString() || !value["path"].IsString() || !value["nClasses"].IsUint())
      throw std::runtime_error("Name or path is not a string, or nClasses is not a UInt");

    ModelConfig config{value["name"].GetString(), value["path"].GetString(), value["nClasses"].GetUint()};
    configs.push_back(config);
  }
}

size_t ConfigParser::getLength() const {
  return configs.size();
}

ModelConfig& ConfigParser::get(size_t index) {
  return configs[index];
}

std::vector<ModelConfig>::const_iterator ConfigParser::begin() const {
  return configs.begin();
}

std::vector<ModelConfig>::const_iterator ConfigParser::end() const {
  return configs.end();
}

}  // namespace lgbm_serving