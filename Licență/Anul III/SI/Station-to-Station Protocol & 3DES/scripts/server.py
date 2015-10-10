"""pySTS - Server Application"""
import select
from hashlib import sha256

import pysts.util as sutil
import pysts.cryptography as crypto
from pysts.protocol import STSClient

from pysts.protocol import STServer
from pysts.protocol import STSWorker


def comunicate(worker, connection):
    """Comunicate with client"""

    secret_key = worker._sts["secret_key"]
    des3_key = sha256(secret_key).digest()[:16]

    des3_cipher = crypto.DES3Cipher(des3_key)

    while True:
        socket_list = [connection]

        # Get the list sockets which are readable
        read_sockets, _, _ = select.select(socket_list, [], [])

        for sock in read_sockets:

            # incoming message from remote server
            if sock == connection:
                try:
                    message = sutil.read_data(connection)
                except ValueError as msg:
                    print("[x] {}".format(msg))

                message = des3_cipher.decrypt(message)
                print("<<< {}".format(message))
                message = des3_cipher.encrypt(message)
                print(">>> {}".format(message))
                connection.sendall(message)

STSWorker.comunicate = comunicate


def start_server():
    """Start STS Server"""
    server = STServer()
    server.run()

if __name__ == "__main__":
    start_server()
