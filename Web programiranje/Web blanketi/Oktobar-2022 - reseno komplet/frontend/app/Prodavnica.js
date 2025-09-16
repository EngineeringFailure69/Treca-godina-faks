export class Prodavnica{
    constructor(id, naziv, dnevnaZarada){
        this.id=id;
        this.naziv=naziv;
        this.dnevnaZarada=dnevnaZarada;
    }

    draw(container){
        const lbl = document.createElement("label");
        lbl.innerHTML = `${this.naziv} - ${this.dnevnaZarada} din`;
        container.appendChild(lbl);
    }
}