#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#include "socket.h"  
#include "message.h"
#include "controller.h"

#define MAXFD(x,y) ((x) >= (y)) ? (x) : (y)

int main(int argc, char *argv[]){
	int port;
	struct cignal cig;
	// A buffer to store a serialized message
	char *cig_serialized = malloc(sizeof(char)*CIGLEN);
	// An array to registered sensor devices
	int device_record[MAXDEV] = {0};
	
	if(argc == 2){
		port = strtol(argv[1], NULL, 0);
	} else{
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(1);
	}

	int gatewayfd = set_up_server_socket(port);
	printf("\nThe Cignal Gateway is now started on port: %d\n\n", port);
	int peerfd;
	


	/* TODO: Implement the body of the server.  
	 *
	 * Use select so that the server process never blocks on any call except
	 * select. If no sensors connect and/or send messsages in a timespan of
	 * 5 seconds then select will return and print the message "Waiting for
	 * Sensors update..." and go back to waiting for an event.
	 * 
	 * The server will handle connections from devices, will read a message from
	 * a sensor, process the message (using process_message), write back
	 * a response message to the sensor client, and close the connection.
	 * After reading a message, your program must print the "RAW MESSAGE"
	 * message below, which shows the serialized message received from the *
	 * client.
	 * 
	 *  Print statements you must use:
     * 	printf("Waiting for Sensors update...\n");
	 * 	printf("RAW MESSAGE: %s\n", YOUR_VARIABLE);
	 */

	// TODO implement select loop

	// Suppress unused variable warning.  The next 5 ilnes can be removed 
	// after the variables are used.


	fd_set temp_fd;
	fd_set all_set;
	FD_ZERO(&all_set);
	FD_SET(gatewayfd, &all_set);
	int max_fd;
	max_fd = gatewayfd;
	while(1) {
		temp_fd = all_set;
		struct timeval timeout;
		timeout.tv_sec = 5;
	    timeout.tv_usec = 0;
		int nready = select(max_fd + 1, &temp_fd, NULL, NULL, &timeout);
		if (nready == -1) {
			perror("select");
	        exit(1);
		}
		else if(nready == 0){
			printf("Waiting for Sensors update...\n");
		}
		else{
			// add new connected device
			if(FD_ISSET(gatewayfd, &temp_fd) ){
				peerfd = accept_connection(gatewayfd);
				FD_SET(peerfd, &temp_fd);
				max_fd = MAXFD(max_fd, peerfd);

			}

			char* msg = malloc(sizeof(char)*CIGLEN);
			// this is to read from all existing file descriptors
			for(int i = 0; i < max_fd +1; i++) {
				if(FD_ISSET(i, &temp_fd)) {
					int r = read(i, msg, CIGLEN);
					if (r > 0) {
						//unpack and serialize cig
						unpack_cignal(msg, &cig);
						cig_serialized = serialize_cignal(cig);
						printf("RAW MESSAGE: %s\n", cig_serialized);
						//process message
						int dev_id = process_message(&cig, device_record);
						cig.hdr.device_id = dev_id;
						cig_serialized = serialize_cignal(cig);
						//write to connection
						if (write(i, cig_serialized, CIGLEN) == -1){
							perror("write");
							exit(1);
						} //close connection
						if (close(i) == -1){
							perror("close");
						} 
				}else {
				perror("read");
				exit(1);
				}
			}
		}

		}
		return 0;
	}
}
