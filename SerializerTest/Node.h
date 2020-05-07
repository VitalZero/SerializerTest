#pragma once
#include "Serializer.h"

class Node : public ISerializable
{
public:
	Node( int value )
		: pNext( nullptr ), value( value )
	{}
	virtual ~Node() {}
	void SetNext( Node* pNext )
	{
		this->pNext = pNext;
	}
	ClassInfo GetClassInfo() const
	{
		return ClassInfo( "Node", 1 );
	}
	void Serialize( Serializer& s ) const
	{
		s.Serialize( "Value", value );
		s.Serialize( "Next", *pNext );
	}

private:
	Node* pNext;
	int value;
};