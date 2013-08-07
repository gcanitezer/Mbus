#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <mbus.h>
#include <mbusprotocol.h>
#include <mbusprotocolaux.h>
#include <mbusserial.h>

#include "mbus_config.h"


//------------------------------------------------------------------------------
// Primary addressing scanning of mbus devices.
//------------------------------------------------------------------------------
int scanvalue(uint8_t rxPin, uint8_t txPin)
{
    mbus_handle *handle;
    char *device;
    int address, baudrate = 9600;
    int ret;

    IF_SERIAL_DEBUG(printf_P(PSTR("Entered scanvalue \n")));


    SoftwareSerial mySerial(rxPin, txPin);


    //  mbus_register_send_event(&mbus_dump_send_event);
    //  mbus_register_recv_event(&mbus_dump_recv_event);


    if ((handle = mbus_connect_serial(&mySerial)) == NULL)
    {
    	IF_SERIAL_DEBUG(printf_P(PSTR("Failed to setup connection to M-bus gateway\n")));
        return 1;
    }

    if (mbus_serial_set_baudrate(handle->m_serial_handle, baudrate) == -1)
    {
    	IF_SERIAL_DEBUG(printf_P(PSTR("Failed to set baud rate.\n")));
        return 1;
    }


  	IF_SERIAL_DEBUG(printf_P(PSTR("Scanning primary addresses:\n")));

    for (address = 0; address <= 250; address++)
    {
        mbus_frame reply;

        memset((void *)&reply, 0, sizeof(mbus_frame));

        IF_SERIAL_DEBUG(printf_P(PSTR("%d "), address));

        if (mbus_send_ping_frame(handle, address) == -1)
        {
        	IF_SERIAL_DEBUG(printf_P(PSTR("Scan failed. Could not send ping frame\n")));
            return 1;
        } 
/*
        ret = mbus_recv_frame(handle, &reply);

        if (ret == -1)
        {
            continue;
        }
        
        IF_SERIAL_DEBUG(printf_P(PSTR("\n")));
        
        if (ret == -2)
        {
            //check for more data (collision)
            while (mbus_recv_frame(handle, &reply) != -1);
            
            IF_SERIAL_DEBUG(printf_P(PSTR("Collision at address %d\n"), address));
            
            continue;
        } 

        if (mbus_frame_type(&reply) == MBUS_FRAME_TYPE_ACK)
        {
           // check for more data (collision)
            while (mbus_recv_frame(handle, &reply) != -1)
            {
                ret = -2;
            }
    
            if (ret == -2)
            {
            	IF_SERIAL_DEBUG(printf_P(PSTR("Collision at address %d\n"), address));
                
                continue;
            }
                
            IF_SERIAL_DEBUG(printf_P(PSTR("Found a M-Bus device at address %d\n"), address));
        }
*/
    }

    mbus_disconnect(handle);

    return 0;
}
