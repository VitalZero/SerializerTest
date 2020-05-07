#pragma once
#include "ISerializable.h"
#include <map>

using ObjectsIds = std::map<const ISerializable*, int>;

class Serializer
{
public:
	void Serialize( const char* name, const ISerializable& obj );
	void Serialize( const char* name, int n );

private:
	int depth = 0;
	ObjectsIds objIds;
	int objCount;
};