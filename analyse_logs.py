from collections import defaultdict, Counter
import os

def lire_logs(nom_fichier):
    logs = []
    with open(nom_fichier, "r") as f:
        for ligne in f:
            champs = ligne.strip().split(";")
            log = {
                "date": champs[0],
                "heure": champs[1],
                "ip": champs[2],
                "port": champs[3],
                "protocole": champs[4],
                "statut": champs[5]
            }
            logs.append(log)
    return logs


def calculer_statistiques(logs):
    total = len(logs)
    succes = sum(1 for log in logs if log["statut"] == "SUCCES")
    echec = sum(1 for log in logs if log["statut"] == "ECHEC")

    ports = Counter(log["port"] for log in logs)
    ips = Counter(log["ip"] for log in logs)

    port_plus = ports.most_common(1)[0]
    ip_plus = ips.most_common(1)[0]
    top3 = ports.most_common(3)

    return total, succes, echec, port_plus, ip_plus, top3


def detecter_ips_suspectes(logs):
    echecs = defaultdict(int)

    for log in logs:
        if log["statut"] == "ECHEC":
            cle = (log["ip"], log["port"])
            echecs[cle] += 1

    return [(ip, port, nb) for (ip, port), nb in echecs.items() if nb > 5]


def generer_rapport(stats, suspectes):
    total, succes, echec, port_plus, ip_plus, top3 = stats

    os.makedirs("rapport", exist_ok=True)

    with open("rapport/rapport_analyse.txt", "w", encoding="utf-8") as f:
        f.write("===== RAPPORT D'ANALYSE DES LOGS =====\n\n")
        f.write(f"Total connexions : {total}\n")
        f.write(f"Succès : {succes}\n")
        f.write(f"Échecs : {echec}\n\n")
        f.write(f"Port le plus utilisé : {port_plus[0]} ({port_plus[1]} fois)\n")
        f.write(f"IP la plus active : {ip_plus[0]} ({ip_plus[1]} connexions)\n\n")

        f.write("Top 3 des ports :\n")
        for port, nb in top3:
            f.write(f" - {port} : {nb}\n")

        f.write("\nIP suspectes :\n")
        for ip, port, nb in suspectes:
            f.write(f" - {ip} port {port} -> {nb} échecs\n")

    print("Rapport généré dans dossier rapport/")


def main():
    logs = lire_logs("network_log.txt")
    stats = calculer_statistiques(logs)
    suspectes = detecter_ips_suspectes(logs)
    generer_rapport(stats, suspectes)

main()
