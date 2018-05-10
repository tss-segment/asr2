#!/usr/bin/python3

import serial
import sqlite3
import time




ser = serial.Serial(port = '/dev/ttyUSB0', baudrate = 115200)
# Ouverture du port série



fichierDonnees = "./donneesWiFi.sq3"
# Notre base de données où seront stockés les réseaux WiFi scannés (remplacer par le path adéquat)



base = sqlite3.connect(fichierDonnees)
curseur = base.cursor()



curseur.execute("CREATE TABLE reseaux (nom TEXT, cle TEXT, strength INT, jour DATE, heure TIME)")
# Création de la table dans la base de données : à mettre en commentaire après la première exécution du programme




while(True) :
	# L'esp8266 scanne les réseaux WiFi en continu


	line = ser.readline().decode()
	# Essayons de voir s'il s'agit des informations sur un réseau
	date = time.localtime()
	year = str(date.tm_year)
	month = str(date.tm_mon)
	day = str(date.tm_mday)
	hour = str(date.tm_hour)
	minute = str(date.tm_min)
	second = str(date.tm_sec)
	if (len(month) == 1) :
		month = "0"+month
	if (len(day) == 1) :
		day = "0"+day
	if (len(hour) == 1) :
		hour = "0"+hour
	if (len(minute) == 1) :
		minute = "0"+minute
	if (len(second) == 1) :
		second = "0"+second
	date_str = year+"-"+month+"-"+day
	hour_str = hour+":"+minute+":"+second

		
		

	if (line[0] >= "0" and line[0] <= "9"):
		# Soit c'est un réseau, soit c'est la ligne annonçant combien de réseaux ont été scannés
		i = 1
		while (line[i] >= "0" and line[i] <= "9"):
			i += 1
		if (line[i] == ":"):
			# C'est un réseau
			i += 2
			SSID = ""
			while (line[i] != " "):
				# Tant pis pour les gens qui mettent des espaces dans leur SSID, c'est mal de leur part
				SSID += line[i]
				i += 1
			i += 2
			strength = ""
			while (line[i] != ")"):
				strength += line[i]
				i += 1
			MACadd = ""
			while (line[i] != ":"):
				i += 1
			# On atteint l'adresse MAC
			i += 2
			while (line[i] != ","):
				MACadd += line[i]
				i += 1

			curseur.execute("INSERT INTO reseaux(nom, cle, strength, jour, heure) VALUES(?,?,?,?,?)", (SSID, MACadd, int(strength), date_str, hour_str))
			# Action à exécuter

			base.commit()
			# Sauvegarde des actions exécutées par le curseur dans la base de données
		



curseur.close()
base.close()
ser.close()
# Je referme le port série par principe
# Techniquement cela n'arrivera jamais puisque qu'on ne quittera pas la boucle While
# Je ne suis pas certain que ce soit très propre, mais après tout, la fonction loop() du code Arduino se répète indéfiniment...
