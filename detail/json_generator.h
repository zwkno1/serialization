#pragma once

#include "detail/generator.h"

namespace serialization
{

namespace detail
{

class json_generator : public generator
{
public:
    void gen_code(const serialization::detail::tree & t, const std::string & name) override;
};

} //namespace detail

} //namespace serialization
