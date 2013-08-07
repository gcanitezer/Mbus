//------------------------------------------------------------------------------
// Copyright (C) 2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA 
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <strings.h>

//#include <termios.h>
#include <errno.h>
#include <string.h>

#include "mbus.h"
#include "mbusserial.h"
//2048
#define PACKET_BUFF_SIZE 256

//------------------------------------------------------------------------------
/// Set up a serial connection handle.
//------------------------------------------------------------------------------
/*goker
 SoftwareSerial *
mbus_serial_connect(char *device)
{
    SoftwareSerial *handle= &Serial;
    Serial.begin(2400);



    // Use blocking read and handle it by serial port VMIN/VTIME setting
    if ((handle->fd = open(handle->device, O_RDWR | O_NOCTTY)) < 0)
    {
        fprintf(stderr, "%s: failed to open tty.", __PRETTY_FUNCTION__);
        return NULL;
    }

    memset(&(handle->t), 0, sizeof(handle->t));
    handle->t.c_cflag |= (CS8|CREAD|CLOCAL);
    handle->t.c_cflag |= PARENB;

    // No received data still OK
    handle->t.c_cc[VMIN]  = 0;

    // Wait at most 0.2 sec.Note that it starts after first received byte!!
    // I.e. if CMIN>0 and there are no data we would still wait forever...
    //
    // The specification mentions link layer response timeout this way:
    // The time structure of various link layer communication types is described in EN60870-5-1. The answer time
    // between the end of a master send telegram and the beginning of the response telegram of the slave shall be
    // between 11 bit times and (330 bit times + 50ms).
    //  
    // For 2400Bd this means (330 + 11) / 2400 + 0.05 = 188.75 ms (added 11 bit periods to receive first byte).
    // I.e. timeout of 0.2s seems appropriate for 2400Bd.

    handle->t.c_cc[VTIME] = 2; // Timeout in 1/10 sec

    cfsetispeed(&(handle->t), B2400);
    cfsetospeed(&(handle->t), B2400);

#ifdef MBUS_SERIAL_DEBUG
    printf("%s: t.c_cflag = %x\n", __PRETTY_FUNCTION__, handle->t.c_cflag);
    printf("%s: t.c_oflag = %x\n", __PRETTY_FUNCTION__, handle->t.c_oflag);
    printf("%s: t.c_iflag = %x\n", __PRETTY_FUNCTION__, handle->t.c_iflag);
    printf("%s: t.c_lflag = %x\n", __PRETTY_FUNCTION__, handle->t.c_lflag);
#endif 

    tcsetattr(handle->fd, TCSANOW, &(handle->t));

    return handle;    
}
*/
//------------------------------------------------------------------------------
// Set baud rate for serial connection
//------------------------------------------------------------------------------
int
mbus_serial_set_baudrate(SoftwareSerial *handle, int baudrate)
{
	handle->begin(baudrate);

  /*goker
    speed_t speed;

    if (handle == NULL)
        return -1;

    switch (baudrate)
    {
        case 300:
            speed = B300;
            handle->t.c_cc[VTIME] = 12; // Timeout in 1/10 sec
            break;

        case 1200:
            speed = B1200;
            handle->t.c_cc[VTIME] = 4;  // Timeout in 1/10 sec
            break;

        case 2400:
            speed = B2400;
            handle->t.c_cc[VTIME] = 2;  // Timeout in 1/10 sec
            break;

        case 9600:
            speed = B9600;
            handle->t.c_cc[VTIME] = 1;  // Timeout in 1/10 sec
            break;

       default:
            return -1; // unsupported baudrate
    }

    // Set input baud rate
    if (cfsetispeed(&(handle->t), speed) != 0)
    {
        return -1;
    }

    // Set output baud rate
    if (cfsetospeed(&(handle->t), speed) != 0)
    {
        return -1;
    }

    // Change baud rate immediately
    if (tcsetattr(handle->fd, TCSANOW, &(handle->t)) != 0)
    {
        return -1;
    }
*/
    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int
mbus_serial_disconnect(SoftwareSerial *handle)
{
	handle->end();
 /*goker   if (handle == NULL)
    {
        return -1;
    }

    close(handle->fd);
    
    free(handle);
*/
    return 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int
mbus_serial_send_frame(SoftwareSerial *handle, mbus_frame *frame)
{
    uint8_t buff[PACKET_BUFF_SIZE];
    int len, ret;

    IF_SERIAL_DEBUG(printf_P(PSTR("%s: Entered \n"), "mbus_serial_send_frame"));

    if (handle == NULL || frame == NULL)
    {
        return -1;
    }

    if ((len = mbus_frame_pack(frame, buff, sizeof(buff))) == -1)
    {
    	IF_SERIAL_DEBUG(printf_P(PSTR("%s: mbus_frame_pack failed\n"), "mbus_serial_send_frame"));
        return -1;
    }

//#ifdef MBUS_SERIAL_DEBUG
    // if debug, dump in HEX form to stdout what we write to the serial port
    printf_P(PSTR("%s: Dumping M-Bus frame [%d bytes]: "), __PRETTY_FUNCTION__, len);
    for (int i = 0; i < len; i++)
    {
       printf_P(PSTR("%.2X "), buff[i]);
    }
    printf_P(PSTR("\n"));

//#endif

    if ((ret = handle->write(buff, len)) == len)
    {
        //
        // call the send event function, if the callback function is registered
        // 
        if (_mbus_send_event)
                _mbus_send_event(MBUS_HANDLE_TYPE_SERIAL, buff, len);
    }
    else
    {   
    	IF_SERIAL_DEBUG(printf_P(PSTR("%s: Failed to write frame to socket (ret = %d: )\n"), "mbus_serial_send_frame", ret));
        return -1;
    }
    
    //
    // wait until complete frame has been transmitted
    //
    handle->flush();

    return 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int
mbus_serial_recv_frame(SoftwareSerial *handle, mbus_frame *frame)
{
    uint8_t buff[PACKET_BUFF_SIZE];
    int len, remaining, nread, timeouts;

    printf_P(PSTR("%s: Entered \n"), __PRETTY_FUNCTION__);

    if (handle == NULL || frame == NULL)
    {
        printf_P(PSTR("%s: Invalid parameter.\n"), __PRETTY_FUNCTION__);
        return -1;
    }

    memset(buff, 0, sizeof(buff));

    //
    // read data until a packet is received
    //
    remaining = 1; // start by reading 1 byte
    len = 0;
    timeouts = 0;
    printf_P(PSTR("%s: Entered3 \n"), __PRETTY_FUNCTION__);
    do {
        printf_P(PSTR("%s: Attempt to read %d bytes [len = %d]\n"), __PRETTY_FUNCTION__, remaining, len);
    	while(handle->available()>0 && len<PACKET_BUFF_SIZE)
    	{
    		buff[len] = handle->read();
    		len++;
    	}


    } while ((remaining = mbus_parse(frame, buff, len)) > 0);

    if (len == 0)
    {
        // No data received
        return -1;
    }
    
    //
    // call the receive event function, if the callback function is registered
    // 
    if (_mbus_recv_event)
        _mbus_recv_event(MBUS_HANDLE_TYPE_SERIAL, buff, len);
      
    if (remaining != 0)
    {
        // Would be OK when e.g. scanning the bus, otherwise it is a failure.
        printf_P(PSTR("%s: M-Bus layer failed to receive complete data.\n"), __PRETTY_FUNCTION__);
        return -2;
    }

    if (len == -1)
    {
        printf_P(PSTR("%s: M-Bus layer failed to parse data.\n"), __PRETTY_FUNCTION__);
        return -1;
    }

    return 0;
}



