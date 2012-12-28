#ifndef SINGLETON_H
#define SINGLETON_H

template < typename T >
class Singleton
{
protected:
    //************************************
    // Method:    Singleton
    // FullName:  Singleton<T>::Singleton
    // Access:    protected
    // Returns:
    // Qualifier:
    // Note: Never do anything important in any of the Singleton constructors!
    //************************************
    Singleton()
    {
        /*
        long long offset = (long long)(T *)1 - (long long)(Singleton *)(T *)1;
        instance = (T *)((long long)this + offset);
        */
    }
    virtual ~Singleton()
    {
        instance = NULL;
    }

public:
    static T * GetInstance()
    {
        if (instance == NULL)
            instance = new T;
        return instance;
    };

    static void DestroyInstance()
    {
        if(instance)
        {
            delete instance;
            instance = NULL;
        }
    };

private:
    static T * instance;
};

template<typename T> T* Singleton<T>::instance = NULL;

#define DEFINE_SINGLETON( ThisName )\
	public:\
	friend class Singleton<ThisName>;\
	private:\
	ThisName( void );\
	virtual ~ThisName( void );\
 
#define SINGLETON_CONSTRUCTOR( ThisName )\
	ThisName::ThisName( void ) : Singleton<ThisName>()

#define SINGLETON_DESTRUCTOR( ThisName )\
	ThisName::~ThisName( void )

#endif
