#pragma once

/***** Macros helpful to defining interface properties *****/

#define DECLARE_TYPE_GET(type, name) \
	__property type get_##name (); 

#define DECLARE_TYPE_SET(type, name) \
	__property void set_##name (type); 

#define DECLARE_TYPE(type, name) \
	DECLARE_TYPE_GET(type, name) \
	DECLARE_TYPE_SET(type, name)
	
#define STRCOL System::Collections::Specialized::StringCollection	
//#define STRCOL dtSearch::Engine::StringSetProperty

#define DECLARE_BOOLEAN_GET(name) DECLARE_TYPE_GET(Boolean, name);
#define DECLARE_BOOLEAN_SET(name) DECLARE_TYPE_SET(Boolean, name);
#define DECLARE_BOOLEAN(name) DECLARE_TYPE(Boolean, name)

#define DECLARE_STRING_GET(name) DECLARE_TYPE_GET(String*, name)
#define DECLARE_STRING_SET(name) DECLARE_TYPE_SET(String*, name)
#define DECLARE_STRING(name) DECLARE_TYPE(String*, name)

#define DECLARE_STRCOL_GET(name) DECLARE_TYPE_GET(STRCOL*, name)
#define DECLARE_STRCOL_SET(name) DECLARE_TYPE_SET(STRCOL*, name)
#define DECLARE_STRCOL(name) DECLARE_TYPE(STRCOL*, name)


#define DECLARE_INT_GET(name) DECLARE_TYPE_GET(long, name)
#define DECLARE_INT_SET(name) DECLARE_TYPE_SET(long, name)
#define DECLARE_INT(name) DECLARE_TYPE(long, name)


/***** Macros helpful to implementing class properties *****/

#define IMPLEMENT_TYPE_GET(type, name) \
	__property type get_##name ()	\
	{							\
		return m_##name;	\
	}								
										

#define IMPLEMENT_TYPE_SET(type, name) \
	__property void set_##name (type newValue)	\
	{								\
		m_##name = newValue;		\
	}													
														

#define IMPLEMENT_TYPE(type, name) \
	IMPLEMENT_TYPE_GET(type, name) \
	IMPLEMENT_TYPE_SET(type, name)

#define IMPLEMENT_BOOLEAN_GET(name) IMPLEMENT_TYPE_GET(Boolean, name);
#define IMPLEMENT_BOOLEAN_SET(name) IMPLEMENT_TYPE_SET(Boolean, name);
#define IMPLEMENT_BOOLEAN(name) IMPLEMENT_TYPE(Boolean, name)

#define IMPLEMENT_STRING_GET(name) IMPLEMENT_TYPE_GET(String*, name)
#define IMPLEMENT_STRING_SET(name) IMPLEMENT_TYPE_SET(String*, name)
#define IMPLEMENT_STRING(name) IMPLEMENT_TYPE(String*, name)

#define IMPLEMENT_STRCOL_GET(name) IMPLEMENT_TYPE_GET(STRCOL *, name)
#define IMPLEMENT_STRCOL_SET(name) IMPLEMENT_TYPE_SET(STRCOL *, name)
#define IMPLEMENT_STRCOL(name) IMPLEMENT_TYPE(STRCOL *, name)

#define IMPLEMENT_INT_GET(name) IMPLEMENT_TYPE_GET(long, name)
#define IMPLEMENT_INT_SET(name) IMPLEMENT_TYPE_SET(long, name)
#define IMPLEMENT_INT(name) IMPLEMENT_TYPE(long, name)

#define IMPLEMENT_INTARR(name) \
	__property int get_#name() __gc[] { return m_##name; } \
	__property void set_#name(int arr __gc[]) { m_##name = arr; }


/***** Macros helpful to declaring class fields *****/

#define FIELD_TYPE(type,name) type m_##name;
#define FIELD_TYPE_GET(type,name) FIELD_TYPE(type,name)
#define FIELD_TYPE_SET(type,name) FIELD_TYPE(type,name)

#define FIELD_STRING(name) FIELD_TYPE(String*, name)
#define FIELD_STRING_GET(name) FIELD_STRING(name)
#define FIELD_STRING_SET(name) FIELD_STRING(name)

#define FIELD_STRCOL(name) FIELD_TYPE(STRCOL*, name)
#define FIELD_STRCOL_GET(name) FIELD_STRCOL(name)
#define FIELD_STRCOL_SET(name) FIELD_STRCOL(name)

#define FIELD_INT(name) FIELD_TYPE(long, name)
#define FIELD_INT_GET(name) FIELD_INT(name)
#define FIELD_INT_SET(name) FIELD_INT(name)
#define FIELD_INTARR(name) int m_##name __gc[];

#define FIELD_BOOLEAN(name) FIELD_TYPE(Boolean, name)
#define FIELD_BOOLEAN_GET(name) FIELD_BOOLEAN(name)
#define FIELD_BOOLEAN_SET(name) FIELD_BOOLEAN(name)
