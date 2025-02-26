import os
import requests
import zipfile

def scarica_e_espandi_windows_libraries_feelib():
    """
    Scarica l'asset 'windows_libraries.zip' dall'ultima release di 'caenspa/caen-felib'
    e lo estrae nella cartella 'tmp/caen'.
    """
    # Endpoint GitHub per la release più recente
    url = "https://api.github.com/repos/caenspa/caen-felib/releases/latest"
    
    # Richiesta GET per ottenere i dati in formato JSON
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

    # Scarichiamo il file zip localmente
    zip_path = os.path.join(download_path, "windows_libraries.zip")
    print(f"Scarico {download_url} in {zip_path} ...")
    r = requests.get(download_url)
    r.raise_for_status()
    with open(zip_path, "wb") as f:
        f.write(r.content)
    
    # Espandiamo il file zip
    print(f"Estrazione di {zip_path} in {download_path} ...")
    with zipfile.ZipFile(zip_path, "r") as zip_ref:
        zip_ref.extractall(download_path)
    
    print("Download e estrazione completati con successo.")



def scarica_e_espandi_windows_libraries_dig2():
    """
    Scarica l'asset 'windows_libraries.zip' dall'ultima release di 'caenspa/caen-felib'
    e lo estrae nella cartella 'tmp/caen'.
    """
    # Endpoint GitHub per la release più recente
    url = "https://api.github.com/repos/caenspa/caen-dig2/releases/latest"
    
    # Richiesta GET per ottenere i dati in formato JSON
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

    # Scarichiamo il file zip localmente
    zip_path = os.path.join(download_path, "windows_libraries.zip")
    print(f"Scarico {download_url} in {zip_path} ...")
    r = requests.get(download_url)
    r.raise_for_status()
    with open(zip_path, "wb") as f:
        f.write(r.content)
    
    # Espandiamo il file zip
    print(f"Estrazione di {zip_path} in {download_path} ...")
    with zipfile.ZipFile(zip_path, "r") as zip_ref:
        zip_ref.extractall(download_path)
    
    print("Download e estrazione completati con successo.")

if __name__ == "__main__":
    scarica_e_espandi_windows_libraries_feelib()
    scarica_e_espandi_windows_libraries_dig2()