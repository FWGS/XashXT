/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
/*

===== lights.cpp ========================================================

  spawn and think functions for editor-placed lights

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"

class CLight : public CPointEntity
{
	DECLARE_CLASS( CLight, CPointEntity );
public:
	void	KeyValue( KeyValueData* pkvd ); 
	void	Spawn( void );
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	Think( void );

	DECLARE_DATADESC();

	int ObjectCaps( void ) { return (BaseClass::ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | FCAP_NOT_MASTER | FCAP_IGNORE_PARENT; }
	virtual STATE GetState( void ) { return m_iState; };

	string_t	GetStyle( void ) { return m_iszCurrentStyle; };
	void	SetStyle( int iszPattern );
	void	SetCorrectStyle( void );

private:
	STATE	m_iState;		// current state
	int	m_iOnStyle;	// style to use while on
	int	m_iOffStyle;	// style to use while off
	int	m_iTurnOnStyle;	// style to use while turning on
	int	m_iTurnOffStyle;	// style to use while turning off
	int	m_iTurnOnTime;	// time taken to turn on
	int	m_iTurnOffTime;	// time taken to turn off
	string_t	m_iszPattern;	// custom style to use while on
	string_t	m_iszCurrentStyle;	// current style string
};

LINK_ENTITY_TO_CLASS( light, CLight );

BEGIN_DATADESC( CLight )
	DEFINE_FIELD( m_iState, FIELD_INTEGER ),
	DEFINE_KEYFIELD( m_iszPattern, FIELD_STRING, "pattern" ),
	DEFINE_FIELD( m_iszCurrentStyle, FIELD_STRING ),
	DEFINE_KEYFIELD( m_iOnStyle, FIELD_INTEGER, "m_iOnStyle" ),
	DEFINE_KEYFIELD( m_iOffStyle, FIELD_INTEGER, "m_iOffStyle" ),
	DEFINE_KEYFIELD( m_iTurnOnStyle, FIELD_INTEGER, "m_iTurnOnStyle" ),
	DEFINE_KEYFIELD( m_iTurnOffStyle, FIELD_INTEGER, "m_iTurnOffStyle" ),
	DEFINE_KEYFIELD( m_iTurnOnTime, FIELD_INTEGER, "m_iTurnOnTime" ),
	DEFINE_KEYFIELD( m_iTurnOffTime, FIELD_INTEGER, "m_iTurnOffTime" ),
END_DATADESC()


//
// Cache user-entity-field values until spawn is called.
//
void CLight :: KeyValue( KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "m_iOnStyle"))
	{
		m_iOnStyle = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iOffStyle"))
	{
		m_iOffStyle = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iTurnOnStyle"))
	{
		m_iTurnOnStyle = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iTurnOffStyle"))
	{
		m_iTurnOffStyle = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iTurnOnTime"))
	{
		m_iTurnOnTime = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iTurnOffTime"))
	{
		m_iTurnOffTime = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
 	else if (FStrEq(pkvd->szKeyName, "pitch"))
	{
		Vector angles = GetAbsAngles();
		angles.x = Q_atof(pkvd->szValue);
		SetAbsAngles( angles );
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "pattern"))
	{
		m_iszPattern = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "firetarget"))
	{
		pev->target = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
	{
		CPointEntity::KeyValue( pkvd );
	}
}

void CLight :: SetStyle ( int iszPattern )
{
	if (m_iStyle < 32) // if it's using a global style, don't change it
		return;

	m_iszCurrentStyle = iszPattern;
	LIGHT_STYLE(m_iStyle, (char *)STRING( iszPattern ));
}

// regardless of what's been set by trigger_lightstyle ents, set the style I think I need
void CLight :: SetCorrectStyle ( void )
{
	if (m_iStyle >= 32)
	{
		switch (m_iState)
		{
		case STATE_ON:
			if (m_iszPattern) // custom styles have priority over standard ones
				SetStyle( m_iszPattern );
			else if (m_iOnStyle)
				SetStyle(GetStdLightStyle(m_iOnStyle));
			else
				SetStyle(MAKE_STRING("m"));
			break;
		case STATE_OFF:
			if (m_iOffStyle)
				SetStyle(GetStdLightStyle(m_iOffStyle));
			else
				SetStyle(MAKE_STRING("a"));
			break;
		case STATE_TURN_ON:
			if (m_iTurnOnStyle)
				SetStyle(GetStdLightStyle(m_iTurnOnStyle));
			else
				SetStyle(MAKE_STRING("a"));
			break;
		case STATE_TURN_OFF:
			if (m_iTurnOffStyle)
				SetStyle(GetStdLightStyle(m_iTurnOffStyle));
			else
				SetStyle(MAKE_STRING("m"));
			break;
		}
	}
	else
	{
		m_iszCurrentStyle = GetStdLightStyle( m_iStyle );
	}
}

void CLight :: Think( void )
{
	switch( GetState( ))
	{
	case STATE_TURN_ON:
		m_iState = STATE_ON;
		UTIL_FireTargets( pev->target, this, this, USE_ON, 0 );
		break;
	case STATE_TURN_OFF:
		m_iState = STATE_OFF;
		UTIL_FireTargets( pev->target, this, this, USE_OFF, 0 );
		break;
	}

	SetCorrectStyle();
}

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) LIGHT_START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
*/
void CLight :: Spawn( void )
{
	if( FStringNull( pev->targetname ))
	{
		// inert light
		REMOVE_ENTITY( edict() );
		return;
	}
	
	if( FBitSet( pev->spawnflags, SF_LIGHT_START_OFF ))
		m_iState = STATE_OFF;
	else
		m_iState = STATE_ON;

	SetCorrectStyle();
}

void CLight :: Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if (m_iStyle >= 32)
	{
		if ( !ShouldToggle( useType ))
			return;

		switch( GetState( ))
		{
		case STATE_ON:
		case STATE_TURN_ON:
			if (m_iTurnOffTime)
			{
				m_iState = STATE_TURN_OFF;
				SetNextThink( m_iTurnOffTime );
			}
			else
				m_iState = STATE_OFF;
			break;
		case STATE_OFF:
		case STATE_TURN_OFF:
			if (m_iTurnOnTime)
			{
				m_iState = STATE_TURN_ON;
				SetNextThink( m_iTurnOnTime );
			}
			else
				m_iState = STATE_ON;
			break;
		}
		SetCorrectStyle();
	}
}

//
// shut up spawn functions for new spotlights
//
LINK_ENTITY_TO_CLASS( light_spot, CLight );


class CEnvLight : public CLight
{
	DECLARE_CLASS( CEnvLight, CLight );
public:
	void	KeyValue( KeyValueData* pkvd ); 
	void	Spawn( void );
};

LINK_ENTITY_TO_CLASS( light_environment, CEnvLight );

void CEnvLight::KeyValue( KeyValueData* pkvd )
{
	if (FStrEq(pkvd->szKeyName, "_light"))
	{
		int r, g, b, v, j;
		char szColor[64];
		j = sscanf( pkvd->szValue, "%d %d %d %d\n", &r, &g, &b, &v );
		if (j == 1)
		{
			g = b = r;
		}
		else if (j == 4)
		{
			r = r * (v / 255.0);
			g = g * (v / 255.0);
			b = b * (v / 255.0);
		}

		// simulate qrad direct, ambient,and gamma adjustments, as well as engine scaling
		r = pow( r / 114.0, 0.6 ) * 264;
		g = pow( g / 114.0, 0.6 ) * 264;
		b = pow( b / 114.0, 0.6 ) * 264;

		pkvd->fHandled = TRUE;
		sprintf( szColor, "%d", r );
		CVAR_SET_STRING( "sv_skycolor_r", szColor );
		sprintf( szColor, "%d", g );
		CVAR_SET_STRING( "sv_skycolor_g", szColor );
		sprintf( szColor, "%d", b );
		CVAR_SET_STRING( "sv_skycolor_b", szColor );
	}
	else
	{
		CLight::KeyValue( pkvd );
	}
}


void CEnvLight :: Spawn( void )
{
	char szVector[64];
	UTIL_MakeAimVectors( GetAbsAngles() );

	sprintf( szVector, "%f", gpGlobals->v_forward.x );
	CVAR_SET_STRING( "sv_skyvec_x", szVector );
	sprintf( szVector, "%f", gpGlobals->v_forward.y );
	CVAR_SET_STRING( "sv_skyvec_y", szVector );
	sprintf( szVector, "%f", gpGlobals->v_forward.z );
	CVAR_SET_STRING( "sv_skyvec_z", szVector );

	CLight::Spawn( );
}

//**********************************************************
//LRC- the CTriggerLightstyle entity - changes the style of a light temporarily.
//**********************************************************
class CLightFader : public CPointEntity
{
	DECLARE_CLASS( CLightFader, CPointEntity );
public:
	void FadeThink( void );
	void WaitThink( void );

	DECLARE_DATADESC();

	CLight	*m_pLight;
	char	m_cFrom;
	char	m_cTo;
	char	m_szCurStyle[2];
	float	m_fEndTime;
	string_t	m_iszPattern;
	float	m_fStep;
	int	m_iWait;
};

LINK_ENTITY_TO_CLASS( lightfader, CLightFader );

BEGIN_DATADESC( CLightFader )
	DEFINE_FIELD( m_pLight, FIELD_CLASSPTR ),
	DEFINE_FIELD( m_cFrom, FIELD_CHARACTER ),
	DEFINE_FIELD( m_cTo, FIELD_CHARACTER ),
	DEFINE_AUTO_ARRAY( m_szCurStyle, FIELD_CHARACTER ),
	DEFINE_FIELD( m_fEndTime, FIELD_TIME ),
	DEFINE_FIELD( m_iszPattern, FIELD_STRING ),
	DEFINE_FIELD( m_fStep, FIELD_FLOAT ),
	DEFINE_FIELD( m_iWait, FIELD_INTEGER ),
	DEFINE_FUNCTION( FadeThink ),
	DEFINE_FUNCTION( WaitThink ),
END_DATADESC()

void CLightFader::FadeThink( void )
{
	if (m_fEndTime > gpGlobals->time)
	{
		m_szCurStyle[0] = m_cTo + (char)((m_cFrom - m_cTo) * (m_fEndTime - gpGlobals->time) * m_fStep);
		m_szCurStyle[1] = 0; // null terminator
		m_pLight->SetStyle(MAKE_STRING(m_szCurStyle));
		SetNextThink( 0.1 );
	}
	else
	{
		// fade is finished
		m_pLight->SetStyle(m_iszPattern);
		if (m_iWait > -1)
		{
			// wait until it's time to switch off
			SetThink(&CLightFader:: WaitThink );
			SetNextThink( m_iWait );
		}
		else
		{
			// we've finished, kill the fader
			SetThink(&CLightFader:: SUB_Remove );
			SetNextThink( 0.1 );
		}
	}
}

// we've finished. revert the light and kill the fader.
void CLightFader::WaitThink( void )
{
	if( m_iszPattern )
		m_pLight->SetStyle( m_iszPattern );
	else m_pLight->SetCorrectStyle();
	SetThink( SUB_Remove );
	SetNextThink( 0.1 );
}

class CTriggerLightstyle : public CPointEntity
{
	DECLARE_CLASS( CTriggerLightstyle, CPointEntity );
public:
	void KeyValue( KeyValueData *pkvd );
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	DECLARE_DATADESC();
private:
	char	m_szOldPattern[256];
	string_t	m_iszPattern;
	int	m_iFade;
	int	m_iWait;
};

LINK_ENTITY_TO_CLASS( trigger_lightstyle, CTriggerLightstyle );

BEGIN_DATADESC( CTriggerLightstyle )
	DEFINE_KEYFIELD( m_iszPattern, FIELD_STRING, "pattern" ),
	DEFINE_KEYFIELD( m_iFade, FIELD_INTEGER, "m_iFade" ),
	DEFINE_KEYFIELD( m_iWait, FIELD_INTEGER, "m_iWait" ),
	DEFINE_AUTO_ARRAY( m_szOldPattern, FIELD_CHARACTER ),
END_DATADESC()

void CTriggerLightstyle::KeyValue( KeyValueData *pkvd )
{
	if (FStrEq(pkvd->szKeyName, "pattern"))
	{
		m_iszPattern = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iFade"))
	{
		m_iFade = Q_atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iWait"))
	{
		m_iWait = Q_atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
		CBaseEntity::KeyValue( pkvd );
}

void CTriggerLightstyle::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CBaseEntity *pTarget = NULL;
	if ( !pev->target )
		return;

	//ALERT( at_console, "Lightstyle change for: (%s)\n", STRING(pev->target) );

	while( 1 )
	{
		pTarget = UTIL_FindEntityByTargetname(pTarget,STRING(pev->target), pActivator);
		if (FNullEnt(pTarget))
			break;

		int iszPattern;
		if (m_iszPattern)
			iszPattern = m_iszPattern;
		else
			iszPattern = GetStdLightStyle(m_iStyle);

		// not a light entity?
		if (!FClassnameIs(pTarget->pev, "light") && !FClassnameIs(pTarget->pev, "light_spot") && !FClassnameIs(pTarget->pev, "light_environment"))
		{
			if (pTarget->m_iStyle >= 32)
				LIGHT_STYLE( pTarget->m_iStyle, (char*)STRING( iszPattern ));
		}
		else
		{
			CLight *pLight = (CLight *)pTarget;

			if( m_iFade )
			{
				CLightFader *pFader = GetClassPtr( (CLightFader*)NULL );
				pFader->pev->classname = MAKE_STRING( "lightfader" );
				pFader->m_pLight = pLight;
				pFader->m_cFrom = ((char*)STRING(pLight->GetStyle()))[0];
				pFader->m_cTo = ((char*)STRING(iszPattern))[0];
				pFader->m_iszPattern = iszPattern;
				pFader->m_fEndTime = gpGlobals->time + m_iFade;
				pFader->m_fStep = 1.0f / m_iFade;
				pFader->m_iWait = m_iWait;
				pFader->SetThink( &CLightFader::FadeThink );
				pFader->SetNextThink( 0.1 );
			}
			else
			{
				if( g_iXashEngineBuildNumber >= 2000 )
				{
					// save old pattern in case we needs to be restore it
					Q_strncpy( m_szOldPattern, GET_LIGHT_STYLE( pLight->m_iStyle ), 256 ); 
				}

				pLight->SetStyle( iszPattern );

				if( m_iWait != -1 )
				{
					CLightFader *pFader = GetClassPtr( (CLightFader*)NULL );
					pFader->pev->classname = MAKE_STRING( "lightfader" );
					pFader->m_pLight = pLight;
					if( g_iXashEngineBuildNumber >= 2000 )
					{
						// i'm hope somebody don't delete this entity from map :-)
						pFader->m_iszPattern = MAKE_STRING( m_szOldPattern );
					}
					pFader->SetThink( &CLightFader::WaitThink );
					pFader->SetNextThink( m_iWait );
				}
			}
		}
	}
}