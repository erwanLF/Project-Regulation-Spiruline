Projet M1 ISEN

Le but de ce projet est de réaliser une automatisation d'un bassin de culture de spiruline (une algue).
Pour se faire une solution a été implémentée sur une Beaglebone black tournant sur Debian 3.8.13

Identifiant pour se connecter: 
usr: debian
mdp: spiruline

Il faut veiller à noter que les paquets soit bien à jours:
 - sudo apt-get install 
 - sudo apt-get upgrade

Pour utiliser la cape motor qui se trouve sur le site : https://www.seeedstudio.com/Motor-Bridge-Cape-p-2569.html
Il faut veiller que la date de la beaglebone soit bien à jours:

-sudo date -s **:**:** 
-sudo date -s **/**/**

Puis "flasher" le firmware : 
cd /Desktop/motor/MotorBridgeCapeFirmware 
-sudo make flash 
Verifier que le flash a bien fonctionné:
-sudo i2cdetect -y -r 1
On doit observer l'adresse 0x4b qui correspond à l'adresse i2c de communication entre la cape et la BBB

pour compiler le précédent projet :

-cd Desktop/c/manip
-sudo gcc manip.c ../dht22/bbb_dht_read.c ../dht22/bbb_mmio.c ../dht22/common_dht_read.c -o manip -lrt -lm
