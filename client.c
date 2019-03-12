#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
// Structure to hold link status

struct _linkStatus
{

	char *status;
} linkstatus;

//Structure to events occuring to change link status

struct _lmpEvents
{
	char *events;
} lmpevents;

//possible events affecting link status as per RFC 4204

enum _listofEvents
{
	evBringUp,
	evCCDn,
	evConfDone,
	evConfErr,
	evNewConfOK,
	evNewConfErr,
	evContenWin,
	evContenLost,
	evAdminDown,
	evNbrGoesDn,
	evHelloRcvd,
	evHoldTimer,
	evSeqNumErr,
	evReconfig,
	evConfRet,
	evHelloRet,
	evDownTimer
};

//possible link status as per RFC 4204

enum _listofStates
{
	Down,
	ConfSnd,
	ConfRcv,
	Active,
	Up,
	GoingDown
};
char buffer[1024];
int clientSocket;

void send_toserver(const char *message, int size, int socket)
{
	strcpy(buffer, message);
	send(socket, buffer, 1024, 0);
	bzero(buffer, sizeof(buffer));
}

int test_commands()
{
	int input;
	while (input != 10)
	{
		printf("Please select from the following choice\n 1. ConfigAck 2.Config  \n");
		scanf("%d", &input);

		switch (input)
		{
		case 1:
			send_toserver("configAck", 1024, clientSocket);
			break;

		case 2:
			send_toserver("configNack", 1024, clientSocket);
			break;

		case 3:
			send_toserver("NewConfig", 1024, clientSocket);
			break;

		case 4:
			send_toserver("hello", 1024, clientSocket);
			break;

		case 5:
			send_toserver("CCDown", 1024, clientSocket);
			break;

		case 6:
			send_toserver("CCDnFlg", 1024, clientSocket);
			break;

		case 7:
			send_toserver("reConfig", 1024, clientSocket);
			break;

		case 8:
			send_toserver("helloSeqErr", 1024, clientSocket);
			break;

		case 9:
			send_toserver("newConfigErr", 1024, clientSocket);
			break;

		case 10:
			return 0;
		default:
			break;
		}
	}
}

int main()
{
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
	connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
	/*---- Read the message from the server into the buffer ----*/
	state = 2;
	linkstatus.status = "ConfRcv";

	while (recv(clientSocket, buffer, 1024, 0) < 0)
	{
		puts("waiting for peer to send its message");
		sleep(2);
	}

		event = 0;
		printf("\nmy event is =%d\n", event);
		printf("Message received from peer: %s", buffer);
		bzero(buffer, sizeof(buffer));

	while (test_commands()) {}


	close(clientSocket);
	return 0;
}
