import select
import sys
import os
import socket
from threading import Thread
import subprocess
import time
import pickle

import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.Util.number as CUN
from time import sleep


port = 15345
maximumConnections = 20
maxBuffSize = 100000

clients = []
ids = {}
currentID = 0
coinID = 0


def genID():
	global currentID

	currentID = currentID + 1
	return currentID

def getCoinID():
	global coinID	

	coinID = coinID + 1
	return coinID


def Help():
	print "==== RIBODC Bank server v1.0 ===="
	print "= This is the bank server."
	print "= It will keep a record of all"
	print "= the money our clients have and"
	print "= what they do with it"
	print "---------------------------------"
	print ""
	print "Waiting for connections..."


def talkToClient (connection, address):
	global maxBuffSize

	print " -> Client connected: "+str(address)
	print " --> Waiting for command.."

	clientTempID = len(clients)-1

	clients.append({})
	clients[clientTempID][str(address)] = {}
	clients[clientTempID][str(address)]["Status"] = "Connected"

	while True:
		buff = connection.recv(maxBuffSize)
		if (len(buff)>0):
			print buff
			if (buff.strip() == "1"):
				print "generating id"
				ID = genID()
				connection.send("1")
				print "sent response"
				connection.send(str(ID))
				print "sent ID"
				connection.send( pickle.dumps(BankKeys.publickey()) )
				print "sent public key"

				ids[ID] = {}				
				ids[ID]["clientPubKey"] = pickle.loads(connection.recv(maxBuffSize))
				ids[ID]["money"] = 100   
				ids[ID]["coins"] = {}

				print "account created"
			
			elif (buff.strip() == "2"):
				connection.send("2")

				
				while True:
					coinHash = pickle.loads(clientSocket.recv(maxBuffSize))
					signedCoin = pickle.loads(clientSocket.recv(maxBuffSize))				  	

					if (ids[ID]["clientPubKey"].verify(coinHash, signedCoin)):
						if (ids[ID]["money"]):
							connection.send("ok")
							ids[ID]["coins"][coinHash] = signedCoin	
							ids[ID]["money"] -= 1

						else:
							connection.send("nomoney")
							break
					else:
						connection.send("coinNotOk")
						break
				
				
			
		
	
BankKeys = RSA.generate(1024,os.urandom)


#thread.start_new_thread(za_commands, ([], ))
serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serverSocket.bind(('localhost', port))
serverSocket.listen(maximumConnections)


while True:
	connection, address = serverSocket.accept()
	thread = Thread(target = talkToClient, args = (connection, address))
	thread.start()

