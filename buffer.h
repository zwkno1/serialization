
#include <vector>
#include <stdint.h>

class Buffer
{
	public:
		Buffer(size_t size = 0)
			: data_(size)
			  , index_(0)
	{
	}

		template<typename T>
			void append(const T & v)
			{
				data_.insert(data_.end(), (uint8_t *)&v, (uint8_t *)&v + sizeof(v));
			}

		template<typename T>
			bool get(T & out) const
			{
				if(!can_read(sizeof(out)))
				  return false;
				out = *reinterpret_cast<const T *>(&data_[index_]);
				index_ += sizeof(out);
				return true;
			}

		bool can_read(size_t size) const
		{
			return index_ + size >= data_.size();
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
