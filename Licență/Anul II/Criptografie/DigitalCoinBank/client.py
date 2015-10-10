import socket
import sys
import select
import ctypes
import random
import win32gui
import subprocess
from threading import Thread
from time import sleep
import os
import pickle

import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.Util.number as CUN



port = 15345
maxBuffSize = 100000
ID = ""

commandIndex = 0
sentCommandIndex = 0

def Help():
	print "Hello"
	print "Welcome to the Romanian International Bank Of Digital Coin client"
	print "...RIBODC for short..."
	print " -> You can use this client to create a DigitalCoin bank account"
	print " -> and manage it"
	BaseOptions()

def BaseOptions():
	print "You have the following options:"
	print "1. Create new account"
	print "2. Withdraw Money"
	print "3. Transfer money"


def my_raw_input(message):
	return raw_input (message)



clientSocket = ""
command = []
commBusy = False
coins = []
coinStart = 0
coinEnd = 0

BankPubKey = ""




def connectToBank():
	global clientSocket
	global sentCommandIndex
	global commandIndex

	clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	clientSocket.connect (('localhost', port))
	
	#while True:
	#	if (commandIndex != sentCommandIndex):
	#		clientSocket.send(command[sentCommandIndex])
	#		command.pop(sentCommandIndex)
	#		sentCommandIndex = sentCommandIndex + 1

	thread2 = Thread(target = waitForResponse, args = ())
	thread2.start()
	
			

def listenForCommands():
	while True:
		if (not commBusy):
			sendCommand (str(my_raw_input ("Command: ")))
			sleep(10)

def sendCommand(comm):      
		global command  
		global commandIndex
                            	
		command.insert(commandIndex, comm)
		commandIndex = commandIndex + 1






def waitForResponse():
	global clientSocket

	#ok = False
	#while ok == False:
	#	try:
	#		response = clientSocket.recv(maxBuffSize)
	#		ok = True
	#	except:
	#		ok = False
	
	

	while True:

		clientSocket.send(str(my_raw_input ("Command: ")))
		response = clientSocket.recv(maxBuffSize)		

		if len(response):
			if (response.strip() == "1"): 
				print "response:1" 
				#commBusy = True

				ID = clientSocket.recv(1)
				print "received id"
				print ID
				BankPubKey = clientSocket.recv(maxBuffSize)
				print "received bankPubKey"
				BankPubKey = pickle.loads(BankPubKey)
				print "pickled it: bankPubKey"

				print "Your ID is: "+str(ID)
				print "Bank public key: "+str(BankPubKey)

				clientSocket.send(pickle.dumps(MyKeys.publickey()))

				#commBusy = False
			elif (response.strip() == "2"):
				print "response:2"
				commBusy = True
			
				while True:
					coin = CUN.getRandomNumber(128,os.urandom)

					coinHash = MD5.new(coin).digest()
					clientSocket.send(pickle.dumps(coinHash))

					signedCoin = MyKeys.sign(coinHash,'')
					clientSocket.send(pickle.dumps(signedCoin))

					response = clientSocket.recv(maxBuffSize)
					if (response == "ok"):
						coins.insert(coinEnd, {})
						coins[coinEnd][coin] = coinHash
						coinEnd += 1
					elif (response == "nomoney"):
						break
					elif (response == "coinNotOk"):
						print "BAD coin"
						break
				
				if (response == "nomoney"):
					print "Withdraw complete"
				else:
					print "Bad coin somewhere"

				commBusy = False
	

MyKeys = RSA.generate(1024,os.urandom)


############ START 
Help()

#connect to the bank
#thread1 = Thread(target = listenForCommands, args = ())
#thread1.start()
connectToBank()

