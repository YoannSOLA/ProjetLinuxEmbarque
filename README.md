# ProjetLinuxEmbarque

Notre projet est constitué d'un client et de deux serveurs :

- miniserver_armadeus : ce programme correspond au serveur qui est présent dans la carte Armadeus et qui permet d'allumer et d'éteindre la LED 21 à la réception du code morse envoyé par le client.
               
- miniserver_computer : ce programme correspond au serveur qui permet de décrypter les messages du client depuis un ordinateur et non depuis une carte Armadeus.


Pour tester le miniserver_computer, il faut effectuer dans l'ordre les étapes suivantes :

1. Lancer dans un terminal le programme "miniserver_computer" qui se trouve dans le répertoire ProjetLinuxEmbarque/src/miniserver

2. Lancer dans un second terminal le programme "client" qui se trouve dans le répertoire ProjetLinuxEmbarque/src/client via la commande suivante :
   
        client 127.0.0.1 {message}
     
   L'argument 127.0.0.1 correspond à l'adresse IP local.
   
   L'argument {message} doit être remplacé par le message que le client veut envoyer au serveur.
   
   /!\ Le message ne doit contenir que des lettres majuscules. Les lettres minuscules et les chifres ne sont pas permis.
     
3. Le client va alors afficher le message à envoyer en clair, puis son cryptage par chiffre de vigenère, et enfin le codage en morse du message crypté. Il va ensuite envoyer ce dernier au serveur.
   
   La clé du cryptage par Vigenère peut être changé grâce à la variable "char *code", présente dans le fichier ProjetLinuxEmbarque/src/client/client.c
   
4. Le serveur va recevoir le message du client, va déchiffrer le code morse, puis décrypter le chiffage par Vigenère et afficher le message en clair.

     /!\ Si la clé de chiffrage a été modifié dans le client.c, il faut penser à actualiser la valeur de la variable "char *code", présente dans le fichier ProjetLinuxEmbarque/src/miniserver/miniserver_computer.c
   
