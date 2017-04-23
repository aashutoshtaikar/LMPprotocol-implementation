/****************** SERVER CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

// Structure to hold link status
struct _linkStatus {
	
	int status;
	char *state;
}linkstatus;

//Structure to events occuring to change link status
struct _lmpEvents {
	
	int Events;
	char* event;
}lmpevents;


//possible events affecting link status as per RFC 4204
enum _listofEvents {evBringUp, evCCDn, evConfDone, evConfErr, evNewConfOK,
		    evNewConfErr, evContenWin, evContenLost, evAdminDown,
		    evNbrGoesDn, evHelloRcvd, evHoldTimer, evSeqNumErr,
		    evReconfig, evConfRet, evHelloRet, evDownTimer};

//possible link status as per RFC 4204
enum _listofStates {Down, ConfSnd, ConfRcv, Active, Up, GoingDown};


//State to string conversion
char* stateDescription(enum _listofStates* state) {
	
	char *description;
	switch(*state) {
		case 0: 
			description="Down";
			break;
		case 1:
			description="ConfSnd";
			break;
		case 2:
			description="confRcv";
			break;
		case 3:
			description="Active";
			break;
		case 4:
			description="Up";
			break;
		case 5: 
			description="GoinDown";

			break;
		default:
			description="Uknown state generated\n";
			break;
	}	
	return description;
}

//Event to String conversion
char* eventDescription(enum _listofEvents* event) {
	
	char *description;
	switch(*event) {
		case 0: 
			description="evBringUp";
			break;
		case 1:
			description="evCCDn";
			break;
		case 2:
			description="evConfDone";
			break;
		case 3:
			description="evConfErr";
			break;
		case 4:
			description="evNewConfOK";
			break;
		case 5: 
			description="evNewConfErr";
			break;
		case 6:
			description="evContenWin";
			break;
		case 7:
			description="evContenLost";
			break;
		case 8:
			description="evAdminDown";
			break;
		case 9:
			description="evNbrGoesDn";
			break;
		case 10:
			description="evHelloRcvd";
			break;
		case 11:
			description="evHoldTimer";
			break;
		case 12:
			description="evSeqNumErr";
			break;
		case 13:
			description="evReconfig";
			break;
		case 14:
			description="evConfRet";
			break;
		case 15:
			description="eveHelloRet";
			break;
		case 16: 
			description="evDownTimer";
			break;
		default:
			description="Generated a wrong event";
			break;
	
	}
	return description;
}

//Print function to print generated event
void printevent(char* messages) {
	
	printf("event generated= %s\n",messages);
}

//Print function to print generated state
void printstate(char* messages) {
	
	printf("my state is= %s\n",messages);
}

//Fuction to update the _lmpevent struct
void eventModifier (struct _lmpEvents *lmpevents,
		enum _listofEvents *event) {

	char *messages;
	lmpevents->Events=*event;
	messages=eventDescription(event);
	lmpevents->event=messages;
	printevent(messages);
}

//Fuction to update the _states struct
void stateModifier(struct _linkStatus* linkstatus,
                   enum _listofStates* state) {
    
    char *messages;
    linkstatus->status=*state;
    messages=stateDescription(state);
    linkstatus->state=messages;
	printstate(messages);
}

//Funtion to receive message from socket
char* receiveMessage(int* newSocket, char *buffer) {
    
    int socket=*newSocket;
    while((recv(socket, buffer, 1024, 0))<=0) {
	
	}
    printf("received message =%s\n",buffer);
    return buffer;
}


//Function to handle Down state events and corresponding actions
void downStateActions (int* newSocket, char *buffer,struct _linkStatus* linkstatus,
                        struct _lmpEvents* lmpevents, enum _listofEvents *event,
                        enum _listofStates *state, int* buffer_len) {
	
	if(*state ==0) {
        bzero(buffer,*buffer_len);
        strcpy(buffer,"ConfSnd\n");
        send(*newSocket,buffer,1024,0);
        *event=0;
        eventModifier(lmpevents, event);
        *state=1;
        stateModifier(linkstatus,state);
        bzero(buffer,*buffer_len);
    }
}

//Function to handle confSnd state events and corresponding actions
void confSndStateActions(int* newSocket, char* buffer,struct _linkStatus* linkstatus,
						 struct _lmpEvents* lmpevents, enum _listofEvents *event,
					     enum _listofStates *state,int* buffer_len) {
	
	buffer=receiveMessage(newSocket,buffer);
	
	if(strcmp(buffer,"configAck") == 0) {

		*event=2;
		eventModifier(lmpevents,event);
		*state=3;
		stateModifier(linkstatus, state);
	} else if(strcmp(buffer,"configNack")==0) {

		*event=3;
		eventModifier(lmpevents,event);
		*state=1;
		stateModifier(linkstatus, state);
	}else if(strcmp(buffer,"reConfig")==0) {
		*event=13;
		eventModifier(lmpevents,event);
		*state=1;
		stateModifier(linkstatus, state);
	}else if(strcmp(buffer,"") ==0) {
		
		*state=0;
		stateModifier(linkstatus, state);
		*event=1;
		stateModifier(linkstatus, state);
	} else {

		printf("system ended with a wrong state\n");
		exit(0);
	} 
	bzero(buffer,*buffer_len);
}

//Function to handle Receive state events and corresponding actions
void confRcvStateActions(int* newSocket, char* buffer,struct _linkStatus* linkstatus,
						 struct _lmpEvents* lmpevents, enum _listofEvents *event,
					     enum _listofStates *state,int* buffer_len) {

	buffer=receiveMessage(newSocket,buffer);
	if(strcmp(buffer,"newConfigErr")==0) {
		*event=5;
		eventModifier(lmpevents, event);
		*state=2;
		stateModifier(linkstatus, state);
	} else if(strcmp(buffer,"newConfig")==0) {
		*event=4;
		eventModifier(lmpevents, event);
		*state=3;
		stateModifier(linkstatus, state);
	}
}
//Function to handle activeState events and corresponding actions
void activeStateActions(int* newSocket, char* buffer,struct _linkStatus* linkstatus,
		struct _lmpEvents* lmpevents, enum _listofEvents *event,
		enum _listofStates *state,int* buffer_len) {

	buffer=receiveMessage(newSocket,buffer);

	if(strcmp(buffer,"hello") == 0) {

		*event=10;
		eventModifier(lmpevents, event);
		*state=4;
		stateModifier(linkstatus, state);
	}
	else if(strcmp(buffer,"hello") == 0) {

		*event=10;
		eventModifier(lmpevents, event);
		*state=4;
		stateModifier(linkstatus, state);
	}else if(strcmp(buffer,"helloSeqErr") == 0) {

		*event=12;
		eventModifier(lmpevents, event);
		*state=3;
		stateModifier(linkstatus, state);
	}
	bzero(buffer,*buffer_len);

}

//Function to handle upstate events and corresponding actions
void upStateActions(int* newSocket, char* buffer,struct _linkStatus* linkstatus,
					struct _lmpEvents* lmpevents, enum _listofEvents *event,
					enum _listofStates *state, int* buffer_len) {
	
	buffer=receiveMessage(newSocket,buffer);
	if(strcmp(buffer,"hello") == 0) {
		*state=*state;
		stateModifier(linkstatus, state);
	} else if(strcmp(buffer,"helloSeqErr")==0) {
		
		*event=12;
		eventModifier(lmpevents, event);
		*state=*state;
		stateModifier(linkstatus, state);
	} else if((strcmp (buffer,"NewConfig"))== 0) {

		*event=4;
		eventModifier(lmpevents,event);
		*state=3;
		stateModifier(linkstatus, state);
	} else if((strcmp (buffer,"CCDown")) ==0) {
		*event=9;
		eventModifier(lmpevents,event);
		*state=5;
		stateModifier(linkstatus,state);
	} else if(strcmp(buffer,"reConfig")==0) {

		*event=13;
		eventModifier(lmpevents,event);
		*state=1;
		stateModifier(linkstatus,state);
	} else if(strcmp(buffer,"newConfigErr")==0) {
		
		*event=5;
		eventModifier(lmpevents,event);
		*state=2;
		stateModifier(linkstatus,state);
	}
	bzero(buffer,*buffer_len);
}

void goinDownStateActions(int* newSocket, char* buffer,struct _linkStatus* linkstatus,
						  struct _lmpEvents* lmpevents, enum _listofEvents *event,
						  enum _listofStates *state, int* buffer_len) {

	buffer=receiveMessage(newSocket,buffer);
	if(strcmp(buffer,"CCDnFlg")==0) {
		*event=8;
		eventModifier(lmpevents,event);
		*state=0;
		stateModifier(linkstatus,state);
	}
}


int main(){
	int welcomeSocket, newSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	enum _listofEvents event;
	enum _listofStates state;
	int buffer_len=sizeof(buffer);
	

	/*---- Create the socket. The three arguments are: ----*/
	/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;
	/* Set port number, using htons function to use proper byte order */
	serverAddr.sin_port = htons(7891);
	/* Set IP address to localhost */
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	/*---- Bind the address struct to the socket ----*/
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	/*---- Listen on the socket, with 5 max connection requests queued ----*/
	if(listen(welcomeSocket,5)==0)
		printf("Ready for configuration\n");
	else
		printf("Error in setting listening\n");

	/*---- Accept call creates a new socket for the incoming connection ----*/
	addr_size = sizeof serverStorage;
	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

	/*---- Send message to the socket of the incoming connection ----*/

	bzero(buffer,buffer_len);
	
	state=0;
	stateModifier(&linkstatus,&state);
	event=0;
	linkstatus.status=state;
	lmpevents.Events=event;
	while(1) {
	//switch based on status of link i.e.. down , confsnd etc..
		switch(state) {

			case 0: 
				//down state
				downStateActions(&newSocket, buffer,&linkstatus, &lmpevents,&event,
								 &state,&buffer_len);
				break;
			case 1:
				//confsnd state
				confSndStateActions(&newSocket, buffer, &linkstatus,&lmpevents,
									&event,&state,&buffer_len);
			break;
			case 2:
				//confrcv state
				confRcvStateActions(&newSocket, buffer, &linkstatus,&lmpevents,
									&event,&state,&buffer_len);
					break;
			case 3:
				//active state
				activeStateActions(&newSocket, buffer, &linkstatus,&lmpevents,
                                    &event,&state,&buffer_len);
			break;
			case 4:
			//upstate
				upStateActions(&newSocket, buffer, &linkstatus,&lmpevents,
                                    &event,&state,&buffer_len);
			break;
			case 5:
			//going down
				goinDownStateActions(&newSocket, buffer, &linkstatus,&lmpevents,
                                     &event,&state,&buffer_len);
			break;
			default:
				printf("\nended up in a wrong state\n");
		}
	}
	return 0;
}
