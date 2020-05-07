#include "Serializer.h"
#include <iostream>

void Serializer::Serialize( const char* name, const ISerializable& obj )
{
	ClassInfo info = obj.GetClassInfo();

	if ( depth > 0 )
	{
		std::cout << "<" << name << ">\n";
	}

	ObjectsIds::iterator iObj = objIds.find( &obj );

	if ( iObj != objIds.end() )
	{
		std::cout << "<Reference class=\"" << info.Name() << "\" version=\"" <<
			info.Version() << "\" id=\"" << iObj->second << "\"/>\n";
	}
	else
	{
		int id = objCount++;

		objIds[&obj] = id;

		std::cout << "<Object class=\"" << info.Name() << "\" version =\"" << info.Version() <<
			"\" id=\"" << id << "\">";

		depth++;
		obj.Serialize( *this );
		depth--;

		std::cout << "</Object>\n";
	}

	if ( depth > 0 )
	{
		std::cout << "</" << name << ">\n";
	}
}

void Serializer::Serialize( const char* name, int n )
{
	if ( depth > 0 )
	{
		std::cout << "<" << name << ">" << n << "</" << name << ">\n";
	}
	else
	{
		std::cout << n;
	}

}