#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IP 2000
#define MAX_PORTS 65536
#define MAX_LOGS 2000

typedef struct {
    char ip[16];
    int count;   // nombre de connexions
} CompteurIP;

typedef struct {
    char ip[16];
    int port;
    int count;   // nombre d'échecs
} Echec;

typedef struct {
    char date[11];
    char heure[9];
    char ip[16];
    int port;
    char proto[10];
    char statut[10];
} LogEntry;

int trouver_ip(CompteurIP arr[], int n, char *ip) {
    for (int i = 0; i < n; i++)
        if (strcmp(arr[i].ip, ip) == 0)
            return i;
    return -1;
}

int trouver_ip_port(Echec arr[], int n, char *ip, int port) {
    for (int i = 0; i < n; i++)
        if (strcmp(arr[i].ip, ip) == 0 && arr[i].port == port)
            return i;
    return -1;
}

int main() {
    FILE *file = fopen("network_log.txt", "r");
    if (!file) {
        printf("Erreur : Impossible de lire network_log.txt\n");
        return 1;
    }

    LogEntry logs[MAX_LOGS];
    int total = 0;
    int n_succes = 0, n_echecs = 0;
    int ports[MAX_PORTS] = {0};
    CompteurIP ips[MAX_IP];
    int nb_ips = 0;
    Echec echecs[MAX_LOGS];
    int nb_echecs = 0;

    char ligne[256];

    // Lecture des logs
    while (fgets(ligne, sizeof(ligne), file)) {
        if (strlen(ligne) <= 1) continue; // ignore lignes vides

        char date[20], heure[20], ip[20], protocole[10], statut[10];
        int port;

        int lus = sscanf(ligne, "%[^;];%[^;];%[^;];%d;%[^;];%s", 
                        date, heure, ip, &port, protocole, statut);
        if (lus != 6) continue;
        if (port < 0 || port >= MAX_PORTS) continue;

        strcpy(logs[total].date, date);
        strcpy(logs[total].heure, heure);
        strcpy(logs[total].ip, ip);
        logs[total].port = port;
        strcpy(logs[total].proto, protocole);
        strcpy(logs[total].statut, statut);
        total++;

        ports[port]++;

        int idx = trouver_ip(ips, nb_ips, ip);
        if (idx == -1 && nb_ips < MAX_IP) {
            strcpy(ips[nb_ips].ip, ip);
            ips[nb_ips].count = 1;
            nb_ips++;
        } else if (idx != -1) {
            ips[idx].count++;
        }

        if (strcmp(statut, "SUCCES") == 0) n_succes++;
        else {
            n_echecs++;
            int i = trouver_ip_port(echecs, nb_echecs, ip, port);
            if (i == -1 && nb_echecs < MAX_LOGS) {
                strcpy(echecs[nb_echecs].ip, ip);
                echecs[nb_echecs].port = port;
                echecs[nb_echecs].count = 1;
                nb_echecs++;
            } else if (i != -1) {
                echecs[i].count++;
            }
        }
    }
    fclose(file);

    // Port le plus utilisé
    int port_max = 0;
    for (int i = 1; i < MAX_PORTS; i++)
        if (ports[i] > ports[port_max]) port_max = i;

    // IP la plus active
    int ip_max = 0;
    for (int i = 1; i < nb_ips; i++)
        if (ips[i].count > ips[ip_max].count) ip_max = i;

    // Top 3 ports
    int top_ports[3] = {-1, -1, -1};
    int top_counts[3] = {0, 0, 0};
    for (int i = 0; i < MAX_PORTS; i++) {
        if (ports[i] > top_counts[0]) {
            top_counts[2] = top_counts[1];
            top_ports[2] = top_ports[1];
            top_counts[1] = top_counts[0];
            top_ports[1] = top_ports[0];
            top_counts[0] = ports[i];
            top_ports[0] = i;
        } else if (ports[i] > top_counts[1]) {
            top_counts[2] = top_counts[1];
            top_ports[2] = top_ports[1];
            top_counts[1] = ports[i];
            top_ports[1] = i;
        } else if (ports[i] > top_counts[2]) {
            top_counts[2] = ports[i];
            top_ports[2] = i;
        }
    }

    FILE *rapport = fopen("rapport_analyse.txt", "w");
    fprintf(rapport, "===== RAPPORT D'ANALYSE DES LOGS =====\n\n");
    fprintf(rapport, "Total connexions : %d\n", total);
    fprintf(rapport, "Succès : %d\n", n_succes);
    fprintf(rapport, "Échecs : %d\n\n", n_echecs);
    fprintf(rapport, "Port le plus utilisé : %d (%d fois)\n", port_max, ports[port_max]);
    fprintf(rapport, "IP la plus active : %s (%d connexions)\n\n", ips[ip_max].ip, ips[ip_max].count);

    fprintf(rapport, "Top 3 des ports :\n");
    for (int i = 0; i < 3; i++) {
        if (top_ports[i] != -1)
            fprintf(rapport, " - %d : %d\n", top_ports[i], top_counts[i]);
    }

    fprintf(rapport, "\nIP suspectes :\n");
    int trouve = 0;
    for (int i = 0; i < nb_echecs; i++) {
        if (echecs[i].count > 5) {
            fprintf(rapport, " - %s port %d -> %d échecs\n", echecs[i].ip, echecs[i].port, echecs[i].count);
            trouve = 1;
        }
    }
    if (!trouve) fprintf(rapport, "Aucune IP suspecte détectée\n");

    fclose(rapport);

    printf("Analyse terminée. Rapport généré dans rapport_analyse.txt\n");
    printf("Appuyez sur Entrée pour quitter...");
    getchar();

    return 0;
}


