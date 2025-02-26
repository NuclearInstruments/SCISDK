import os
import json
import zipfile
import requests

def scarica_e_espandi_windows_libraries_feelib():
    """
    Scarica l'asset 'windows_libraries.zip' dall'ultima release di 'caenspa/caen-felib'
    e lo estrae nella cartella 'tmp/caen'.
    """
    # Endpoint GitHub per la release più recente
    url = "https://api.github.com/repos/caenspa/caen-felib/releases/latest"

    # Eseguiamo la richiesta con requests
    response = requests.get(url)
    response.raise_for_status()  # alza un'eccezione in caso di errore
    data = response.json()

    # Cerca l'asset 'windows_libraries.zip'
    target_asset = None
    for asset in data.get("assets", []):
        if asset["name"] == "windows_libraries.zip":
            target_asset = asset
            break

    if not target_asset:
        print("Asset 'windows_libraries.zip' non trovato!")
        return

    download_url = target_asset["browser_download_url"]

    # Creiamo la cartella di destinazione se non esiste
    download_path = "tmp/caen"
    os.makedirs(download_path, exist_ok=True)

    # Percorso completo del file .zip
    zip_path = os.path.join(download_path, "windows_libraries.zip")

    print(f"Scarico {download_url} in {zip_path} ...")

    # Scarichiamo il file zip localmente usando requests
    with requests.get(download_url, stream=True) as r:
        r.raise_for_status()
        with open(zip_path, "wb") as f:
            for chunk in r.iter_content(chunk_size=8192):
                if chunk:
                    f.write(chunk)

    # Espandiamo il file zip
    print(f"Estrazione di {zip_path} in {download_path} ...")
    with zipfile.ZipFile(zip_path, "r") as zip_ref:
        zip_ref.extractall(download_path)

    print("Download e estrazione completati con successo.")


def scarica_e_espandi_windows_libraries_dig2():
    """
    Scarica l'asset 'windows_libraries.zip' dall'ultima release di 'caenspa/caen-dig2'
    e lo estrae nella cartella 'tmp/caen'.
    """
    # Endpoint GitHub per la release più recente
    url = "https://api.github.com/repos/caenspa/caen-dig2/releases/latest"

    # Eseguiamo la richiesta con requests
    response = requests.get(url)
    response.raise_for_status()
    data = response.json()

    # Cerca l'asset 'windows_libraries.zip'
    target_asset = None
    for asset in data.get("assets", []):
        if asset["name"] == "windows_libraries.zip":
            target_asset = asset
            break

    if not target_asset:
        print("Asset 'windows_libraries.zip' non trovato!")
        return

    download_url = target_asset["browser_download_url"]

    # Creiamo la cartella di destinazione se non esiste
    download_path = "tmp/caen"
    os.makedirs(download_path, exist_ok=True)

    # Percorso completo del file .zip
    zip_path = os.path.join(download_path, "windows_libraries.zip")

    print(f"Scarico {download_url} in {zip_path} ...")

    # Scarichiamo il file zip localmente usando requests
    with requests.get(download_url, stream=True) as r:
        r.raise_for_status()
        with open(zip_path, "wb") as f:
            for chunk in r.iter_content(chunk_size=8192):
                if chunk:
                    f.write(chunk)

    # Espandiamo il file zip
    print(f"Estrazione di {zip_path} in {download_path} ...")
    with zipfile.ZipFile(zip_path, "r") as zip_ref:
        zip_ref.extractall(download_path)

    print("Download e estrazione completati con successo.")


if __name__ == "__main__":
    scarica_e_espandi_windows_libraries_feelib()
    scarica_e_espandi_windows_libraries_dig2()
