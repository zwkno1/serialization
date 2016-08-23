#pragma once

#include "detail/objects.h"

namespace serialization
{

namespace detail
{

class generator
{
public:
	virtual void gen_code(const serialization::detail::tree & t, const std::string & name) = 0;

protected:
    virtual void gen_def(const serialization::detail::tree & t, std::ostream & os);

};

} //namespace detail

} //namespace serialization
