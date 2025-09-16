import { Kompanija } from "./Kompanija.js";
import { Vozilo } from "./Vozilo.js";

export class Application{
    constructor(){
        this.polja = [
            {naziv: "Zapremina (cm^3): ", klasa: "zapremina"},
            {naziv: "Tezina (kg): ", klasa: "tezina"},
            {naziv: "Datum prijema: ", klasa: "datumPrijema"},
            {naziv: "Datum dostave: ", klasa: "datumDostave"},
            {naziv: "Cena od: ", klasa: "cenaOd"},
            {naziv: "Cena do: ", klasa: "cenaDo"}
        ]; 
    }

    draw(container){
        const bodyContainer = document.createElement("div");
        bodyContainer.classList.add("bodyContainer");

        const forma = document.createElement("div");
        forma.classList.add("kriterijumiPretrage");
        this.drawForma(forma);
        bodyContainer.appendChild(forma);

        const rezultatiPretrage = document.createElement("div");
        rezultatiPretrage.classList.add("rezultatiPretrage");
        bodyContainer.appendChild(rezultatiPretrage);

        container.appendChild(bodyContainer);
    }

    drawForma(container){
        this.polja.forEach(p => {
            let lbl = document.createElement("label");
            lbl.innerHTML = p.naziv;
            lbl.classList.add("margin-10");
            container.appendChild(lbl);

            let unos = document.createElement("input");
            unos.classList.add("margin-10", `input-${p.klasa}`);
            if(p.klasa === "datumPrijema" || p.klasa === "datumDostave"){
                unos.type="datetime-local";
                unos.innerHTML='';
            }
            container.appendChild(unos);
        });

        const btnPretrazi = document.createElement("input");
        btnPretrazi.type = "button";
        btnPretrazi.classList.add("pretraga-dugme", "margin-10");
        btnPretrazi.value = "Pretrazi";
        btnPretrazi.addEventListener("click", this.handleSearch.bind(this));
        container.appendChild(btnPretrazi);
    }

    async handleSearch(){
        const zapremina = document.querySelector('.input-zapremina').value;
        const tezina = document.querySelector('.input-tezina').value;
        const datumPrijema = document.querySelector('.input-datumPrijema').value;
        const datumDostave = document.querySelector('.input-datumDostave').value;
        const cenaOd = document.querySelector('.input-cenaOd').value;
        const cenaDo = document.querySelector('.input-cenaDo').value;

        console.log("Podaci iz forme:");
        console.log("Tezina: ", tezina);
        console.log("Zapremina:", zapremina);
        console.log("Datum prijema:", datumPrijema);
        console.log("Datum dostave:", datumDostave);
        console.log("Cena od:", cenaOd);
        console.log("Cena do:", cenaDo);

        const dostave = await Kompanija.searchKompanijeVozila(zapremina, tezina, datumPrijema, datumDostave, cenaOd, cenaDo);
        this.drawRezultati(dostave);
    }

    drawRezultati(dostave) {
        const rezultatiPretrage = document.querySelector('.rezultatiPretrage');
        if (rezultatiPretrage) {
            rezultatiPretrage.innerHTML = ''; // Očisti prethodne rezultate
            if (Array.isArray(dostave)) { // Proveri da li je dostave niz
                dostave.forEach(dostava => {
                    const kompanija = new Kompanija(
                        dostava.id,
                        dostava.naziv,
                        dostava.prosecnaZarada,
                        dostava.cena,
                        dostava.slikaPath
                    );
                    kompanija.draw([dostava]); // Prosledi niz sa jednim objektom
                });
            } else {
                console.error("Podaci za prikaz nisu u ispravnom formatu:", dostave);
            }
        }
    }    
}