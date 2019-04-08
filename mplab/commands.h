/* 
 * File:   commands.h
 * Author: marcus
 *
 * Created on 11. April 2015, 07:15
 */

#ifndef COMMANDS_H
#define	COMMANDS_H

#define SPICMD_NONE             '\0'

#define SPICMD_SHUTTERUP          'U'
#define SPICMD_SHUTTERDOWN        'D'
#define SPICMD_SHUTTERSTOP        'S'

#define SPICMD_GETLASTKEY         'L'
#define SPICMD_SETKEYFORSHUTTER   'K'
#define SPICMD_GETKEYFORSHUTTER   'G'
#define SPICMD_GETSTATUS          'Z'

#define SPICMD_GETTESTPATTERN     'T'

#define SPICMD_GETSHUTTERGROUP    'M'
#define SPICMD_SETSHUTTERGROUP    'N'

#define SPICMD_SHUTTERGROUPUP     'O'
#define SPICMD_SHUTTERGROUPDOWN   'P'



#endif	/* COMMANDS_H */

