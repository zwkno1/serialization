#include "buffer.h"

#include <string>

template<>
bool Buffer::get(std::string & out) const
{
	uint32_t size = 0;
	if(!get(size))
	  return false;
	if(!can_read(size))
	  return false;
	out = std::string(reinterpret_cast<const char *>(&data_[index_]), size);
	index_ += size;
	return true;
}
