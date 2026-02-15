  TP Analyseur de Logs Réseau 

Description
Ce projet propose deux outils d'analyse de fichiers de logs réseau pour détecter des tentatives d'intrusion

Fichiers du projet
- "network_log.txt" : Le fichier contenant les traces réseau.
- "analyse_logs.py" : Script d'analyse en Python
- "analyse_logs.c" : Programme d'analyse en C 
- "rapport_analyse.txt" : Rapport généré automatiquement après analyse.

Comment utilser ces fichiers ?

voici la procedure d'utilisation du projet:

1.  En Python
Lancer l'analyse avec la commande :
"Gith bash"

python analyse_logs.py

2. En C
  a. Compilation 
gcc analyse_logs.c -o analyse_logs


pour voir le resultat en C vous devez exécueter le fichier analyse_logs.exe que le programme va regenerer et une fois l'exécuter, il va vous creer un fichier txt avec le meme rapport que celui du programme python qui sera creé dans un dossier Rapport.
b. Exécution
./analyse_logs
