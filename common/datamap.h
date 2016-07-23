//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef DATAMAP_H
#define DATAMAP_H
#ifdef _WIN32
#pragma once
#endif

#include "utlarray.h"

// SINGLE_INHERITANCE restricts the size of CBaseEntity pointers-to-member-functions to 4 bytes
class __single_inheritance CBaseEntity;

//-----------------------------------------------------------------------------
// Field sizes... 
//-----------------------------------------------------------------------------
template <int FIELD_TYPE>
class CDatamapFieldSizeDeducer
{
public:
	enum
	{
		SIZE = 0
	};

	static int FieldSize( )
	{
		return 0;
	}
};

#define DECLARE_FIELD_SIZE( _fieldType, _fieldSize )	\
	template< > class CDatamapFieldSizeDeducer<_fieldType> { public: enum { SIZE = _fieldSize }; static int FieldSize() { return _fieldSize; }};
#define FIELD_SIZE( _fieldType )		CDatamapFieldSizeDeducer<_fieldType>::SIZE

DECLARE_FIELD_SIZE( FIELD_FLOAT,		sizeof(float))
DECLARE_FIELD_SIZE( FIELD_STRING,		sizeof(int))
DECLARE_FIELD_SIZE( FIELD_ENTITY,		sizeof(int))
DECLARE_FIELD_SIZE( FIELD_CLASSPTR,		sizeof(int))
DECLARE_FIELD_SIZE( FIELD_EHANDLE,		sizeof(int))
DECLARE_FIELD_SIZE( FIELD_EVARS,		sizeof(int))
DECLARE_FIELD_SIZE( FIELD_EDICT,		sizeof(int))
DECLARE_FIELD_SIZE( FIELD_VECTOR,		3 * sizeof(float))
DECLARE_FIELD_SIZE( FIELD_POSITION_VECTOR, 	3 * sizeof(float))
DECLARE_FIELD_SIZE( FIELD_POINTER,		sizeof(int *))
DECLARE_FIELD_SIZE( FIELD_INTEGER,		sizeof(int))
DECLARE_FIELD_SIZE( FIELD_FUNCTION,		sizeof(int *))
DECLARE_FIELD_SIZE( FIELD_BOOLEAN,		sizeof(char))
DECLARE_FIELD_SIZE( FIELD_SHORT,		sizeof(short))
DECLARE_FIELD_SIZE( FIELD_CHARACTER,		sizeof(char))
DECLARE_FIELD_SIZE( FIELD_TIME,		sizeof(float))
DECLARE_FIELD_SIZE( FIELD_MODELNAME,		sizeof(int))
DECLARE_FIELD_SIZE( FIELD_SOUNDNAME,		sizeof(int))

#define _FIELD(name, fieldtype, count, flags, mapname )		{ fieldtype, #name, offsetof(classNameTypedef, name), count, flags, mapname, NULL }
#define _EFIELD(name, fieldtype, count, flags, mapname )		{ fieldtype, #name, offsetof(entvars_t, name), count, flags, mapname, NULL }
#define DEFINE_FIELD_NULL					{ FIELD_VOID, 0, 0, 0, 0, 0, 0 }
#define DEFINE_FIELD(name, fieldtype)				_FIELD(name, fieldtype, 1, FTYPEDESC_SAVE, NULL )
#define DEFINE_KEYFIELD(name, fieldtype, mapname)			_FIELD(name, fieldtype, 1, FTYPEDESC_KEY|FTYPEDESC_SAVE, mapname )
#define DEFINE_AUTO_ARRAY(name, fieldtype)			_FIELD(name, fieldtype, ARRAYSIZE(((classNameTypedef *)0)->name), FTYPEDESC_SAVE, NULL )
#define DEFINE_ARRAY(name, fieldtype, count)			_FIELD(name, fieldtype, count, FTYPEDESC_SAVE, NULL )
#define DEFINE_ENTITY_FIELD(name, fieldtype)			_EFIELD(name, fieldtype, 1, FTYPEDESC_KEY|FTYPEDESC_SAVE, #name )
#define DEFINE_ENTITY_GLOBAL_FIELD(name, fieldtype)		_EFIELD(name, fieldtype, 1, FTYPEDESC_KEY|FTYPEDESC_SAVE|FTYPEDESC_GLOBAL, #name )
#define DEFINE_GLOBAL_FIELD(name, fieldtype)			_FIELD(name, fieldtype, 1, FTYPEDESC_GLOBAL|FTYPEDESC_SAVE, NULL )
#define DEFINE_GLOBAL_KEYFIELD(name, fieldtype, mapname)		_FIELD(name, fieldtype, 1, FTYPEDESC_GLOBAL|FTYPEDESC_KEY|FTYPEDESC_SAVE, mapname )

// replaces EXPORT table for portability and non-DLL based systems (xbox)
#define DEFINE_FUNCTION_RAW( function, func_type )		{ FIELD_VOID, nameHolder.GenerateName(#function), 0, 1, FTYPEDESC_FUNCTIONTABLE, NULL, (func_t)((func_type)(&classNameTypedef::function)) }
#define DEFINE_FUNCTION( function )				DEFINE_FUNCTION_RAW( function, func_t )

//
// Generic function prototype.
//
typedef void (CBaseEntity::*func_t)(void);

typedef struct typedescription_s
{
	FIELDTYPE		fieldType;
	const char	*fieldName;
	int		fieldOffset;
	unsigned short	fieldSize;
	short		flags;
	const char	*mapName;				// a name of keyfield on a map (e.g. "origin", "spawnflags" etc)	
	func_t		func;				// pointer to a function 
} TYPEDESCRIPTION;


//-----------------------------------------------------------------------------
// Purpose: stores the list of objects in the hierarchy
//          used to iterate through an object's data descriptions
//-----------------------------------------------------------------------------
typedef struct datamap_s
{
	TYPEDESCRIPTION	*dataDesc;
	int		dataNumFields;
	char const	*dataClassName;
	datamap_s		*baseMap;
} DATAMAP;

//-----------------------------------------------------------------------------
//
// Macros used to implement datadescs
//
#define DECLARE_SIMPLE_DATADESC() \
	static DATAMAP m_DataMap; \
	static DATAMAP *GetBaseMap(); \
	template <typename T> friend void DataMapAccess(T *, DATAMAP **p); \
	template <typename T> friend DATAMAP *DataMapInit(T *);

#define DECLARE_DATADESC() \
	DECLARE_SIMPLE_DATADESC() \
	virtual DATAMAP *GetDataDescMap( void );

#define BEGIN_DATADESC( className ) \
	DATAMAP className::m_DataMap = { 0, 0, #className, NULL }; \
	DATAMAP *className::GetDataDescMap( void ) { return &m_DataMap; } \
	DATAMAP *className::GetBaseMap() { DATAMAP *pResult; DataMapAccess((BaseClass *)NULL, &pResult); return pResult; } \
	BEGIN_DATADESC_GUTS( className )

#define BEGIN_DATADESC_NO_BASE( className ) \
	DATAMAP className::m_DataMap = { 0, 0, #className, NULL }; \
	DATAMAP *className::GetDataDescMap( void ) { return &m_DataMap; } \
	DATAMAP *className::GetBaseMap() { return NULL; } \
	BEGIN_DATADESC_GUTS( className )

#define BEGIN_SIMPLE_DATADESC( className ) \
	DATAMAP className::m_DataMap = { 0, 0, #className, NULL }; \
	DATAMAP *className::GetBaseMap() { return NULL; } \
	BEGIN_DATADESC_GUTS( className )

#define BEGIN_DATADESC_GUTS( className ) \
	template <typename T> DATAMAP *DataMapInit(T *); \
	template <> DATAMAP *DataMapInit<className>( className * ); \
	namespace className##_DataDescInit \
	{ \
		DATAMAP *g_DataMapHolder = DataMapInit( (className *)NULL ); /* This can/will be used for some clean up duties later */ \
	} \
	\
	template <> DATAMAP *DataMapInit<className>( className * ) \
	{ \
		typedef className classNameTypedef; \
		static CDatadescGeneratedNameHolder nameHolder(#className); \
		className::m_DataMap.baseMap = className::GetBaseMap(); \
		static TYPEDESCRIPTION dataDesc[] = \
		{ \
		{ FIELD_VOID,0,0,0,0,0,0}, /* so you can define "empty" tables */

#define END_DATADESC() \
		}; \
		\
		if ( sizeof( dataDesc ) > sizeof( dataDesc[0] ) ) \
		{ \
			classNameTypedef::m_DataMap.dataNumFields = ARRAYSIZE( dataDesc ) - 1; \
			classNameTypedef::m_DataMap.dataDesc 	  = &dataDesc[1]; \
		} \
		else \
		{ \
			classNameTypedef::m_DataMap.dataNumFields = 1; \
			classNameTypedef::m_DataMap.dataDesc 	  = dataDesc; \
		} \
		return &classNameTypedef::m_DataMap; \
	}

//-----------------------------------------------------------------------------

#define DECLARE_CLASS( className, baseClassName ) \
	typedef baseClassName BaseClass; \
	typedef className ThisClass;

#define DECLARE_CLASS_NOBASE( className ) \
	typedef className ThisClass;

//-----------------------------------------------------------------------------

template <typename T> 
inline void DataMapAccess(T *ignored, DATAMAP **p)
{
	*p = &T::m_DataMap;
}

//-----------------------------------------------------------------------------

class CDatadescGeneratedNameHolder
{
public:
	CDatadescGeneratedNameHolder( const char *pszBase ) : m_pszBase(pszBase)
	{
		m_nLenBase = strlen( m_pszBase ) + 2;
	}
	
	~CDatadescGeneratedNameHolder()
	{
		for( int i = 0; i < m_Names.Count(); i++ )
		{
			delete m_Names[i];
		}
	}
	
	const char *GenerateName( const char *pszIdentifier )
	{
		char *pBuf = new char[m_nLenBase + strlen(pszIdentifier) + 1];
		sprintf( pBuf, "%s::", m_pszBase );
		strcat( pBuf, pszIdentifier );
		m_Names.AddToTail( pBuf );
		return pBuf;
	}
	
private:
	const char *m_pszBase;
	size_t m_nLenBase;
	CUtlArray<char *> m_Names;
};

#endif // DATAMAP_H