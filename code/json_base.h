
// for base item
// char or uchar is treated as interger"
template<class T>
bool serialize(rapidjson::Value & val, const T & in, rapidjson::Document & doc)
{
	rapidjson::Value v(rapidjson::kStringType);
	std::stringstream ss;
	ss << in;
	v.SetString(ss.str().c_str(), doc.GetAllocator());
	val = std::move(v);
	return true;
}

//list 
template<class T>
bool serialize(rapidjson::Value & val, const std::list<T> & in, rapidjson::Document & doc)
{
	rapidjson::Value v(rapidjson::kArrayType);
	for(auto &i : in)
	{
		rapidjson::Value v1;
		if(!serialize(v1, i, doc))
		  return false;
		v.PushBack(std::move(v1), doc.GetAllocator());
	}

	val = std::move(v);

	return true;
}

//vector
template<class T>
bool serialize(rapidjson::Value & val, const std::vector<T> & in, rapidjson::Document & doc)
{
	rapidjson::Value v(rapidjson::kArrayType);
	for(auto &i : in)
	{
		rapidjson::Value v1;
		if(!serialize(v1, i, doc))
		  return false;
		v.PushBack(std::move(v1), doc.GetAllocator());
	}

	val = std::move(v);

	return true;
}

//array
template<class T, size_t N>
bool serialize(rapidjson::Value & val, const T (&in)[N], rapidjson::Document & doc)
{
	rapidjson::Value v(rapidjson::kArrayType);
	for(auto &i : in)
	{
		rapidjson::Value v1;
		if(!serialize(v1, i, doc))
		  return false;
		v.PushBack(std::move(v1), doc.GetAllocator());
	}

	val = std::move(v);

	return true;
}

/* ----------------------------------- unserailize ------------------------------------ */

// for base item
template<class T>
bool unserialize(const rapidjson::Value & val, T & out)
{
	if(!val.IsString())
	  return false;
	std::stringstream ss;
	ss << val.GetString();
	ss >> out;
	if(!ss.eof())
	  return false;
	return true;
}

//list 
template<class T>
bool unserialize(const rapidjson::Value & val, std::list<T> & out)
{
	if(!val.IsArray())
	  return false;
	T tmp;
	for(auto i = val.Begin(); i != val.End(); ++i)
	{
		if(!unserialize(*i, tmp))
		  return false;
		out.push_back(std::move(tmp));
	}
	return true;
}

//vector
template<class T>
bool unserialize(const rapidjson::Value & val, std::vector<T> & out)
{
	if(!val.IsArray())
	  return false;
	T tmp;
	for(auto i = val.Begin(); i != val.End(); ++i)
	{
		if(!unserialize(*i, tmp))
		  return false;
		out.push_back(std::move(tmp));
	}
	return true;
}

//array
template<class T, size_t N>
bool unserialize(const rapidjson::Value & val, T (&out)[N])
{
	if(!val.IsArray())
	  return false;
	size_t count = 0;
	for(auto i = val.Begin(); i != val.End(); ++i)
	{
		if(count == N)
		  return false;
		if(!unserialize(*i, out[count]))
		  return false;
		++count;
	}
	if(count != N)
	  return false;
	return true;
}

