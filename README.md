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
b. Exécution
./analyse_logs

pour voi le résultat en C, exécutez le fichier exe qui sera regeneré par le programme, et ce dernier va creer un fichier txt qui aura le même resultat que celui de ^ython creé dans un dossier rapport.




