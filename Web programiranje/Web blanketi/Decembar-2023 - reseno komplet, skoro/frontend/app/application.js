import { Prodavnica } from "./Prodavnica.js";

export class Application{
    constructor(prodavniceFetched){
        this.prodavnica = prodavniceFetched.map(p => {
            console.log(p);
            return new Prodavnica(p.id, p.naziv);
        });
    }

    draw(container){
        this.prodavnica.forEach(p => {
            p.draw(container);
        });
    }
}