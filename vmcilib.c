/* *************************************************************************************************
 * File Name	: vmcilib.c
 * Purpose		: Implementation of the VMCI dynamic library.
 * Author		: Cory Levy
 * Date Created	: 26/03/2017
 * ************************************************************************************************/
/******************************************** INCLUDES ********************************************/

#include "vmcilib.h"

/********************************************* MACROS *********************************************/

#define AF_VMCI_DEFAULT (-1)

/******************************************** GLOBALS *********************************************/

static int AF_VMCI = AF_VMCI_DEFAULT;

/**************************************** IMPLEMENTATIONS *****************************************/

_export 
int vmci_errno()
{
	return errno;
}

_export 
int vmci_get_local_cid()
{
	return VMCISock_GetLocalCID();
}

_export 
socket_t vmci_socket(int type, int protocol)
{
	return (socket_t)socket(AF_VMCI, type, protocol);
}

_export 
int vmci_bind(socket_t sockfd, port_t port)
{
	sockaddr_vm_t vm_addr = {0};

	/* Create remote vm sockaddr struct */
	vm_addr.svm_family = AF_VMCI;
	vm_addr.svm_port = port;
	vm_addr.svm_cid = DEFAULT_CID;

	/* Bind socked to context ID */
	return bind(sockfd, (struct sockaddr *)&vm_addr, sizeof(vm_addr));
}

_export 
int vmci_connect(socket_t sockfd, int cid, port_t port)
{
	sockaddr_vm_t vm_addr = {0};

	/* Create remote vm sockaddr struct */
	vm_addr.svm_family = AF_VMCI;
	vm_addr.svm_port = port;
	vm_addr.svm_cid = cid;

	return connect(sockfd, (struct sockaddr*)&vm_addr, (socklen_t)sizeof(vm_addr));
}

_export 
int vmci_listen(socket_t sockfd, int backlog)
{
	return listen(sockfd, backlog);
}

_export
socket_t vmci_accept(socket_t sockfd, int *cid, port_t *port)
{
	socket_t return_value = -1;
	sockaddr_vm_t vm_addr = {0};
	socklen_t addr_len = sizeof(vm_addr);
	
	return_value = accept(sockfd, (struct sockaddr*)&vm_addr, &addr_len);

	*cid = vm_addr.svm_cid;
	*port = vm_addr.svm_port;

	return return_value;
}

_export 
int vmci_send(socket_t sockfd, char *buffer, size_t size, int flags)
{
	return send(sockfd, buffer, size, flags);
}

_export 
int vmci_recv(socket_t sockfd, char *buffer, size_t size, int flags)
{
	return recv(sockfd, buffer, size, flags);
}

_export 
int vmci_recvfrom(socket_t sockfd, char* buffer, size_t size, int flags, int* cid, port_t* port)
{
	sockaddr_vm_t destaddr = {0};
	int received_bytes = 0;
	size_t address_size = sizeof(destaddr);

	received_bytes = recvfrom(sockfd, buffer, size, flags, (struct sockaddr*)&destaddr, &address_size);

	return received_bytes;
}

_export
int vmci_shutdown(socket_t sockfd, int how)
{
	return shutdown(sockfd, how);
}

_export 
int vmci_close(socket_t sockfd)
{
	#ifdef _WIN32
		return closeSocket(sockfd);
	#else
		return close(sockfd);
	#endif
}

/********************************************** MAIN **********************************************/
/* *************************************************************************************************
 * Name		: constructor
 * Purpose	: Initializes the AF_VMCI variable
 * Arguments: 
 * 				OUT: int *fd - The file descriptor of the VMCI network card (Passed automatically)
 * Remarks 	: Called when loading a shared library
 * ************************************************************************************************/
__attribute__((constructor)) 
static void construcor(void)
{
	AF_VMCI = VMCISock_GetAFValue();
}

/* *************************************************************************************************
 * Name		: destructor
 * Purpose	: deinitializes the AF_VMCI variable
 * Remarks	: Called when shared library is freed
 * TODO: Check if removing it doesn't break anything
 * ************************************************************************************************
__attribute__((destructor)) 
static void destructor(void) 
{
	AF_VMCI = AF_VMCI_DEFAULT;
}
*/

#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE hinsDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(DLL_PROCESS_ATTACH == fdwReason)
		constructor();
	else if(DLL_PROCESS_DETACH == fdwReason)
		destructor();

	return TRUE;
}
#endif /* _WIN32 */

