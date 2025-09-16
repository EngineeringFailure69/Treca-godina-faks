export class Vozilo{
    constructor(id, naziv, slikaPath, zapremina, tezina, cena, datumPrijema, datumIsporuke, kompanijinoVozilo){
        this.id=id;
        this.naziv=naziv;
        this.slikaPath=slikaPath;
        this.zapremina=zapremina;
        this.tezina=tezina;
        this.cena=cena;
        this.datumPrijema=datumPrijema;
        this.datumIsporuke=datumIsporuke;
        this.kompanijinoVozilo=kompanijinoVozilo;
    }
}