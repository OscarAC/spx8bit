#ifndef spx_hpp
#define spx_hpp

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <fstream>
#include <utility>
#include <variant>

namespace spx
{

    using String = std::string;
    using View = std::string_view;
    using File = std::ifstream;
    using Position = std::pair<size_t, size_t>;
    using Encoding = uint16_t;

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

    struct Register : public Object<Register>
    {
        View key;
        View id;

        Register(View k, View i) : key(k), id(i) {}
    };

    struct OpCode : public Object<OpCode>
    {

        enum Type
        {
            O,
            S,
            SO
        };

        View key;
        char width;
        Encoding encoding;
        View format;
        Type type;

        OpCode(View k, char w, Encoding e, View f, Type t)
            : key(k), width(w), encoding(e), format(f), type(t) {}
    };

    struct Address : public Object<Address>
    {

        Address(uint64_t loc) : location(loc) {}

        uint64_t location;
    };

    struct Immediate : public Object<Immediate>
    {
        Immediate(uint64_t val) : value(val) {}
        uint64_t value;
    };

    struct Operand : public Object<Operand>
    {
        using Value = std::variant<Register, Address, Immediate>;

        enum Type
        {
            Reg,
            Addr,
            Imm
        };

        template <typename T>
        Operand(Type t, T val) : type(t), value(val) {}

        Address as_address()
        {
            return std::get<Address>(value);
        }

        Register as_register()
        {
            return std::get<Register>(value);
        }

        Immediate as_immediate()
        {
            return std::get<Immediate>(value);
        }

        template <typename T>
        void set_value(T val)
        {
            value = val;
        }

        Type type;
        Value value;
    };

    struct Instruction : public Object<Instruction>
    {
        uint64_t address;
        uint64_t code;
    };

}

#endif