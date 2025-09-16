export class Marka{
    constructor(id, naziv){
        this.id=id;
        this.naziv=naziv;
    }
    async fetchModeliByMarka() {
        try {
            const response = await fetch(`https://localhost:7137/PreuzmiModel/${this.id}`);
            const responseDataModel = await response.json();

            if (!response.ok) {
                throw new Error('Ne mogu da preuzmem modele automobila');
            }

            return responseDataModel;
        } catch (error) {
            console.error("Greška pri preuzimanju modela: ", error);
            return [];
        }
    }

    async fetchBojaByMarka() {
        try {
            const response = await fetch(`https://localhost:7137/PreuzmiBoju/${this.id}`);
            const responseDataBoja = await response.json();

            if (!response.ok)
                throw new Error('Ne mogu da preuzmem boje automobila');

            return responseDataBoja;
        } catch (error) {
            console.error("Greska pri preuzimanju boja: ", error);
            return [];
        }
    }
}