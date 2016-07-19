#pragma once

#include <list>
#include <string>
#include <ostream>
#include <cstring>
#include <exception>

#include <rapidxml.hpp>

#ifdef SERIALIZATION_NO_TRACE
#define SERIALIZATION_TRACE(os, str1, str2)
#else
#define SERIALIZATION_TRACE(os, str1, str2) ::serialization::trace trace_object(os, str1, str2); (void)trace_object;
#endif

#ifdef SERIALIZATION_PARSE_NO_EXCEPTION
extern void serialize_parse_error(const char *, const char *)
#define SERIALIZATION_PARSE_ERROR(what, where)  serialize_parse_error(what, where)
#else
#define SERIALIZATION_PARSE_ERROR(what, where) throw ::serialization::parse_error(what, where);
#endif

namespace serialization
{

class parse_error: public std::exception
{
public:

    parse_error(const char *what, const char * where)
        : what_(what)
        , where_(where)
    {
    }

    virtual const char *what() const throw()
    {
        return what_;
    }

    const char *where() const
    {
        return where_;
    }

private:
    const char * what_;
    const char * where_;
};

class trace
{
public:
    trace(std::ostream & os, const std::string & str1, const std::string & str2)
        : os_(os)
        , str1_(str1)
        , str2_(str2)
    {
        os_ << "/* @generate [ " << str1_ << ":" << str2_ << " ] begin @ */\n\n";
    }

    ~trace()
    {
        os_ << "/* @generate [ " << str1_ << ":" << str2_ << " ] end @ */\n\n";
    }

private:
    std::ostream & os_;
    const std::string & str1_;
    const std::string & str2_;
};

namespace detail
{

class object
{
public:
    object(const std::string & type, const std::string & name, const std::string & desc)
        : type_(type)
        , name_(name)
        , desc_(desc)
    {
    }

    virtual void genDefinition(std::ostream & )
    {
    }

    virtual void genSerialize(std::ostream & )
    {
    }

    virtual void genUnserialize(std::ostream & )
    {
    }

    virtual const char * objectType() const
    {
        return "@object";
    }

    virtual ~object()
    {
    }

protected:
    std::string type_;
    std::string name_;
    std::string desc_;
};

class item : public object
{
public:
    item(const std::string & type, const std::string & name, const std::string & desc)
        : object(type, name, desc)
    {
    }
};

//base type : for int, double, string...
class base : public item
{
public:
    base(const std::string & type, const std::string & name, const std::string & desc)
        : item(type, name, desc)
    {
    }

    virtual const char * objectType() const final
    {
        return "@item";
    }
};

//sequence : sequence of items of the same type 
class sequence : public item
{
public:
    sequence(const std::string& type, const std::string& name, const std::string& desc)
        : item(type, name, desc)
    {
    }

    virtual const char * objectType() const final
    {
        return "@sequence";
    }
};

//structure: set of items 
class structure: public item
{
public:
    structure(const std::string & type, const std::string & name, const std::string & desc)
        : item(type, name, desc)
    {
    }

    virtual const char * objectType() const final
    {
        return "@structure";
    }
};

//entry : object can serialization and unserialization
class entry : public object
{
public:
    entry(const std::string & ns, const std::string & type, const std::string & desc, const std::string & id)
        : object(type , "", desc)
        , namespace_(ns)
        , id_(id)
    {
    }

    virtual const char * objectType() const final
    {
        return "@entry";
    }

    void append(item * i)
    {
        items_.push_back(i);
    }

    virtual void genFuncDef(std::ostream & os)
    {
    }

protected:
    std::string namespace_;
    std::string id_;
    std::list<item *> items_;
};

class root : public object
{
public:
    root(const std::string & type, const std::string & desc)
        : object(type, "", desc)
    {
    }

    virtual const char * objectType() const final
    {
        return "@root";
    }

    void append(entry * e)
    {
        entries_.push_back(e);
    }

protected:
    std::list<entry *> entries_;
};

} //namespace detail

} //namespace serialization

