#include "FileSystem.hpp"

const std::string &IObject::getName() const
{
    return m_name;
}

void IObject::setName(const std::string &name)
{
    m_name = name;
}

const std::string &File::getExtension() const
{
    return m_extension;
}

void File::setExtension(const std::string &extension)
{
    m_extension = extension;
}

std::string File::getFullName() const
{
    return m_name + "." + m_extension;
}

IObject &Directory::findChildByName(const std::string &name)
{
    for (auto &child : m_children)
    {
        auto* file_cast = dynamic_cast<File*>(child);
        auto* dir_cast = dynamic_cast<Directory*>(child);

        if(dir_cast)
        {
            if (dir_cast->getName() == name)
            {
                return *child;
            }
        } else
        if(file_cast)
        {
            if(file_cast->getFullName() == name)
            {
                return *child;
            }
        }
    }
    std::cout << "Couldn't find child with name [ " << name.c_str() << " ] at directory with name [ " << m_name << " ]\n";
    return *(new File());
}

FileSystem::FileSystem()
{
    Directory *root = new Directory();
    root->m_immutable = true;
    root->setName(ROOT_DIR_NAME);
    setCurrentDirectory(*root);

    m_objects.push_back(root);
}

void FileSystem::createDirectory(const std::string &name)
{
    Directory *directory = new Directory;
    directory->setParent(*m_current_dir);
    directory->setName(name);
    getCurrentDirectory().m_children.push_back(directory);

    m_objects.push_back(directory);
}

void FileSystem::createFile(const std::string &name, const std::string &extension)
{
    File *file = new File;
    file->setExtension(extension);
    file->setName(name);
    getCurrentDirectory().m_children.push_back(file);
    m_objects.push_back(file);
}

void FileSystem::setCurrentDirectory(Directory &dir)
{
    m_current_dir = &dir;
}

Directory &FileSystem::getCurrentDirectory()
{
    return *m_current_dir;
}

Directory &IObject::getParent() const
{
    return *m_parent;
}

void IObject::setParent(Directory &dir)
{
    m_parent = &dir;
}

void Directory::dump()
{
    for (auto &child : m_children)
    {
        auto *file_cast = dynamic_cast<File *>(child);
        auto *dir_cast = dynamic_cast<Directory *>(child);

        if (file_cast != nullptr)
        {
            if (SHOW_EXTENSIONS_ON_DUMP)
            {
                std::cout << file_cast->getFullName() << "\n";
            }
            else
            {
                std::cout << file_cast->getName() << "\n";
            }
        }

        if (dir_cast != nullptr)
        {
            std::cout << dir_cast->getName() << "\n";
        }
    }
}

void FileSystem::setCurrentDirectory(const std::string &dir_name)
{
    setCurrentDirectory(*dynamic_cast<Directory *>(&getCurrentDirectory().findChildByName(dir_name)));
}

template <typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


std::string getStringNQ(const std::string& input, int starting_index = 0)
{
    std::string res;
    int c;
    for(int i = starting_index; i < input.size(); i++)
    {
        if(input[i] != '\'')
        {
            res += input[i];
        }

        if(i-1<0 && input[i] == '\'' && input[i-1] == '\\')
        {
            res += input[i];
        }
    }
    return res;
}

std::vector<std::string>& File::getContents()
{
    return m_contents;
}

void parseInput(FileSystem &file_system, const std::string &input, Directory &root)
{
    std::vector<std::string> args = split(input, ' ');

    if (args[0] == "exit")
    {
        exit(0);
    }
    else

        if (args[0] == "ls")
    {
        file_system.getCurrentDirectory().dump();
    }
    else

        if (args[0] == "touch")
    {
        auto file_ext_del = split(args[1], '.');
        file_system.createFile(file_ext_del[0], file_ext_del[1]);
    }
    else

        if (args[0] == "mkdir")
    {
        file_system.createDirectory(args[1]);
    }
    else

        if (args[0] == "cd")
    {
        std::string t = args[1];

        if (args[1] == "..")
        {
            if (file_system.getCurrentDirectory().isImmutable() && file_system.getCurrentDirectory().getName() == ROOT_DIR_NAME)
            {
            }
            else
            {
                file_system.setCurrentDirectory(file_system.getCurrentDirectory().getParent());
            }
        }
        else
        {
            file_system.setCurrentDirectory(args[1]);
        }

        if (args[1] == "$r")
        {
            file_system.setCurrentDirectory(root);
        }
    }
    else

        if (args[0] == "cls" || args[0] == "clear")
    {
        system("cls"); // i know
    }
    else

        if (args[0] == "start")
    {
        auto* child = &file_system.getCurrentDirectory().findChildByName(args[1]);
        auto* file_cast = dynamic_cast<File*>(child);

        if(file_cast)
        {
            for(auto& line : file_cast->getContents())
            {
                parseInput(file_system, line, root);
            }
        }

    }
    else
    if(args[0] == "extstart")
        {
            std::ifstream input(args[1]);
            if(!input.is_open())
            {
                std::cout<<"Couldn't open external file [ "<<args[1]<<" ]\n";
            }

            for(std::string line; getline(input, line);)
            {
                parseInput(file_system, line, root);
            }
        }

    else if (args[0] == "echo")
    {
        for (int i = 1; i < args.size(); i++)
        {

            if (args[i][0] == '$' && args[i][1] == 'r')
            {
                std::cout << file_system.getCurrentDirectory().getName() << " ";
            }
            else
            {
                std::cout << args[i] << " ";
            }
        }
        std::cout << "\n";
    }
    else

    if (args[0] == "fsio")
    {
        auto* child = &file_system.getCurrentDirectory().findChildByName(args[1]);
        auto* file_cast = dynamic_cast<File*>(child);

        if (file_cast == nullptr)
        {
            std::cout << "Couldn't open file [ " << args[1] << " ]\n";
        }

        if (args[2] == "read")
        {
            for(auto& line : file_cast->getContents())
            {
                std::cout<<line<<"\n";
            }

        } else
        if(args[2] == "write")
        {
            int index_begin = 0;
            int index_end = input.size();
            int index = 0;

            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < args[i].size(); j++)
                {
                    index += 1;
                }
                index += 1;
            }

            std::string final = getStringNQ(input, index);
            file_cast->getContents().push_back(final);
        }
    }

    else {
        std::cout << "Unknown command [ " << args[0] << " ]\n";
    }
}

bool IObject::isImmutable() const
{
    return m_immutable;
}

bool Directory::hasChildWithName(const std::string &name)
{
    for (auto &child : m_children)
    {
        auto *file_cast = dynamic_cast<File*>(child);
        auto *dir_cast = dynamic_cast<Directory*>(child);

        if(dir_cast)
        {
            if (child->getName() == name)
            {
                return true;
            }
        } else
        if(file_cast)
        {
            if(file_cast->getFullName() == name)
            {
                return true;
            }
        }
    }
    return false;
}