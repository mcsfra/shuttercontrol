#include <xc.h>

//#include "shutter.h"
#include "masterswitch.h"
#include "hardware.h"
#include "buttons.h"

unsigned char upState         = 0;
unsigned char downState       = 0;
unsigned int  entprellCounter = 0;

/**/

void testeMasterTaster(void)  {

    if ( entprellCounter++ < 200)
        return;

   entprellCounter = 0;

   if ( MASTERSWITCHPORT_UP == 0)
       decodeButton(201);
   
   if ( MASTERSWITCHPORT_DOWN == 0)
       decodeButton(202);

}
/**/

