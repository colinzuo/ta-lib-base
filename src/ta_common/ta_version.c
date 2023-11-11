

#include <stddef.h>
#include <stdlib.h>

/* Version number controlled manually.
 *
 * Should be modified only by TA-Lib.org
 */
#define MAJOR "0"
#define MINOR "4"
#define BUILD "0"

/* Nothing to modify below this line. */

#define TA_VERSION_STRING(maj,min,build,date,time) maj"."min"."build" ("date" "time")"

const char  *TA_GetVersionString( void )
{
    return TA_VERSION_STRING(MAJOR,MINOR,BUILD,__DATE__,__TIME__);
}

const char *TA_GetVersionMajor( void )
{
   return MAJOR;
}

const char *TA_GetVersionMinor( void )
{
   return MINOR;
}

const char *TA_GetVersionBuild( void )
{
   return BUILD;
}

const char *TA_GetVersionDate( void )
{
   return __DATE__;
}

const char *TA_GetVersionTime( void )
{
   return __TIME__;
}
