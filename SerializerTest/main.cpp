//#include "Node.h"
//
//int main()
//{
//	Node n3( 300 );
//	Node n2( 200 );
//	Node n1( 100 );
//	Node n0( 0 );
//
//	n0.SetNext( &n1 );
//	n1.SetNext( &n2 );
//	n2.SetNext( &n3 );
//	n3.SetNext( &n0 );
//
//	Serializer s;
//	s.Serialize( "n0", n0 );
//
//	return 0;
//}

//Example on persist.h
#include "Persist.h"
#include <fstream>

class Event : public Persistent {
private:
	int _id;

public:
	Event() : _id( 0 ) {}
	virtual ~Event() {}
	int getId() const { return _id; }

protected:
	virtual void serialize( Archive& stream )
	{
		if ( stream.IsStoring() )
			stream << _id;
		else
			stream >> _id;
	}

	PERSISTENT_DECL(Event)
};

PERSISTENT_IMPL( Event )

void serialize_example()
{
	std::auto_ptr<Event> event( new Event() );

	std::fstream file("try.txt",
		std::ios::out| std::ios::in | std::ios::binary | std::ios::trunc );

	ArchiveFile stream( &file );

	if ( !file )
		throw "Unable to open file for writing";

	event->Store( stream );

	file.seekg( 0, std::ios::beg );

	Event* newEvent = dynamic_cast<Event*>(Persistent::Load( stream ));

	event.reset( newEvent );

	file.close();
}

int main()
{
	serialize_example();

	return 0;
}