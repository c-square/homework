"""STS Protocol"""

import os
import json
import socket
import errno
import signal
import threading
from fractions import gcd

from Crypto.Util import number as nutil

import pysts.util as sutil
import pysts.cryptography as crypto
from pysts.constants import CONFIG, GENERAL


class STSetup(object):

    """
    Setup for STS Protocol
    """

    def __init__(self, prime=None, bites=GENERAL.PRIME_BITES, quick=False):
        """
        :param prime: prime number
        :type prime: Integer, Long or None
        :param int bites: number of bites for randome prime number generator
        :param bool quick: True for quick setup

        If :param args: is not set will be generare a prime number of
        :param bites: bites.
        """

        if not prime:
            if not isinstance(bites, int):
                raise TypeError("Parameter bites should be integer !")
            self._prime = self.get_prime(bites)

        else:
            if not sutil.primality_test(prime):
                raise ValueError("Parameter prime should be a prime number !")
            self._prime = prime

        sutil.debug("[i] Prime number is: {}".format(self._prime))

        if quick:
            self._primitive_root = self.quick_setup()
        else:
            self._primitive_root = self.setup()

    @property
    def prime(self):
        """Getter for prime number"""
        return self._prime

    @property
    def primitive_root(self):
        """Getter for primitive root"""
        return self._primitive_root

    def process_number(self):
        """Returns a posbile generator"""
        while True:
            # Get a random number
            random_number = nutil.getRandomRange(2, self._prime - 2,
                                                 os.urandom)
            # Check if it is co-prime
            if gcd(random_number, self._prime) == 1:
                break

        return random_number

    def quick_setup(self):
        """Minimum checks that provide a generator for a large subgroup."""

        index = 1
        limit = sutil.phi(self._prime)

        while True:
            random_number = self.process_number()
            while index < limit:
                root_test = pow(random_number, index, self._prime)
                if root_test == 1:
                    break
                index = index + 1
            else:
                break

        return random_number

    def setup(self):
        """
        Will choose a generator that will generates the *entire*
        multiplicative group modulo :param prime:
        """
        divisors = self.divisors(self._prime - 1)

        while True:
            root_test = 1
            random_number = self.process_number()

            sutil.debug("[i] Random number is: {}".format(random_number))
            sutil.debug("[i] Will check if number is primitive root !")

            for divisor in divisors:
                if sutil.primality_test(divisor):
                    root_test = pow(random_number, (self._prime - 1) / divisor,
                                    self._prime)
                else:
                    root_test = pow(random_number, divisor, self._prime)

                if root_test == 1:
                    sutil.debug("[x] Number is not generator !")
                    break

            if root_test != 1:
                sutil.debug("[o] Number is generator !")
                break

        return random_number

    @staticmethod
    def divisors(number):
        """Compute the prime divisors for recived number"""
        divisors = []

        while number > 1:

            if sutil.primality_test(number):
                sutil.debug("[i] Last divisor: {}".format(number))
                divisors.append(number)
                break

            if GENERAL.DIVISORS == "brent_rho":
                divisor = sutil.brent_rho(number)
            elif GENERAL.DIVISORS == "pollard_rho":
                divisor = sutil.pollard_rho(number)
            else:
                raise ValueError("Invalid value for GENERAL.DIVISORS !")

            divisors.append(divisor)
            sutil.debug("[i] Divisor found: {}".format(divisor))
            number = number / divisor

        return divisors

    @staticmethod
    def get_prime(bites):
        """Generate a new prime number"""
        prime = nutil.getPrime(bites, os.urandom)
        return prime


class STSWorker(threading.Thread):

    """
    The worker will accept new connections, will send the public key and
    after that it will process information received from client.
    """

    def __init__(self, listen_socket, public_data, rsa_key):
        """
        :param listen_socket: Gatekeeper socket
        :param server: Thread manager
        """

        threading.Thread.__init__(self)         # Call superclass constructor

        # Create a RSAKey object with RSA key recived from Gatekeeper
        self._rsa_key = crypto.PrivateKey(rsa_key)

        self._public_data = public_data     # Prime number | Generator
        self._socket = listen_socket        # Connection to clients
        self._sts = {}                      # Information regardin client
        self.online = True                  # Exit flag

    def stop(self):
        """Kill thread"""
        self._Thread__stop()

    def handshake(self, connection):
        """Exchange cryptographic keys whit user"""

        # Send public information - Public key | Prime number | Generator
        sutil.debug("[i] Send public information to client !")
        connection.sendall(json.dumps(self._public_data))

        try:
            # Recive information from client
            sutil.debug("[i] Waiting for client RSA Public Key")
            message = sutil.read_data(connection)
        except ValueError as exc:
            print("[x] {}".format(exc))
            return False

        try:
            # Will check if recived message is a RSA Public Key
            sutil.debug("[i] Check if recived message is a RSA Public Key")
            self._sts["public_key"] = crypto.PublicKey(message)
        except ValueError as exc:
            print("[x] {}".format(exc))
            return False

        return True

    def _auth_step_one(self):
        """Compute pow(genrator, random_value)"""

        # Generate a random number in (2, prime_number)
        self._sts["s_exponent"] = nutil.getRandomRange(
            2, self._public_data["prime_number"], os.urandom)

        # Compute generator ^ x_exponent
        self._sts["s_secret"] = str(pow(
            long(self._public_data["generator"]),
            long(self._sts["s_exponent"]),
            long(self._public_data["prime_number"])))

        return self._sts["s_secret"]

    def _check_step_one(self, message):
        """Will process and validate data recived from client"""

        self._sts["c_secret"], data = message.split(",", 1)
        self._sts["secret_key"] = str(pow(
            long(self._sts["c_secret"]),
            long(self._sts["s_exponent"]),
            long(self._public_data["prime_number"])))

        self._sts["c_signature"] = "".join([self._sts["c_secret"],
                                            self._sts["s_secret"]])

        #aes_cipher = crypto.AESCipher(self._sts["secret_key"])
        #signature = aes_cipher.decrypt(data)

        signature = self._sts["public_key"].verify(
            self._sts["c_signature"], json.loads(data))

        return signature

    def _auth_step_two(self):
        """Compute confirmation for client"""
        self._sts["s_signature"] = "".join([self._sts["s_secret"],
                                            self._sts["c_secret"]])

        signature = self._rsa_key.sign(
            self._sts["s_signature"], "{},{}".format(self._sts["c_secret"],
                                                     self._sts["s_secret"]))

        #aes_cipher = crypto.AESCipher(self._sts["secret_key"])
        #signature = aes_cipher.encrypt(json.dumps(signature))
        response = json.dumps(signature)
        return response

    def authentificate(self, connection):
        """Will validate client identity"""

        sutil.debug("[i] Authentification - Send exponent")
        # Compute pow(genrator, random_value) and send to client
        connection.sendall(self._auth_step_one())

        sutil.debug("[i] Authentification - Check client response")

        # Get response for step one and process it
        message = sutil.read_data(connection)
        if not self._check_step_one(message):
            return False

        # User is valid - send confirmation
        sutil.debug("[i] Authentification complete - Send confirmation")
        connection.sendall(self._auth_step_two())
        return True

    def comunicate(self, connection):
        """Comunicate with client"""
        pass

    def _request_handler(self, connection):
        """Process information from request and send response to client"""
        if not self.handshake(connection):
            print("[x] Invalid handshake !")
            return False

        if not self.authentificate(connection):
            print("[x] Unknown user !")
            return False

        sutil.debug("[o] Authentification complete.")
        self.comunicate(connection)

    def run(self):
        """Start process information received from client"""
        while self.online:
            try:
                # Waiting for new connection to handle
                connection, client_address = self._socket.accept()
                self._sts.clear()
                self._sts["address"] = client_address
                sutil.debug("[i] New client: {}".format(client_address))
            except IOError as exc:
                error_code, _ = exc.args
                if error_code == errno.EINTR:
                    # This call did not succeed because it was interrupted.
                    # However, if you try again, it will probably work.
                    continue
                sutil.debug(exc)
            else:
                self._request_handler(connection)    # Communicate with client
                connection.close()                   # Close current connection


class STServer(object):

    """pySTS Server Object"""

    def __init__(self, adress="127.0.0.1", port=61111):
        """Setup STServer Instance"""

        self._private_key = None
        self._socket = socket.socket()  # Create a new socket
        self._adress = (adress, port)
        self.data = {"public_key": None, "prime_number": None,
                     "generator": None}

        self._workers = []
        self._lock = threading.Lock()

        # Load settings from the previous session
        self._load_settings()

        # Check if STS settings are available
        if not self.data["prime_number"] or not self.data["generator"]:
            self._sts_setup()

        # Check if Private Key is available
        if not self._private_key:
            if GENERAL.DEBUG:
                print("[i] Will generate new RSA Key !")
            self._private_key = crypto.PrivateKey(crypto.rand_rsa_key())
            self.data["public_key"] = self._private_key.public_key

        # SO_REUSEADDR flag tells the kernel to reuse a local socket in
        # TIME_WAIT state, without waiting for its natural timeout to expire.
        self._socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._socket.setblocking(1)  # Set blocking mode of the socket

    def _sts_setup(self):
        """Get STS required settings"""
        if GENERAL.DEBUG:
            print("[i] Generate prime number and generator for Zp !")

        information = STSetup(quick=CONFIG.SERVER.SETUP_QUICK)
        self.data["prime_number"] = information.prime
        self.data["generator"] = information.primitive_root

    def _load_settings(self):
        """Try to load the settings from the previous session."""

        if not os.path.exists(GENERAL.CONFIG):
            try:
                os.makedirs(GENERAL.CONFIG)
            except IOError:
                raise ValueError("Invalid value provided for GENERAL.CONFIG")

            return

        if os.path.isfile(CONFIG.SERVER.RSA):
            if GENERAL.DEBUG:
                print("[i] Will load RSA Private Key !")

            try:
                private_key = crypto.PrivateKey(CONFIG.SERVER.RSA)
                self.data["public_key"] = private_key.public_key
                self._private_key = private_key
            except ValueError:
                print("[x] Fail to load RSA Private Key !")

        if os.path.isfile(CONFIG.SERVER.STS):
            if GENERAL.DEBUG:
                print("[i] Will load STS required information !")
            try:
                file_content = []
                file_handler = open(CONFIG.SERVER.STS, "rb")
                while True:
                    chunck = file_handler.read(1024)
                    if not chunck:
                        break
                    file_content.append(chunck)
                file_handler.close()
            except IOError:
                print("[x] Fail to load STS information !")

            if len(file_content):
                try:
                    sts_settings = json.load("".join(file_content))
                    self.data["prime_number"] = sts_settings.get(
                        "prime_number", None)
                    self.data["generator"] = sts_settings.get(
                        "generator", None)
                except ValueError:
                    print("[x] Fail to load STS information !")

    def _dump_settings(self):
        """Save current settings"""

    def exit(self, signal_value, _):
        """Shutdown server"""
        print("Shutdown server, recived signal {} !".format(signal_value))

        for worker in self._workers:
            if worker.is_alive():
                worker.online = False   # Set flag for kill worker
                worker.stop()           # Kill worker
                worker.join()           # Wait for finish jobs

    def run(self):
        """Start TCP Server"""
        try:
            self._socket.bind(self._adress)     # Bind
            self._socket.listen(5)              # Listen
        except socket.error as exc:
            print(exc)
            return

        # Start workers
        for _ in range(CONFIG.SERVER.WORKERS):
            worker = STSWorker(
                self._socket, dict(self.data), self._private_key.private_key)
            worker.start()
            self._workers.append(worker)

        # Set handler for kill signal
        signal.signal(signal.SIGTERM, self.exit)
        # Set handler for KeyboardInterrupt (Ctrl+C) | -2
        signal.signal(signal.SIGINT, self.exit)
        signal.pause()  # Wait for one signal


class STSClient(object):

    """pySTS Server Object"""

    def __init__(self, host="127.0.0.1", port=61111):
        """Setup STSClient Isinstance"""
        self._private_key = None
        self.data = {"public_key": None, "prime_number": None,
                     "generator": None}
        self._address = (host, port)
        self._sts = {}

        # Check if Private Key is available
        if not self._private_key:
            if GENERAL.DEBUG:
                print("[i] Will generate new RSA Key !")
            self._private_key = crypto.PrivateKey(crypto.rand_rsa_key())

        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.settimeout(10)

    def _load_settings(self):
        """Try to load the settings from the previous session."""

        if not os.path.exists(GENERAL.CONFIG):
            try:
                os.makedirs(GENERAL.CONFIG)
            except IOError:
                raise ValueError("Invalid value provided for GENERAL.CONFIG")

            return

        if os.path.isfile(CONFIG.CLIENT.RSA):
            if GENERAL.DEBUG:
                print("[i] Will load RSA Private Key !")

            try:
                private_key = crypto.PrivateKey(CONFIG.SERVER.RSA)
                self._private_key = private_key
            except ValueError:
                print("[x] Fail to load RSA Private Key !")

    def handshake(self):
        """Exchange cryptographic keys whit user"""

        try:
            # Recive information from server
            sutil.debug("[i] Waiting for Public Information")
            message = sutil.read_data(self._socket)
            message = json.loads(message)
            for key in self.data:
                if not key in message:
                    raise ValueError("Missing {} field !".format(key))
                self.data[key] = message[key]
        except ValueError as exc:
            print("[x] {}".format(exc))
            return False

        except TypeError:
            print("[x] Invalid information recived !")
            return False

        except Exception as exc:
            print("[x] {}".format(exc))
            return False

        # Send Public key
        sutil.debug("[i] Send public key to server !")
        self._socket.sendall(self._private_key.public_key)

        try:
            # Will check if recived message is a RSA Public Key
            sutil.debug("[i] Check if recived message is a RSA Public Key")
            self.data["public_key"] = crypto.PublicKey(self.data["public_key"])
        except ValueError as exc:
            print("[x] {}".format(exc))
            return False

        return True

    def _request_handler(self):
        """Process information from request and send response to server"""
        if not self.handshake():
            print("[x] Invalid handshake !")
            return False

        if not self.authentificate():
            print("[x] Unknown Server !")
            return False

        self.comunicate()

    def _auth_step_one(self):
        """Compute pow(genrator, random_value)"""

        # Generate a random number in (2, prime_number)
        self._sts["c_exponent"] = nutil.getRandomRange(
            2, self.data["prime_number"], os.urandom)

        # Compute generator ^ x_exponent
        self._sts["c_secret"] = str(pow(
            long(self.data["generator"]), long(self._sts["c_exponent"]),
            long(self.data["prime_number"])))

    def _auth_step_two(self):
        """Compute STS key"""

        self._sts["secret_key"] = str(pow(long(self._sts["s_secret"]),
                                      long(self._sts["c_exponent"]),
                                      long(self.data["prime_number"])))

        self._sts["c_signature"] = "".join([self._sts["c_secret"],
                                            self._sts["s_secret"]])

        signature = self._private_key.sign(
            self._sts["c_signature"], "{},{}".format(self._sts["c_secret"],
                                                     self._sts["s_secret"]))

        #aes_cipher = crypto.AESCipher(self._sts["secret_key"])
        #signature = aes_cipher.encrypt(str(signature[0]))

        response = "{},{}".format(self._sts["c_secret"],
                                  json.dumps(signature))
        return response

    def _check_auth(self, message):
        """Check server identity"""
        self._sts["s_signature"] = "".join([self._sts["s_secret"],
                                            self._sts["c_secret"]])

        #aes_cipher = crypto.AESCipher(self._sts["secret_key"])
        #signature = aes_cipher.decrypt(data)

        signature = self.data["public_key"].verify(
            self._sts["s_signature"], json.loads(message))

        return signature

    def authentificate(self):
        """Will validate server identity"""
        # Compute the secret
        self._auth_step_one()

        # Get exponent from server
        sutil.debug("[i] Get Server Secret !")
        self._sts["s_secret"] = sutil.read_data(self._socket)

        # Send authentification data
        sutil.debug("[i] Send authentification step two information !")
        self._socket.sendall(self._auth_step_two())

        # Wait for server response
        sutil.debug("[i] Waiting for server response !")
        message = sutil.read_data(self._socket)

        if not self._check_auth(message):
            return False

        return True

    def comunicate(self):
        """Communicate with server"""

    def run(self):
        """Start client"""
        while True:
            try:
                sutil.debug("[i] Try to connect to: {}".format(self._address))
                self._socket.connect(self._address)
            except IOError as exc:
                error_code, _ = exc.args
                if error_code == errno.EINTR:
                    # This call did not succeed because it was interrupted.
                    # However, if you try again, it will probably work.
                    continue
                sutil.debug(exc)
            else:
                sutil.debug("[o] Successfully connected !")
                self._request_handler()         # Communicate with client
                # self._socket.close()          # Close current connection

            break
