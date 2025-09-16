export class Dimenzija{
    constructor(id, visina, sirina){
        this.id=id;
        this.visina=visina;
        this.sirina=sirina;
    }

    async fetchRamByDimenzija() {
        try {
            console.log(`Fetching ramovi for dimenzija ID: ${this.id}`); // Loguj ID dimenzije
    
            const response = await fetch(`https://localhost:7137/UzmiRamPoDimenziji/${this.id}`);
            console.log("Response status:", response.status); // Loguj status odgovora
            const responseData = await response.json();
            console.log("Fetched ramovi:", responseData); // Loguj povratni podatak
    
            if (!response.ok) {
                throw new Error('Ne mogu da preuzmem materijale ramova');
            }
    
            return responseData;
        } catch (error) {
            console.error("Greška pri preuzimanju ramova:", error);
            return [];
        }
    }  
}