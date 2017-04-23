#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include<unistd.h>
// Structure to hold link status


struct _linkStatus {

	char *status;
}linkstatus;

//Structure to events occuring to change link status

struct _lmpEvents {

	char *events;
}lmpevents;

//possible events affecting link status as per RFC 4204

enum _listofEvents {evBringUp, evCCDn, evConfDone, evConfErr, evNewConfOK,
	evNewConfErr, evContenWin, evContenLost, evAdminDown,
	evNbrGoesDn, evHelloRcvd, evHoldTimer, evSeqNumErr,
	evReconfig, evConfRet, evHelloRet, evDownTimer};

//possible link status as per RFC 4204

enum _listofStates {Down, ConfSnd, ConfRcv, Active, Up, GoingDown};

int main(){
	int clientSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	enum _listofEvents event;
	enum _listofStates state;
	/*---- Create the socket. The three arguments are: ----*/
	/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;
	/* Set port number, using htons function to use proper byte order */
	serverAddr.sin_port = htons(7891);
	/* Set IP address to localhost */
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	/*---- Connect the socket to the server using the address struct ----*/
	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	/*---- Read the message from the server into the buffer ----*/
	state=2;	
	linkstatus.status="ConfRcv";
	//while(1) {

	while(recv(clientSocket, buffer, 1024, 0) < 0) {

		puts("waiting for peer to send its message");
	}
	event=0;
	printf("my event is =%d\n",event);
	printf("Message received from peer: %s",buffer);   
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"configAck");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"hello");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"NewConfig");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"hello");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"CCDown");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"CCDnFlg");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	
	strcpy(buffer,"configNack");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"reConfig");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"configAck");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));

	strcpy(buffer,"helloSeqErr");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	
	strcpy(buffer,"hello");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	
	strcpy(buffer,"helloSeqErr");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	
	strcpy(buffer,"newConfigErr");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	
	strcpy(buffer,"newConfigErr");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	
	strcpy(buffer,"newConfig");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	
	strcpy(buffer,"hello");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	
	strcpy(buffer,"reConfig");
	send(clientSocket,buffer,1024,0);
	bzero(buffer,sizeof(buffer));
	//}
	close(clientSocket);
	return 0;
}
