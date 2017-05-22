/* *************************************************************************************************
 * File Name	: vimcilib.h
 * Purpose		: Header file of the VMCI dynamic library.
 * Author		: Cory Levy
 * Date Created : 26/03/2017
 * ************************************************************************************************/

#ifndef __VIMCILIB_H__
#define __VIMCILIB_H__

/******************************************** INCLUDES ********************************************/

#include <errno.h>
#include "vmci_sockets.h"

/********************************************* MACROS *********************************************/

/* Configure includes and macros based on os type */
#ifdef _WIN32
	#include <windows.h>
	#define _export __declspec(dllexport)
	#define __attribute__(x)
#else
	#include <sys/socket.h>
	#define _export
#endif

#ifdef __DEBUG__
	#define _debug(x) printf(x)
#else
	#define _debug(x)
#endif

#define SOCKET_ERROR (-1)
#define DEFAULT_CID (-1)

/******************************************** GLOBALS *********************************************/
/********************************************* TYPES **********************************************/

typedef struct sockaddr_vm sockaddr_vm_t;
typedef unsigned char byte;
typedef int socket_t;
typedef unsigned int port_t;

/****************************************** DECLARATIONS ******************************************/
/* *************************************************************************************************
 * Name		: vmci_get_local_cid
 * Purpose	: Returns the local context id of the machine.
 * Returns 	: Context id.
 * ************************************************************************************************/
_export 
int vmci_get_local_cid();

/* *************************************************************************************************
 * Name		: vmci_errno()
 * Purpose	: Return the current value of errno, for error handling.
 * Returns 	: The errno variable
 * ************************************************************************************************/
_export 
int vmci_errno();

/* *************************************************************************************************
 * Name		: vmci_socket
 * Purpose	: Creates a new VMCI socket.
 * Arguments:
 * 				IN: int type - The type of the socket. One of the constants SOCK_STREAM or
 * 					SOCK_DGRAM.
 * 				IN: int protocol - The protocol of the socket. TODO: elaborate
 * Returns 	: The file descriptor of the newly created socket, or -1 if failed.
 * ************************************************************************************************/
_export 
socket_t vmci_socket(int type, int protocol);

/* *************************************************************************************************
 * Name		: vmci_bind
 * Purpose	: Binds the VMCI socket to a specific port.
 * Arguments: 
 * 				IN: socket_t sockfd - The socket file descriptor.
 * 				IN: unsigned int port - The port that the socket will be bound to.
 * Returns 	: 0 if success, -1 if not.
 * ************************************************************************************************/
_export 
int vmci_bind(socket_t sockfd, port_t port);

/* *************************************************************************************************
 * Name		: vmci_getname
 * Purpose	:
 * Arguments:
 * 				IN: socket_t sockfd - The socket's file descriptor.
 * 				OUT: int *cid - The context ID of the socket.
 * 				OUT: unsigned int *port - The socket's port.
 * Returns 	:
 * ************************************************************************************************/
_export 
int vmci_getname(socket_t sockdf, int* cid, port_t *port);

/* *************************************************************************************************
 * Name		: vmci_getpeername
 * Purpose	:
 * Arguments:
 * 				IN: socket_t sockfd - The socket's file descriptor.
 * 				IN: int cid - The context ID.
 * 				OUT: unsigned int *port - A placeholder for the port.
 * Returns 	:
 * ************************************************************************************************/
_export 
int vmci_getpeername(int, int, unsigned int*);

/* *************************************************************************************************
 * Name		: vmci_connect
 * Purpose	: Connect the socket to another socket.
 * Arguments:
 * 				IN: socket_t sockfd - The socket's file descriptor.
 * 				IN: int cid - The context ID of the target socket.
 * 				IN: int port - The port to connect through.
 * Returns 	: 0 if success, -1 otherwise.
 * ************************************************************************************************/
_export 
int vmci_connect(socket_t sockfd, int cid, port_t port);

/* *************************************************************************************************
 * Name		: vmci_listen
 * Purpose	: Listen for incomming connections.
 * Arguments: 
 * 				IN: socket_t sockfd - The socket's file descriptor
 * 				IN: int backlog - The maximum number of incomming connection the socket can listen 
 * 								  to before refusing the connection.
 * Returns 	:
 * ************************************************************************************************/
_export 
int vmci_listen(socket_t sockfd, int backlog);

/* *************************************************************************************************
 * Name		: vmci_accept
 * Purpose	: Accept incoming connection.
 * Arguments:
 * 				IN: socket_t sockfd - The accepting socket.
 * 				OUT: int *cid - The connecting machine's cid.
 *				OUT: port_t *port - The port the connection is using.
 * Returns 	: socket_t - The new socket file descriptor.
 * ************************************************************************************************/
_export 
socket_t vmci_accept(socket_t sockfd, int *cid, port_t *port);

/* *************************************************************************************************
 * Name		: vmci_send
 * Purpose	:
 * Arguments:
 * Returns 	:
 * ************************************************************************************************/
_export 
int vmci_send(socket_t, char*, size_t, int);

/* *************************************************************************************************
 * Name		: vmci_sendto
 * Purpose	:
 * Arguments:
 * Returns 	:
 * ************************************************************************************************/
_export 
int vmci_sendto(socket_t sockfd, int, int, char*, int, int);

/* *************************************************************************************************
 * Name		: vmci_recv
 * Purpose	: Receive data from other sockets.
 * Arguments: 
 * 				IN: socket_t sockfd - The recieving socket file descriptor
 * 				OUT: char *buffer - A buffer for the received data
 * 				IN: size_t buflen - The buffer's size
 * 				IN: int flags - Flags for how to receive the data	
 * Returns 	: 0 on success, -1 otherwise
 * ************************************************************************************************/
_export 
int vmci_recv(socket_t, char*, size_t, int); 

/* *************************************************************************************************
 * Name		: vmci_recvfrom
 * Purpose	: Recieve data from a specific sender
 * Arguments: 
 * 				IN: socket_t sockfd - The recieving socket
 * 				OUT: char *buffer - The buffer of the received data
 * 				IN: size_t size - The size of the buffer
 * 				IN: int flags - Settings for how the file should be received
 * 				IN: int *cid - 
 * Returns 	:
 * ************************************************************************************************/
_export 
int vmci_recvfrom(socket_t sockfd, char* buffer, size_t size, int flags, int* cid, port_t* port);

/* *************************************************************************************************
 * Name		: vmci_shutdown
 * Purpose	: Shut down the connection to the socket
 * Arguments:
 * 				IN: socket_t sockfd - The shutting-down socket.
 * 				IN: int how - The flag to the termine how to shut down the connection.
 * 							  
 * 							  SHUT_RD - Shutdown read
 * 							  SHUT_WR - Shutdown write
 * 							  SHUT_RDWR - Shutdown read and write
 * Returns 	: 0 if success, -1 otherwise
 * ************************************************************************************************/
_export 
int vmci_shutdown(socket_t, int);

/* *************************************************************************************************
 * Name		: vmci_close
 * Purpose	: Close the socket.
 * Arguments:
 * 				IN: socket_t sockfd - The file descriptor of the socket that'll be closed.
 * Returns 	: 0 if success, -1 otherwise
 * ************************************************************************************************/
_export 
int vmci_close(socket_t);

#endif /* __VIMCILIB_H__ */
