//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "materialsystem/imaterialproxy.h"
#include "materialproxyfactory.h"
#include "toolframework/itoolframework.h"
#include "toolframework/itoolsystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
extern CSysModule		*g_ClientDLLModule;

IMaterialProxy *CMaterialProxyFactory::CreateProxy( const char *proxyName )
{
#if !defined(SWDS)
	IMaterialProxy *materialProxy = LookupProxy( proxyName, Sys_GetFactory( g_ClientDLLModule ) );

	// If the client didn't have it and we're in tool mode, ask the tools...
	if ( toolframework->InToolMode() && !materialProxy )
	{
		materialProxy = toolframework->LookupProxy( proxyName );
	}

	if( !materialProxy )
	{
		ConDMsg( "Can't find material proxy \"%s\"\n", proxyName );
		return NULL;
	}
	return materialProxy;
#else
	return NULL;
#endif
}

void CMaterialProxyFactory::DeleteProxy( IMaterialProxy *pProxy )
{
	// how do you delete something generated by an interface.h factory?
	if( pProxy )
	{
		pProxy->Release();
	}
}


//-----------------------------------------------------------------------------
// Look up proxy
//-----------------------------------------------------------------------------
IMaterialProxy *CMaterialProxyFactory::LookupProxy( const char *proxyName, CreateInterfaceFn factory )
{
	if( !factory )
		return NULL;

	// allocate exactly enough memory for the versioned name on the stack.
	char *proxyVersionedName;
	int buflen = Q_strlen( proxyName ) + Q_strlen( IMATERIAL_PROXY_INTERFACE_VERSION ) + 1;

	proxyVersionedName = ( char * )_alloca( buflen );
	Q_strncpy( proxyVersionedName, proxyName, buflen );
	Q_strncat( proxyVersionedName, IMATERIAL_PROXY_INTERFACE_VERSION, buflen, COPY_ALL_CHARACTERS );
	return ( IMaterialProxy * )factory( proxyVersionedName, NULL );
}



