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
        m_instance = nullptr;
    }

public:
    static T * getInstance()
    {
        if (m_instance == nullptr)
            m_instance = new T;
        return m_instance;
    };

    static void destroyInstance()
    {
        if(m_instance)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    };

private:
    static T * m_instance;
};

template<typename T> T* Singleton<T>::m_instance = nullptr;

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
