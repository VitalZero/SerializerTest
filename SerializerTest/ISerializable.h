#pragma once
// Metadata
// ClassInfo interface
// Contains class name and version number for every serialized class
class ClassInfo
{
public:
	ClassInfo( const char* name, int version )
		: name( name ), version( version )
	{}
	int Version() const { return version; }
	const char* Name() const { return name; }

private:
	const char* name;
	int version;
};

class Serializer;
// Interface
class ISerializable
{
public:
	virtual ~ISerializable() {}
	virtual void Serialize( Serializer& serializer ) const = 0;
	virtual ClassInfo GetClassInfo() const = 0;
};