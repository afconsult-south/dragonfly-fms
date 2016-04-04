/*****************************************************************************
 * @file    flightcontrol.cpp
 * @brief	Flightcontrol handles the communication with the flight control
 * 			board, FCB.
 * 			It handles high level tasks concerning the flight.
 ******************************************************************************/

#include <iostream>
#include <fcntl.h>
#include <stropts.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "flightcontrol.h"
#include "com.h"

#include "dragonfly_fcb.pb.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"

using namespace std;

/**
 * @brief	Constructor of flightcontrol.
 * 			Opens the communication to the flight control board.
 * @param	None.
 * @retval	None.
 */
flightcontrol::flightcontrol()
{
	fcbCom.openCom("/dev/ttyS2", B115200);
}

/**
 * @brief	Gets the sensor values from the flight control board
 * 			in nonserialized format
 * @param	None.
 * @retval	1 if OK, 0 if NOT OK.
 */
int flightcontrol::getReceiver()
{
	ReceiverSignalValuesProto receiverSignalsProto; //struct that contains receiver data
	char buffer[ReceiverSignalValuesProto_size];	//encoded data
	bool status = false;

	//empty first
	fcbCom.readCom(&buffer[0], sizeof(buffer));
	//send message
	fcbCom.writeCom("get-receiver p\r", ReceiverSignalValuesProto_size);
	sleep(1);
	//receive message
	fcbCom.readCom(&buffer[0], sizeof(buffer));

	//decode the reply
	//TODO Remove the first bytes containing message id and size
	//create a stream
	pb_istream_t stream = pb_istream_from_buffer((uint8_t*)(&buffer[0]), (size_t)ReceiverSignalValuesProto_size);
	//decode
	status = pb_decode(&stream, ReceiverSignalValuesProto_fields, &receiverSignalsProto);

	if(!status)
	{
		cout << "Error decoding get-receiver" << endl;
		return 0;
	}
	//print data
	(receiverSignalsProto.has_throttle) ? (cout << "Throttle" << receiverSignalsProto.throttle) : (cout << "No Throttle"); cout << endl;
	(receiverSignalsProto.has_aileron)  ? (cout << "Aileron" << receiverSignalsProto.aileron)   : (cout << "No Aileron"); cout << endl;
	(receiverSignalsProto.has_elevator) ? (cout << "Elevator" << receiverSignalsProto.elevator) : (cout << "No Elevator"); cout << endl;
	(receiverSignalsProto.has_rudder)   ? (cout << "Rudder" << receiverSignalsProto.rudder)     : (cout << "No Rudder"); cout << endl;
	(receiverSignalsProto.has_gear)     ? (cout << "Gear" << receiverSignalsProto.gear)         : (cout << "No Gear"); cout << endl;
	(receiverSignalsProto.has_aux1)     ? (cout << "Aux1" << receiverSignalsProto.aux1)         : (cout << "No Aux1"); cout << endl;

	return 1;
}



