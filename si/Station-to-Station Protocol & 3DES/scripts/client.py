"""pySTS - Client Application"""
import sys
import select
from hashlib import sha256

import pysts.util as sutil
import pysts.cryptography as crypto
from pysts.protocol import STSClient


def prompt():
    """Flush console"""
    sys.stdout.write('<say> ')
    sys.stdout.flush()


def comunicate(client):
    """Comunicate with client"""
    secret_key = client._sts["secret_key"]
    des3_key = sha256(secret_key).digest()[:16]

    des3_cipher = crypto.DES3Cipher(des3_key)

    prompt()
    while True:
        socket_list = [sys.stdin, client._socket]

        # Get the list sockets which are readable
        read_sockets, _, _ = select.select(socket_list, [], [])

        for sock in read_sockets:

            # incoming message from remote server
            if sock == client._socket:
                try:
                    message = sutil.read_data(client._socket)
                except ValueError as msg:
                    print("[x] {}".format(msg))
                    break
                print(">>> {}".format(des3_cipher.decrypt(message)))
                prompt()

            else:
                message = sys.stdin.readline()
                client._socket.sendall(des3_cipher.encrypt(message))

STSClient.comunicate = comunicate


def main():
    """Start STS Client"""
    client = STSClient()
    client.run()

if __name__ == "__main__":
    main()
