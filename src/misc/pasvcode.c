// Using PASV Mode
		// TODO: implement
		
		// send PASV command
		status = STATUS_SENDPASV;
		debugLine("PASV");
		sendclientResponseLn("PASV");
		
		// wait for return/failure of pasv command
		int ret = ftpTimeoutIS(STATUS_SENDPASV, ERRTIMEOUT);
		if (ret < 0) {
			// timed out
			return -1;
		}
		
		if ( status == STATUS_ENTERPASV ) {
			// PASV info (ip/port) was received and parsed
		
			// ******* Connect
		
			// Create Connect Socket
			struct sockaddr_in addrConnect;
			memset(&addrConnect, 0, sizeof(struct sockaddr_in));

			SOCKET sockConnect = sceNetInetSocket(AF_INET, SOCK_STREAM, 0);
			if (sockConnect & 0x80000000) {
				debugLine("[ERROR] Unable to create connect socket");
				return -1;
			}
		
			// Setup Connection Info
			addrConnect.sin_family = AF_INET;
			
			// get the port to use (portA (static number) * 256 + portB (incrementing number)
			con->dataPort = (con->data_portA * 256) + con->data_portB;
			
			// set port to use
			addrConnect.sin_port = htons(con->dataPort);
			
			// ip info not needed
			addrConnect.sin_addr[0] = 0;
			addrConnect.sin_addr[1] = 0;
			addrConnect.sin_addr[2] = 0;
			addrConnect.sin_addr[3] = 0;
	
			// Bind the Connect Socket
			err = sceNetInetBind(sockConnect, &addrConnect, sizeof(addrConnect));
			if (err != 0) {
				debugLine("[ERROR] Unable to bind to connect socket");
				return -1;
			}
		
			// Save the Connect Socket for Later
			con->dataSocket = sockConnect;
		
		
			// Finished, Return to IDLE State
			status = STATUS_IDLE;
		}