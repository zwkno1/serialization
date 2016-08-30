
// for base item
// char or uchar is treated as interger"
template<class T>
bool serialize_impl(rapidjson::Value & val, const T & in, rapidjson::Document & doc)
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
bool serialize_impl(rapidjson::Value & val, const std::list<T> & in, rapidjson::Document & doc)
{
    rapidjson::Value v(rapidjson::kArrayType);
    for(auto &i : in)
    {
        rapidjson::Value v1;
        if(!serialize_impl(v1, i, doc))
            return false;
        v.PushBack(std::move(v1), doc.GetAllocator());
    }

    val = std::move(v);

    return true;
}

//vector
template<class T>
bool serialize_impl(rapidjson::Value & val, const std::vector<T> & in, rapidjson::Document & doc)
{
    rapidjson::Value v(rapidjson::kArrayType);
    for(auto &i : in)
    {
        rapidjson::Value v1;
        if(!serialize_impl(v1, i, doc))
            return false;
        v.PushBack(std::move(v1), doc.GetAllocator());
    }

    val = std::move(v);

    return true;
}

//array
template<class T, size_t N>
bool serialize_impl(rapidjson::Value & val, const T (&in)[N], rapidjson::Document & doc)
{
    rapidjson::Value v(rapidjson::kArrayType);
    for(auto &i : in)
    {
        rapidjson::Value v1;
        if(!serialize_impl(v1, i, doc))
            return false;
        v.PushBack(std::move(v1), doc.GetAllocator());
    }

    val = std::move(v);

    return true;
}

//pair
template<typename Key, typename Value>
bool serialize_impl(rapidjson::Value & val, const std::pair<Key, Value> & in, rapidjson::Document & doc)
{
    rapidjson::Value v(rapidjson::kObjectType);

    rapidjson::Value v1;
    if(!serialize_impl(v1, in.first, doc))
        return false;
    v.AddMember("first", std::move(v1), doc.GetAllocator());

    rapidjson::Value v2;
    if(!serialize_impl(v2, in.second, doc))
        return false;
    v.AddMember("second", std::move(v2), doc.GetAllocator());

    val = std::move(v);

    return true;
}

//map
template<typename Key, typename Value>
bool serialize_impl(rapidjson::Value & val, const std::map<Key, Value> & in, rapidjson::Document & doc)
{
    rapidjson::Value v(rapidjson::kArrayType);
    for(auto &i : in)
    {
        rapidjson::Value v1;
        if(!serialize_impl(v1, i, doc))
            return false;
        v.PushBack(std::move(v1), doc.GetAllocator());
    }
    val = std::move(v);
    return true;
}

/* ----------------------------------- unserailize ------------------------------------ */

// for base item
template<class T>
bool unserialize_impl(const rapidjson::Value & val, T & out)
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
bool unserialize_impl(const rapidjson::Value & val, std::list<T> & out)
{
    if(!val.IsArray())
        return false;
    T tmp;
    for(auto i = val.Begin(); i != val.End(); ++i)
    {
        if(!unserialize_impl(*i, tmp))
            return false;
        out.push_back(std::move(tmp));
    }
    return true;
}

//vector
template<class T>
bool unserialize_impl(const rapidjson::Value & val, std::vector<T> & out)
{
    if(!val.IsArray())
        return false;
    T tmp;
    for(auto i = val.Begin(); i != val.End(); ++i)
    {
        if(!unserialize_impl(*i, tmp))
            return false;
        out.push_back(std::move(tmp));
    }
    return true;
}

//array
template<class T, size_t N>
bool unserialize_impl(const rapidjson::Value & val, T (&out)[N])
{
    if(!val.IsArray())
        return false;
    size_t count = 0;
    for(auto i = val.Begin(); i != val.End(); ++i)
    {
        if(count == N)
            return false;
        if(!unserialize_impl(*i, out[count]))
            return false;
        ++count;
    }
    if(count != N)
        return false;
    return true;
}

//pair
template<typename Key, typename Value>
bool unserialize_impl(const rapidjson::Value & val, std::pair<Key, Value> & out)
{
    if(!val.IsObject())
        return false;

    if(!unserialize_impl(val["first"], out.first))
        return false;

    if(!unserialize_impl(val["second"], out.second))
        return false;

    return true;
}

//map
template<typename Key, typename Value>
bool unserialize_impl(const rapidjson::Value & val, std::map<Key, Value> & out)
{
    if(!val.IsArray())
        return false;

    std::pair<Key, Value> tmp;
    for(auto i = val.Begin(); i != val.End(); ++i)
    {
        if(!unserialize_impl(*i, tmp))
            return false;
        out.insert(std::move(tmp));
    }
    return true;
}
