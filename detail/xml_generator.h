#pragma once

#include "detail/generator.h"

namespace serialization
{

namespace detail
{

class xml_generator : public generator
{
public:
    virtual void gen_code(const serialization::detail::tree & t, const std::string & name) override;
};

} //namespace detail

} //namespace serialization
