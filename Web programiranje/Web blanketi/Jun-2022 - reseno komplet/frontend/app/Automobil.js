export class Automobil{
    constructor(id, slikaPath, kolicina, cena, datumPoslednjeProdaje, marka, model, boja){
        this.id=id;
        this.slikaPath=slikaPath;
        this.kolicina=kolicina;
        this.cena=cena;
        this.datumPoslednjeProdaje=datumPoslednjeProdaje;
        this.marka=marka;
        this.model=model;
        this.boja=boja;
    }
    static async searchAutomobili(queryString) {
        try {
            const response = await fetch(`https://localhost:7137/PronadjiAutomobil?${queryString}`);
            if (!response.ok) {
                const errorText = await response.text();
                throw new Error(`Ne mogu da preuzmem rezultate pretrage: ${errorText}`);
            }
            return await response.json();
        } catch (error) {
            console.error("Greška pri pretrazi automobila:", error);
            return [];
        }
    }

    static async naruciAutomobil(marka, model) {
        try {
            const response = await fetch(`https://localhost:7137/NaruciAutomobil/${marka}/${model}`, {
                method: 'PUT',
            });

            if (!response.ok) {
                const errorText = await response.text();
                throw new Error(`Greška pri poručivanju automobila: ${errorText}`);
            }

            return await response.text();
        } catch (error) {
            console.error("Greška pri poručivanju automobila:", error);
            return null;
        }
    }
}