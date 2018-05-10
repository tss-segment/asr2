#!/usr/bin/python3

import requests
import sqlite3
import json




fichierDonnees = "./donneesWiFi.sq3"
base = sqlite3.connect(fichierDonnees)
curseur = base.cursor()
#connexion à la base de donnees
curseur.execute("SELECT cle, strength FROM reseaux")
# On va chercher les champs qui nous intéressent pour la requête à l'API Google
requete = curseur.fetchall()
base.commit()
curseur.close()
base.close()

# On en a fini avec la base de données !




url = "https://www.googleapis.com/geolocation/v1/geolocate?key=AIzaSyB7i-tbaEahgWHnRdfuM3JXxdsTaSy7bJY"
# L'url où l'on envoie la requête
# La clé importe peu, j'ai créé un projet spécialement pour l'occasion simplement pour en avoir une



payload = { "considerIp": "false", "wifiAccessPoints": [] }
# Là où l'on range les données à envoyer

for i in range(len(requete)):
	(cle, signal) = requete[i]
	payload["wifiAccessPoints"].append({"macAddress": cle, "signalStrength": signal})
	# Pour chaque élément trouvé dans la structure de données, on l'ajoute dans les données à envoyer



r = requests.post(url, json=payload)
# On envoie la requête en spécifiant que les données sont à passer en format json



localisation = r.json()
# On obtient les données réponse du serveur, que l'on décode depuis le format json
# La structure exacte est décrite sur le site de l'API Google


latitude = localisation['location']['lat']
longitude = localisation['location']['lng']
precision = localisation['accuracy']

print("Les réseaux ont été scannés dans un rayon de {0} mètres autour du point de latitude {1} et de longitude {2}".format(precision,latitude,longitude))
