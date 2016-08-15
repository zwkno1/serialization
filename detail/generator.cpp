#include "detail/generator.h"

namespace serialization
{

namespace detail
{

void generator::gen_struct_def(const serialization::detail::tree & t, std::ostream & os)
{
	if(!t.desc.empty())
	  os << "// " << t.desc << "\n";
	if(!t.type.empty())
	  os << "namespace " << t.type << "\n{\n";

	for(serialization::detail::entry * e : t.entries)
	{
		os << "// " << e->desc << "\n";
		os << "struct " << e->type << "\n{\n";
		if(!e->id.empty())
		  os << "enum { ID = " << e->id << " };\n\n";

		for(serialization::detail::item * i : e->items)
		{
			os << "// " << i->desc << "\n";

			switch(i->item_type)
			{
				case serialization::detail::Vector:
					os << "std::vector< " << i->type << " >    " << i->name << ";\n\n";
				case serialization::detail::List:
					os << "std::list< " << i->type << " >    " << i->name << ";\n\n";
					break;
				case serialization::detail::Array:
					os << i->type << "    " << i->name << "[ " << i->size << " ];\n\n";
					break;
				default:
					os << i->type << "    " << i->name << ";\n\n";
					break;
			}
		}
		os << "};\n\n";
	}

	if(!t.type.empty())
	  os << "\n} // namespace " << t.type << "\n";
	os << "\n\n";
}

} //namespace detail

} //namespace serialization
