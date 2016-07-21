#pragma once

#include <list>
#include <string>
#include <ostream>
#include <cstring>
#include <exception>

#include <rapidxml/rapidxml.hpp>

#ifdef SERIALIZATION_NO_TRACE
#define SERIALIZATION_TRACE(os, str1, str2)
#else
#define SERIALIZATION_TRACE(os, str1, str2) ::serialization::trace trace_object(os, str1, str2); (void)trace_object;
#endif

#ifdef SERIALIZATION_PARSE_NO_EXCEPTION
extern void serialize_parse_error(const char *, const char *)
#define SERIALIZATION_PARSE_ERROR(what, where)  serialize_parse_error(what, where)
#else
#define SERIALIZATION_PARSE_ERROR(what, where) throw ::serialization::detail::parse_error(what, where);
#endif

namespace serialization
{

namespace detail
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

enum ItemType
{
    Base,
    Sequence,
    Structure,
};

struct item
{
public:
    item(ItemType it, const std::string & t, const std::string & n, const std::string & d)
        : item_type(it)
        , type(t)
        , name(n)
        , desc(d)
    {
    }

    ItemType item_type;

    std::string type;
    std::string name;
    std::string desc;
};

struct entry
{
    entry(const std::string & t, const std::string & d, const std::string & i)
        : type(t)
        , desc(d)
        , id(i)
    {
    }

    std::string type;
    std::string desc;
    std::string id;

    std::list<item *> items;
};

class pool
{
public:
    pool() = default;

    item* make_item(ItemType item_type, const std::string & type, const std::string & name, const std::string & desc)
    {
        item * new_item = new item(item_type, type, name, desc);
        item_list_.push_back(new_item);
        return new_item;
    }

    entry* make_entry(const std::string & type, const std::string & desc, const std::string & id)
    {
        entry * new_entry = new entry(type, desc, id);
        entry_list_.push_back(new_entry);
        return new_entry;
    }

    ~pool()
    {
        for(auto i : item_list_)
            delete i;

        for(auto i : entry_list_)
            delete i;
    }

private:
    pool( const pool& ) = delete;
    pool& operator=( const pool& ) = delete;

    std::list<item *> item_list_;
    std::list<entry *> entry_list_;
};

struct tree : public pool
{
    //namespace
    std::string type;
    std::string desc;
    std::list<entry *> entries;
};

} //namespace detail

} //namespace serialization

