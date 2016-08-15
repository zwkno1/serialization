#pragma once

#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <endian.h>

class Buffer
{
public:
	Buffer(size_t size = 0)
		: data_(size)
		  , index_(0)
	{
	}

	bool get(std::string & out ) const;

	void append(const std::string & out );

	template<typename T>
	bool get(T & out) const
	{
		if(!can_read(sizeof(out)))
		  return false;
		out = *reinterpret_cast<const T *>(&data_[index_]);
		index_ += sizeof(out);
	
		switch(sizeof(T))
		{
			case 2:
			{
				out = be16toh(out);
			}
			break;
			case 4: 
			{
				out = be32toh(out);
			}
			break;
			case 8:
			{
				out = be64toh(out);
			}
			break;
			default:
			break;
		}
		return true;
	}

	template<typename T>
	void append(const T & v)
	{
		switch(sizeof(T))
		{
			case 2:
			{
				T val = htobe16(v);
				data_.insert(data_.end(), (uint8_t *)&val, (uint8_t *)&val + sizeof(val));
			}
			break;
			case 4: 
			{
				T val = htobe32(v);
				data_.insert(data_.end(), (uint8_t *)&val, (uint8_t *)&val + sizeof(val));
			}
			break;
			case 8:
			{
				T val = htobe64(v);
				data_.insert(data_.end(), (uint8_t *)&val, (uint8_t *)&val + sizeof(val));
			}
			break;
			default:
				data_.insert(data_.end(), (uint8_t *)&v, (uint8_t *)&v + sizeof(v));
			break;
		}
	}
	
	bool can_read(size_t size) const
	{
		return index_ + size <= data_.size();
	}

	size_t size() const
	{
		return data_.size();
	}

	uint8_t * data()
	{
		return data_.data();
	}

private:
	std::vector<uint8_t> data_;
	mutable size_t index_;
};

