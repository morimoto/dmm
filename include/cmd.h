/************************************************************************


                                  cmd.h

2008/09  morimoto
************************************************************************/
#ifndef CMD_H
#define CMD_H

//======================================================================
//
//
//                                include
//
//
//======================================================================
#include <stdio.h>
#include "dmm.h"

//======================================================================
//
//
//                            struct , enum
//
//
//======================================================================
//=====================================
//
//          HELPMSG
//
//=====================================
typedef struct helpmsg {
    const char* pstrCmd;
    const char* pstrUsage;
    const char* pstrDetail;
} STHELPMSG;

//=====================================
//
//               STCMD
//
//=====================================
typedef struct cmd {
    const char *pstrName;
    STHELPMSG  *pHelp;
    bool        (*fnIsHit)( int , char ** );
    bool        (*fnCmd  )( int , char ** );
} STCMD;

//=====================================
//
//               STCMD
//
//=====================================
typedef struct cmdlist {
    const STCMD *pCmd;
    bool     isEnable;
    void    *pPrivateData;
} STCMDLIST;

//======================================================================
//
//
//                                 define
//
//
//======================================================================
#define HELPMSGEND  { NULL, NULL, NULL },

//=====================================
//
//          for cmd-XXX.c
//
//=====================================
#define INSTALL_CMD( name )     \
    const STCMD cmd_##name = {  \
        .pstrName  = #name,     \
        .pHelp     = s_Help,    \
        .fnIsHit   = ishit,     \
        .fnCmd     = cmd,       \
    };

#define CMD_HIT( name , cmd ) (0 == strcmp( name , cmd ))

//=====================================
//
//          for Makefile
//
//=====================================
#define CMD_EXTERN( name ) extern STCMD cmd_##name;
#define CMD_DEFINE( name ) { \
    .pCmd = &cmd_##name,     \
    .isEnable = false,       \
    .pPrivateData = NULL,    \
},

#define CMD_LIST_SIZE static int s_nCmdListSize = ARRAY_SIZE(s_tblCmdList);

//======================================================================
//
//
//                      function
//
//
//======================================================================
#define IsCmdHit( ) _IsCmdHit( pstrArgv[0] , s_Help )
bool _IsCmdHit( const char * , STHELPMSG * );
bool Usage( const char* );
bool DetailUsage( const char* );
enum ESTYPE CmdGetDataSize( char* , enum ESTYPE );

int GetCmdListSize( void );
const STCMD* GetCmdList( int );
bool IsEnableCmd( int );
bool IsEnableCmd( int );

//=====================================
//
//          for cpu
//
//=====================================
void EnableCmd( const char* );
void SetCmdData( const char*, void* );
void* GetCmdData( const char* );

//----------------------
// system function
//----------------------
void _sysCmdInit( void );

#endif

