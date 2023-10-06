#include "GenerateAst.hpp"
#include <cctype>
#include <cstdio>
#include <fstream>
#include <ios>
#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string s, const char c) {
  std::vector<std::string> ret;
  std::stringstream ss(s);
  std::string part;
  while (!ss.eof()) {
    std::getline(ss, part, c);
    ret.push_back(part);
  }
  return ret;
}

bool isSpace(const char c) {
  return c == ' ' || c == '\r' || c == '\t' || c == '\n';
}

std::string trim(const std::string &s) {
  std::string ret;

  int i, j;
  for (i = 0; i <= s.size() - 1 && isSpace(s[i]); ++i)
    ;
  for (j = s.size() - 1; j >= 0 && isSpace(s[j]); --j)
    ;

  return ret = s.substr(i, j - i + 1);
}

void GenerateAst::defineAst(const std::string &outputDir,
                            const std::string &baseName,
                            const std::vector<std::string> &types) {
  std::string path = outputDir + "/" + baseName + ".hpp";
  std::ofstream ofs(path, std::ofstream::out | std::ofstream::binary);

  ofs << "#pragma once\n"
      << "#include \"Lox.hpp\"\n"
      << "#include <bits/stdc++.h>\n\n";

  ofs << "template <typename R> class Visitor;\n\n";

  ofs << "template <typename R>\n"
      << "class " + baseName + " {\n"
      << "public:\n"
      << "  virtual R accept(const Visitor<R> &visitor) const = 0;\n"
      << "  virtual ~" + baseName + "() = default;\n"
      << "};\n\n";

  for (const std::string &type : types) {
    std::string className = trim(split(type, ':')[0]);
    std::string fields = trim(split(type, ':')[1]);
    if (!ofs.is_open()) {
      std::cerr << "error1!!!\n";
      exit(123);
    }
    defineType(ofs, baseName, className, fields);
  }

  defineVisitor(ofs, baseName, types);

  ofs.close();
}

std::string stringTolower(const std::string &s) {
  std::string ret;

  for (auto c : s) {
    if (c >= 'A' && c <= 'Z') {
      ret += c - 'A' + 'a';
    } else {
      ret += c;
    }
  }

  return ret;
}

void GenerateAst::defineVisitor(std::ofstream &ofs, const std::string &baseName,
                                const std::vector<std::string> types) {
  ofs << "template <typename R>\n"
      << "class Visitor {\n"
      << "public:\n";

  for (auto type : types) {
    const std::string typeName = trim(split(type, ':')[0]);
    ofs << "  virtual R visit" + typeName + baseName + "(const " + typeName +
               "<R> &" + stringTolower(baseName) + ") const = 0;\n";
  }

  ofs << "};\n\n";
}

void GenerateAst::defineType(std::ofstream &ofs, const std::string baseName,
                             const std::string &className,
                             std::string &fieldList) {
  if (!ofs.is_open()) {
    std::cerr << "error!!!\n";
    exit(123);
  }

  ofs << "template <typename R>\n";
  ofs << "class " + className + ": public " + baseName + "<R> {\n"
      << "public:\n";

  // Constructor
  ofs << "  " + className + "(";

  // Store parameters in fields
  auto fields = split(fieldList, ',');
  std::vector<std::string> types, names;
  for (auto field : fields) {
    std::string name = trim(split(trim(field), ' ')[1]);
    std::string type = trim(split(trim(field), ' ')[0]);
    if (type == baseName) {
      type = "std::shared_ptr<" + type + "<R>>" ;
    }
    types.push_back("const " + type);
    names.push_back(name);
  }
  for (int i = 0; i < fields.size(); ++i) {
    std::string type = types[i];
    std::string name = names[i];
    ofs << type << " " << name;
    if (i < fields.size() - 1) {
      ofs << ", ";
    }
  }

  ofs << ") : ";

  for (auto name : names) {
    ofs << name << "(" << name << "), ";
  }

  ofs << baseName + "<R>() "
      << "{}\n\n";

  // Visitor pattern
  ofs << "  R accept(const Visitor<R> &visitor) const override {\n"
      << "    return visitor.visit" + className + baseName + "(*this);\n"
      << "}\n\n";

  // fields
  for (int i = 0; i < fields.size(); ++i) {
    ofs << "  " + types[i] + " " + names[i] + ";\n";
  }

  ofs << "};\n\n";
}
