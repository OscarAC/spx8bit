#ifndef spx_file_hpp
#define spx_file_hpp

#include <spx.hpp>
#include <fstream>
#include <vector>

namespace spx
{
    class TextFile : public Object<TextFile>
    {
    public:
        TextFile() = default;

        explicit TextFile(View path)
            : path(path), file(path.data(), std::ifstream::in) {}

        // no copy
        TextFile(const TextFile &o) = delete;
        auto operator=(const TextFile &o) = delete;

        TextFile(TextFile &&o)
        {
            std::swap(path, o.path);
            std::swap(file, o.file);
        }

        auto operator=(TextFile &&o)
        {
            std::swap(path, o.path);
            std::swap(file, o.file);
        }

        inline bool check()
        {
            return file.is_open() && !file.fail() && !file.bad();
        }

        virtual ~TextFile()
        {
            if (file.is_open())
            {
                file.close();
            }
        }

    protected:
        String path;
        File file;
    };

    class MemoryFile : public TextFile
    {
    public:
        MemoryFile(View path) : TextFile(path)
        {
            load();
        }

        // call check before
        View data()
        {
            return buffer;
        }

    private:
        void load();

    private:
        String buffer;
    };

    class FileManager : public Object<FileManager>
    {
    public:
        using Files = std::vector<TextFile::Ptr>;

    private:
        Files files;
    };
}

#endif
