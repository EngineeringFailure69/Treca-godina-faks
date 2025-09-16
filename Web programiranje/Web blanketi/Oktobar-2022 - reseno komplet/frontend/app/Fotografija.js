export class Fotografija{
    constructor(id, brFotografija, slikaPath, naziv, prodavnica, papir, dimenzija, ram){
        this.id=id;
        this.brFotografija=brFotografija;
        this.slikaPath=slikaPath;
        this.naziv=naziv;
        this.prodavnica=prodavnica;
        this.papir=papir;
        this.dimenzija=dimenzija;
        this.ram=ram;
    }

    static async searchFotografije(queryString){
        try{
            const response = await fetch(`https://localhost:7137/VratiSveSlikeUZavisnostiOdKriterijuma?${queryString}`);
            if(!response.ok){
                const errorText = await response.text();
                throw new Error(`Ne mogu da preuzmem rezultate pretrage: ${errorText}`);
            }
            return await response.json();
        } catch(error){
            console.error("Greska pri pretrazivanju slika", error);
            return [];
        }
    }

    static async kupiFotografiju(id) {
        try {
            const response = await fetch(`https://localhost:7137/KupiFotografiju/${id}`, {
                method: 'PUT',
            });
    
            if (!response.ok) {
                const errorText = await response.text();
                throw new Error(`Greska pri kupovini fotografije: ${errorText}`);
            }
    
            return await response.text();
        } catch (error) {
            console.error("Greska pri kupovini fotografije:", error);
            return null;
        }
    }
    
}