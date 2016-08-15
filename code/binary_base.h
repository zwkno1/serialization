
// for base item
template<typename T>
bool serialize(Buffer & buf, const T & in)
{
	buf.append(in);
	return true;
}

//list 
template<typename T>
bool serialize(Buffer & buf, const std::list<T> & in)
{
	uint32_t size = in.size();
	buf.append(size);
	for(auto &i : in)
	{
		if(!serialize(buf, i))
		  return false;
	}
	return true;
}

//vector
template<typename T>
bool serialize(Buffer & buf, const std::vector<T> & in)
{
	uint32_t size = in.size();
	buf.append(size);
	for(auto &i : in)
	{
		if(!serialize(buf, i))
		  return false;
	}
	return true;
}

//array
template<typename T, size_t N>
bool serialize(Buffer & buf, const T(&in)[N])
{
	uint32_t size = N;
	buf.append(size);
	for(auto &i : in)
	{
		if(!serialize(buf, i))
		  return false;
	}
	return true;
}

/* -------------------------------------- unserialize --------------------------------- */

// for base item
template<typename T>
bool unserialize(const Buffer & buf, T & out)
{
	return buf.get(out);
}

//list 
template<typename T>
bool unserialize(const Buffer & buf, std::list<T> & out)
{
	uint32_t size;
	if(!buf.get(size))
	  return false;

	T tmp;
	for(uint32_t i = 0; i != size; ++i)
	{
		if(!unserialize(buf, tmp))
		  return false;
	}
	return true;
}

//vector
template<typename T>
bool unserialize(const Buffer & buf, std::vector<T> & out)
{
	uint32_t size;
	if(!buf.get(size))
	  return false;

	T tmp;
	for(uint32_t i = 0; i != size; ++i)
	{
		if(!unserialize(buf, tmp))
		  return false;
	}
	return true;
}

//array
template<typename T, size_t N>
bool unserialize(const Buffer & buf, T (&out)[N])
{
	uint32_t size;
	if(!buf.get(size))
	  return false;

	if(size != N)
	  return false;

	for(uint32_t i = 0; i != size; ++i)
	{
		if(!unserialize(buf, out[i]))
		  return false;
	}
	return true;
}
