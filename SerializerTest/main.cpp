#include "Node.h"

int main()
{
	Node n3( 300 );
	Node n2( 200 );
	Node n1( 100 );
	Node n0( 0 );

	n0.SetNext( &n1 );
	n1.SetNext( &n2 );
	n2.SetNext( &n3 );
	n3.SetNext( &n0 );

	Serializer s;
	s.Serialize( "n0", n0 );

	return 0;
}