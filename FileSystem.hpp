#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>

class File;
class Directory;
class IObject;
class FileSystem;

constexpr auto SHOW_EXTENSIONS_ON_DUMP = true;
constexpr auto ROOT_DIR_NAME = "root";

class FileSystem {
    public:
    FileSystem();

    void createDirectory(const std::string& name);
    void createFile(const std::string& name, const std::string& extension = "file");
    void setCurrentDirectory(Directory& dir);
    void setCurrentDirectory(const std::string& name_dir);
    Directory& getCurrentDirectory();

    private:
    std::vector<IObject*> m_objects;
    Directory* m_current_dir;
};

class IObject {
    public:
    virtual ~IObject(){}

    const std::string& getName() const;
    void setName(const std::string& name);

    Directory& getParent() const;
    void setParent(Directory& dir);

    bool isImmutable() const;
    std::string getFullPath() const;

    protected:
    std::string m_name;
    bool m_immutable = false;
    Directory* m_parent;
    friend class FileSystem;
};

class File : public IObject {
    public:
    const std::string& getExtension() const;
    void setExtension(const std::string& extension);
    std::string getFullName() const;
    std::vector<std::string>& getContents();

    private:
    std::string m_extension = "file";
    std::vector<std::string> m_contents;
    friend class FileSystem;
};

class Directory : public IObject {
    public:
    IObject& findChildByName(const std::string& name);
    bool hasChildWithName(const std::string& name);
    void dump();

    private:
    std::vector<IObject*> m_children;
    friend class FileSystem;
};

void parseInput(FileSystem& file_system, const std::string& input, Directory& root);