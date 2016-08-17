#include "buffer.h"

#include <string>
#include <stddef.h>

template<>
bool Buffer::get(std::string & out)
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

template<>
void Buffer::append(const std::string & v)
{
	uint32_t size = v.size();
	append(size);
	data_.insert(data_.end(), (uint8_t *)&v[0], (uint8_t *)&v[size]);
}
