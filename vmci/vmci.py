# ==================================================================================================================== #
# File Name     : vmci.py
# Purpose       : VMCI dynamic library wrapper.
# Date Created  : 26/03/2017
# Author        : Cory Levy
# ==================================================================================================================== #
# ===================================================== IMPORTS ====================================================== #

import os
import ctypes
from ctypes import byref
import socket as _socket

# ==================================================== EXCEPTIONS ==================================================== #


class SocketError(_socket.error):
    """
    """
    pass


class SocketCreationError(SocketError):
    """
    """
    pass

# ==================================================== CONSTANTS ===================================================== #


SOCK_STREAM = _socket.SOCK_STREAM
SOCK_DGRAM = _socket.SOCK_DGRAM
DEFAULT_PROTOCOL = 0
ALL_PORTS = -1
SOCKET_ERROR_CODE = -1

# === PRIVATE === #
_WINDOWS = "nt"
_LOCAL_DIRECTORY = "."
_VMCILIBDLL = os.path.join(_LOCAL_DIRECTORY, "vmcilib.dll")
_VMCILIBSO = os.path.join(_LOCAL_DIRECTORY, "libvmci.so")

# ================================================= IMPORT TIME CODE ================================================= #

# If this code runs on a Windows machine, load the VMCI dll. Else, we're probably running on a Linux machine,
# load the shared object
if os.name == _WINDOWS:
    _vmcilib = ctypes.windll.LoadLibrary(_VMCILIBDLL)
else:
    _vmcilib = ctypes.cdll.LoadLibrary(_VMCILIBSO)

# ===================================================== GLOBALS ====================================================== #
# ==================================================== FUNCTIONS ===================================================== #


def get_local_cid():
    return _vmcilib.vmci_get_local_cid()

# ===================================================== CLASSES ====================================================== #


class socket(object):
    """
    A vmci socket object
    """
    def __init__(self, socktype, protocol=DEFAULT_PROTOCOL, fd=None):
        """
        @summary: Initialize a vmci socket object.
        @param socktype: The connection type of the socket. Use the constant SOCK_STREAM
                         for TCP-like communication and SOCK_DGRAM for UDP-like communication.
        @type socktype: integer
        @param protocol: Reserved.
        @type protocol: integer
        @param fd: A socket file descriptor, intended for internal usage only.
        @type fd: integer
        """
        self.type = socktype
        self.protocol = protocol

        if fd is None:
            self._fd = _vmcilib.vmci_socket(self.type, self.protocol)
        else:
            self._fd = fd

        if self._fd == SOCKET_ERROR_CODE:
            raise SocketCreationError(self.error)

    def __del__(self):
        """
        @summary: Destructor.
        """
        self.close()

    @property
    def error(self):
        """
        @summary: Return the error message of the last operation.
        @return: The error message.
        @rtype: string
        """
        return os.strerror(_vmcilib.vmci_errno())

    @classmethod
    def fromfd(cls, socktype, protocol, fd):
        """
        """
        return cls(socktype, protocol, fd)

    def close(self):
        """
        @summary: Closes the _socket
        """
        status_code = _vmcilib.close(self._fd)
        if status_code == SOCKET_ERROR_CODE:
            raise SocketError(self.error)

    def fileno(self):
        """
        @summary: Get _socket file descriptor.
        @return: Socket file descriptor.
        @rtype: int
        """
        return self._fd

    def bind(self, port=ALL_PORTS):
        """
        """
        status_code = _vmcilib.vmci_bind(self._fd, port)
        if status_code == SOCKET_ERROR_CODE:
            raise SocketError(self.error)

    def connect(self, address, flags=0):
        """
        """
        # Unpack address
        cid, port = address

        # Connect to machine
        status_code = _vmcilib.vmci_connect(self._fd, cid, port, flags)

        if status_code == SOCKET_ERROR_CODE:
            raise SocketError(self.error)

    def listen(self, backlog):
        """
        """
        status_code = _vmcilib.vmci_listen(backlog)
        if status_code == SOCKET_ERROR_CODE:
            raise SocketError(self.error)

    def accept(self):
        """
        """
        cid = ctypes.c_int()
        port = ctypes.c_int()

        sockfd = _vmcilib.vmci_accept(self._fd, byref(cid), byref(port))
        if sockfd == SOCKET_ERROR_CODE:
            raise SocketError(self.error)

        return self.fromfd(self.type, self.protocol, sockfd), (cid.value, port.value)

    def send(self, string, flags=0):
        """
        """
        buffered_data = ctypes.c_buffer(string)
        status_code = _vmcilib.vmci_send(self._fd, buffered_data, len(buffered_data), flags)
        if status_code == SOCKET_ERROR_CODE:
            raise SocketError()

    def recv(self, bufsize, flags=0):
        """
        """
        received_data = ctypes.c_buffer(bufsize)
        status_code = _vmcilib.vmci_recv(self._fd, received_data, bufsize, 0)
        if status_code == SOCKET_ERROR_CODE:
            raise SocketError(self.error)

        return received_data.value

    def sendto(self, string, address):
        """
        """
        raise NotImplementedError
        cid, port = address

    def recvfrom():
        """
        """
        raise NotImplementedError

    def shutdown(self, how):
        """
        """
        status_code = _vmcilib.shutdown(self._fd, how)
        if status_code == SOCKET_ERROR_CODE:
            raise SocketError(self.error)
