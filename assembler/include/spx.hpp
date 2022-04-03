#ifndef spx_hpp
#define spx_hpp

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <fstream>
#include <utility>

namespace spx
{

    using String = std::string;
    using View = std::string_view;
    using File = std::ifstream;
    using Position = std::pair<size_t,size_t>;

    template <class T>
    struct Object
    {
        using Opt = std::optional<T>;
        using Ptr = std::shared_ptr<T>;

        template <class... A>
        static inline Ptr make_ptr(A... args)
        {
            return std::make_shared<T>(args...);
        }
    };
}

#endif