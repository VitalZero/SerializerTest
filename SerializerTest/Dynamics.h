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
#include <map>
#include <string>

//Every cloneable class should be derived from 'Clonable' and implement createObj method.createObj 
//	should return a new object of your class.
//Example;
//	std::string className = "MyComplexClass";
//	Clonable* instance = Clonables::Instance().create( className );
//	MyComplexClass* pCmplx = dynamic_cast<MyComplexClass*>(instance);
class IClonable
{
public:
	virtual ~IClonable() {}
	virtual IClonable* CreateObj() const = 0;
};

//Singleton collection clonable class
using ClonableCollection = std::map<std::string, const IClonable*>;

class Clonables 
{
private:
	ClonableCollection clonables;

private:
	Clonables() {}
	Clonables( const Clonables& ) = delete;                 // Prevent copy-construction
	Clonables& operator=( const Clonables& ) = delete;      //  Prevent assignment
	~Clonables()
	{
		for ( ClonableCollection::const_iterator it = clonables.begin(); it != clonables.end(); it++ ) 
		{
			const IClonable* clone = it->second;
			delete clone;
		}

		clonables.clear();
	}

public:
	static Clonables& Instance()
	{
		static Clonables instance;   // Guaranteed to be destroyed.                              
		return instance;    // Instantiated on first use.
	}
	void AddClonable( const char* className, const IClonable* clone )
	{
		std::string name = className;
		ClonableCollection::const_iterator it = clonables.find( name );

		if ( it == clonables.end() ) 
		{
			clonables[name] = clone;
		}
	}
	IClonable* Create( const char *className )
	{
		std::string name = className;
		ClonableCollection::const_iterator it = clonables.find( name );

		if ( it == clonables.end() ) 
			return nullptr;

		const IClonable* clone = it->second;

		return clone->CreateObj();
	}
};

class AddClonable 
{
public:
	AddClonable( const char* className, const IClonable* clone ) {
		Clonables::Instance().AddClonable( className, clone );
	}
};