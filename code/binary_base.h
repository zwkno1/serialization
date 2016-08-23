
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

//pair
template<typename Key, typename Value>
bool serialize(Buffer & buf, const std::pair<Key, Value> & in)
{
    if(!serialize(buf, in.first))
        return false;
    if(!serialize(buf, in.second))
        return false;
    return true;
}

//map
template<typename Key, typename Value>
bool serialize(Buffer & buf, const std::map<Key, Value> & in)
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
        out.push_back(std::move(tmp));
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

    out.reserve(size);
    T tmp;
    for(uint32_t i = 0; i != size; ++i)
    {
        if(!unserialize(buf, tmp))
            return false;
        out.push_back(std::move(tmp));
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

//pair
template<typename Key, typename Value>
bool unserialize(const Buffer & buf, std::pair<Key, Value> & out)
{
    if(!unserialize(buf, out.first))
        return false;
    if(!unserialize(buf, out.second))
        return false;
    return true;
}

//map
template<typename Key, typename Value>
bool unserialize(const Buffer & buf, std::map<Key, Value> & out)
{
    uint32_t size;
    if(!buf.get(size))
        return false;

    std::pair<Key, Value> tmp;
    for(uint32_t i = 0; i != size; ++i)
    {
        if(!unserialize(buf, tmp))
            return false;
        out.insert(std::move(tmp));
    }
    return true;
}
