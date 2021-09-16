#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#define ENUM_CLASS_NAME fyaml_type
#define ENUM_NAME type

#define ENUM_BODY \
	ENUM_MESSAGE(type, MAP, QUEUE, SCALA, ERROR)

#define ENUM_VALUES \
	xx(fyaml_type::type::MAP)  \
	xx(fyaml_type::type::QUEUE) \
	xx(fyaml_type::type::SCALA) \
	xx(fyaml_type::type::ERROR) \
//

#include "../../include/EnumReflection.h"
namespace tt { 

class fyaml_data{ 
public: 
	using ptr = std::shared_ptr<fyaml_data>;
	using struct_t = std::map<std::string, fyaml_data::ptr>;

	fyaml_data();
	virtual ~fyaml_data();
	
	void            add_son(const std::string& name, fyaml_data::ptr& addr) { m_son_nodes.insert({ name, addr}); }
	fyaml_data::ptr get_son(const std::string& name) { return m_son_nodes[name]; }

	fyaml_data::ptr operator[](const std::string& name) { return get_son(name); }

	
	int isMap() { return m_type == fyaml_type::type::MAP; } // split is :\n && only opne fyaml in lower level 
	int isQueue() { return m_type == fyaml_type::type::QUEUE; } // split is : [args] || more fyaml int same level
	int isScala() { return m_type == fyaml_type::type::SCALA; } // no:
	int isError() { return m_type == fyaml_type::type::ERROR; } // split is :

	fyaml_type::type type() const { return m_type; }
	const std::string& name() const { return m_name; }
	unsigned int level() const { return m_level;}
	unsigned int location_begin() const { return m_location[0]; }
	unsigned int location_line_num() const { return m_location[1]; }

	void
	load(const std::vector<int>& levels, const std::vector<std::string>& strs, 
		 int beg, int nLine);



	void show_data();

protected:
	fyaml_data(const std::string& name, int location, int level);

	void load_line(const std::string& line_str, int location, int level);
	void load_scala(const std::string& name, int location, int level);
public:
	static const std::string& s_name_follows;

protected:
	std::string m_name;
	unsigned int m_level;
	fyaml_type::type m_type;
	
	std::vector<std::string> m_sons;
	unsigned int m_location[2];

	struct_t m_son_nodes;

}; // fyaml

class fyaml_struct {
public:
	using ptr = std::shared_ptr<fyaml_struct>;
	fyaml_struct() { }
	virtual ~fyaml_struct() { }


protected:
	fyaml_data m_data;


}; // fyaml_struct

class fyaml_level_map {
public:
	fyaml_level_map(const int max_level) { m_mapper.resize(max_level + 1); }
	fyaml_data::ptr& find(int level, const std::string& name); 
	void add(int level, fyaml_data::ptr& fd);

protected:
	std::vector<std::map<std::string, fyaml_data::ptr> > m_mapper;
}; // class fyaml_level_map

} // namespcae tt 


