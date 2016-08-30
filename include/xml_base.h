
// for base item
// char or uchar is treated as interger"
template<typename T>
bool serialize_impl(rapidxml::xml_node<> * node, const T & in, rapidxml::xml_document<> & doc)
{
    std::stringstream ss;
    ss << in;
    node->value(doc.allocate_string(ss.str().c_str()));
    return true;
}

//list
template<typename T>
bool serialize_impl(rapidxml::xml_node<> * node, const std::list<T> & in, rapidxml::xml_document<> & doc)
{
    for(auto & i : in)
    {
        auto node1 = doc.allocate_node(rapidxml::node_element, "item");
        node->append_node(node1);
        if(!serialize_impl(node1, i, doc))
            return false;
    }
    return true;
}

//vector
template<typename T>
bool serialize_impl(rapidxml::xml_node<> * node, const std::vector<T> & in, rapidxml::xml_document<> & doc)
{
    for(auto & i : in)
    {
        auto node1 = doc.allocate_node(rapidxml::node_element, "item");
        node->append_node(node1);
        if(!serialize_impl(node1, i, doc))
            return false;
    }
    return true;
}

//array
template<typename T, size_t N>
bool serialize_impl(rapidxml::xml_node<> * node, const T (&in)[N], rapidxml::xml_document<> & doc)
{
    for(auto & i : in)
    {
        auto node1 = doc.allocate_node(rapidxml::node_element, "item");
        node->append_node(node1);
        if(!serialize_impl(node1, i, doc))
            return false;
    }
    return true;
}

//pair
template<typename Key, typename Value>
bool serialize_impl(rapidxml::xml_node<> * node, const std::pair<Key, Value> & in, rapidxml::xml_document<> & doc)
{
    auto node1 = doc.allocate_node(rapidxml::node_element, "first");
    node->append_node(node1);
    if(!serialize_impl(node1, in.first, doc))
        return false;

    auto node2 = doc.allocate_node(rapidxml::node_element, "second");
    node->append_node(node2);
    if(!serialize_impl(node2, in.second, doc))
        return false;
    return true;
}

//map
template<typename Key, typename Value>
bool serialize_impl(rapidxml::xml_node<> * node, const std::map<Key, Value> & in, rapidxml::xml_document<> & doc)
{
    for(auto & i : in)
    {
        auto node1 = doc.allocate_node(rapidxml::node_element, "item");
        node->append_node(node1);
        if(!serialize_impl(node1, i, doc))
            return false;
    }
    return true;
}


/* -----------------------------unserialize_impl------------------------------- */

//base
template<typename T>
bool unserialize_impl(const rapidxml::xml_node<> * node, T & out)
{
    if(!node)
        return false;
    std::stringstream ss;
    ss << node->value();
    ss >> out;
    return ss.eof();
}

//list
template<typename T>
bool unserialize_impl(const rapidxml::xml_node<> * node, std::list<T> & out)
{
    if(!node)
        return false;
    for(auto i = node->first_node(); i; i = i->next_sibling())
    {
        if(std::strcmp(i->name(), "item"))
            return false;
        T tmp;
        if(!unserialize_impl(i, tmp))
            return false;
        out.push_back(std::move(tmp));
    }
    return true;
}

//vector
template<typename T>
bool unserialize_impl(const rapidxml::xml_node<> * node, std::vector<T> & out)
{
    if(!node)
        return false;
    for(auto i = node->first_node(); i; i = i->next_sibling())
    {
        if(std::strcmp(i->name(), "item"))
            return false;
        T tmp;
        if(!unserialize_impl(i, tmp))
            return false;
        out.push_back(std::move(tmp));
    }
    return true;
}

//array
template<typename T, size_t N>
bool unserialize_impl(const rapidxml::xml_node<> * node, T (&out)[N])
{
    if(!node)
        return false;
    int count = 0;
    for(auto i = node->first_node(); i; i = i->next_sibling())
    {
        if(count == N)
            return false;
        if(std::strcmp(i->name(), "item"))
            return false;
        if(!unserialize_impl(i, out[count]))
            return false;
        ++count;
    }
    if(count != N)
        return false;
    return true;
}

//pair
template<typename Key, typename Value>
bool unserialize_impl(const rapidxml::xml_node<> * node, std::pair<Key, Value> & out)
{
    if(!node)
        return false;
    auto i = node->first_node();
    if(!i)
        return false;
    if(std::strcmp(i->name(), "first"))
        return false;
    if(!unserialize_impl(i, out.first))
        return false;

    i = i->next_sibling();
    if(!i)
        return false;
    if(std::strcmp(i->name(), "second"))
        return false;
    if(!unserialize_impl(i, out.second))
        return false;

    return true;
}

//map
template<typename Key, typename Value>
bool unserialize_impl(const rapidxml::xml_node<> * node, std::map<Key, Value> & out)
{
    if(!node)
        return false;
    std::pair<Key, Value> tmp;
    for(auto i = node->first_node(); i; i = i->next_sibling())
    {
        if(std::strcmp(i->name(), "item"))
            return false;
        if(!unserialize_impl(i, tmp))
            return false;
        out.insert(std::move(tmp));
    }
    return true;
}
