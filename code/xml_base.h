
// for base item
// char or uchar is treated as interger"
template<class T>
bool serialize(rapidxml::xml_node<> * node, const T & in, rapidxml::xml_document<> & doc)
{
	std::stringstream ss;
	ss << in;
	node->value(doc.allocate_string(ss.str().c_str()));
	return true;
}

//list
template<class T>
bool serialize(rapidxml::xml_node<> * node, const std::list<T> & in, rapidxml::xml_document<> & doc)
{
	for(auto & i : in)
	{
		auto node1 = doc.allocate_node(rapidxml::node_element, "item");
		node->append_node(node1);
		if(!serialize(node1, i, doc))
		  return false;
	}
	return true;
}

//vector
template<class T>
bool serialize(rapidxml::xml_node<> * node, const std::vector<T> & in, rapidxml::xml_document<> & doc)
{
	for(auto & i : in)
	{
		auto node1 = doc.allocate_node(rapidxml::node_element, "item");
		node->append_node(node1);
		if(!serialize(node1, i, doc))
		  return false;
	}
	return true;
}

//array
template<class T, size_t N>
bool serialize(rapidxml::xml_node<> * node, const T (&in)[N], rapidxml::xml_document<> & doc)
{
	for(auto & i : in)
	{
		auto node1 = doc.allocate_node(rapidxml::node_element, "item");
		node->append_node(node1);
		if(!serialize(node1, i, doc))
		  return false;
	}
	return true;
}

/* -----------------------------unserialize------------------------------- */

//base
template<class T>
bool unserialize(const rapidxml::xml_node<> * node, T & out)
{
	if(!node)
	  return false;
	std::stringstream ss;
	ss << node->value();
	ss >> out;
	return ss.eof();
}

//list
template<class T>
bool unserialize(const rapidxml::xml_node<> * node, std::list<T> & out)
{
	if(!node)
	  return false;
	for(auto i = node->first_node(); i; i = i->next_sibling())
	{
		if(std::strcmp(i->name(), "item"))
		  return false;
		T tmp;
		if(!unserialize(i, tmp))
		  return false;
		out.push_back(std::move(tmp));
	}
	return true;
}

//vector
template<class T>
bool unserialize(const rapidxml::xml_node<> * node, std::vector<T> & out)
{
	if(!node)
	  return false;
	for(auto i = node->first_node(); i; i = i->next_sibling())
	{
		if(std::strcmp(i->name(), "item"))
		  return false;
		T tmp;
		if(!unserialize(i, tmp))
		  return false;
		out.push_back(std::move(tmp));
	}
	return true;
}

//array
template<class T, size_t N>
bool unserialize(const rapidxml::xml_node<> * node, T (&out)[N])
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
		if(!unserialize(i, out[count]))
		  return false;
		++count;
	}
	if(count != N)
	  return false;
	return true;
}

