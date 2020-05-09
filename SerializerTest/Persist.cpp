#include "Persist.h"
#include <vector>
#include <memory>

Archive& Archive::operator<<( int val )
{
	write( &val, sizeof( int ) );
	return *this;
}

Archive& Archive::operator>>( int& val )
{
	read( &val, sizeof( int ) );
	return *this;
}

Archive& Archive::operator<<( const std::string& str )
{
	int length = str.length();
	*this << length;
	write( str.c_str(), sizeof( char ) * length );
	return *this;
}

Archive& Archive::operator>>( std::string& str )
{
	int length = -1;
	*this >> length;

	std::vector<char> mem( length + 1 );

	char* pChars = &mem[0];
	read( pChars, sizeof( char ) * length );

	mem[length] = NULL;
	str = pChars;

	return *this;
}

void ArchiveFile::write( const void* buffer, size_t length )
{
	stream->write( (const char*)buffer, length );

	if ( !*stream )
		throw "ArchiveFile::write Error";
}

void ArchiveFile::read( void* buffer, size_t length )
{
	stream->read( (char*)buffer, length );

	if ( !*stream )
		throw "ArchiveFile::read Error";
}

void Persistent::Store( Archive& stream ) const
{
	std::string className = typeid(*this).name();

	className = className.substr( className.find( ' ' ) + 1 );

	stream << className;

	int ver = Version();

	stream << ver;

	stream.SetDirection( true );

	const_cast<Persistent *>(this)->Serialize( stream );
}

Persistent* Persistent::Load( Archive& stream )
{
	std::string className;
	stream >> className;

	IClonable* clone = Clonables::Instance().Create( className.c_str() );
	if ( clone == nullptr )
		throw "Persistent::load : Error creating object";

	std::auto_ptr<IClonable> delitor( clone );

	Persistent * obj = dynamic_cast<Persistent*>(clone);
	if ( obj == nullptr ) 
		throw "Persistent::load : Error creating object";

	int ver = -1;

	stream >> ver;

	if ( ver != obj->Version() )
		throw "Persistent::load : unmatched version number";

	stream.SetDirection( false );

	obj->Serialize( stream );

	delitor.release();

	return obj;
}