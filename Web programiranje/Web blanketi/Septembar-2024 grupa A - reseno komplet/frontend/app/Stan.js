export class Stan{
    constructor(id){
        this.id=id;
    }

    async searchStan(id){
        try{
            const response = await fetch(`https://localhost:7080/PodaciOStanu/${id}`);
            return await response.json()
        }
        catch(error){
            console.log("Greska prilikom preuzimanja ID stanova: ", error);
            return [];
        }
    }
}