//As in code project by Gilad Rozban
//https://www.codeproject.com/Tips/495191/Serialization-implementation-in-Cplusplus
//1. You should have an abstract class for cloneable objects.
//2. You should have a factory class that creates your cloneable objects given a class name
//	( hint :use Singleton implementation ).
//3. You don't have to use templates at all !
//4. Automatic registration of a class into the factory isn't trivial since C++ doesn't have a static constructor.
//	This can be done by a macro which adds a static member into your class.
//5. You should have an abstract Archive class since you don't know where you store your objects
//	(file, pipes, memory ..).
//6. Use auto_ptr to make sure you delete your objects if the serialization has failed or an exception 
//	has been thrown.
#pragma once
#include <string>
#include <iostream>
#include "Dynamics.h"

//We must implement a base class for streaming( with storing and loading support ).An archive can be a file 
//	or a pipe or anything which stores our object.
class Archive
{
public:
	Archive( bool isStoring = true )
		: isStoring( isStoring )
	{}
	virtual ~Archive() {}

	virtual void Write( const void* buffer, size_t length ) {}
	virtual void Read( void* buffer, size_t length ) {}

	Archive& operator<<( const std::string& str );
	Archive& operator>>( std::string& str );

	Archive& operator<<( int val );
	Archive& operator>>( int& val );

	bool IsStoring() const { return isStoring; }
	void SetDirection( bool isStoring ) { this->isStoring = isStoring; }

private:
	bool isStoring;
};

class ArchiveFile : public Archive
{
public:
	ArchiveFile( std::iostream* stream )
		: stream( stream )
	{}
	virtual ~ArchiveFile() {}
	virtual void Write( const void* buffer, size_t length );
	virtual void Read( void* buffer, size_t length );

private:
	std::iostream* stream;
};

class Persistent : public IClonable
{
public:
	virtual ~Persistent() {}
	static Persistent* Load( Archive& stream );
	void Store( Archive& stream ) const;

protected:
	virtual void Serialize( Archive& stream ) {}
	virtual int Version() const { return 0; }
};

//We want an automatic implementation of the createObj method and an automatic registration
//of our class into the cloneables collection.This can be done with the following macro declerations :
//PERSISTENT_DECL macro implements the createObj method of the 'IClonable' class for us.It also adds 
//	'AddClonable' static member to our class : this makes our persistent class register itself to the 
//	cloneable collection.This should be added to our.h class definition( see examples below ).
//
//PERSISTENT_IMPL simply initializes this static member.This should be added to our.cpp class implementation.
#define PERSISTENT_DECL(className) \
public: \
virtual IClonable* CreateObj() const \
{ \
    return new className(); \
} \
private: \
static AddClonable _addClonable;

#define PERSISTENT_IMPL(className) \
    AddClonable className::_addClonable(#className, new className());