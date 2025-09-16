import { Prodavnica } from "./Prodavnica.js";
import { Dimenzija } from "./Dimenzija.js";
import { Papir } from "./Papir.js";
import { Fotografija } from "./Fotografija.js";

export class Application{
    constructor(prodavnicaFetched, dimenzijeFetched, papiriFetched){
        this.polja = [
            {naziv: "Dimenzija: ", klasa: "dimenzija"},
            {naziv: "Papir:", klasa: "papir"},
            {naziv: "Ram:", klasa: "ram"}
        ];

        this.prodavnica = prodavnicaFetched.map(p => {
        //if (p && p.naziv && p.dnevnaZarada !== undefined) {
                return new Prodavnica(p.id, p.naziv, p.dnevnaZarada);
        //} else {
          //      console.error("Pogrešni podaci:", JSON.stringify(p, null, 2));
            //    return null;
        //}
        });//.filter(p => p !== null);

        this.dimenzije=dimenzijeFetched.map(p => {
            if (p && p.visina && p.sirina !== undefined) {
                return new Dimenzija(p.id, p.visina, p.sirina);
            } else {
                     console.error("Pogrešni podaci:", JSON.stringify(p, null, 2));
                     return null;
              }
              }).filter(p => p !== null);

        this.papiri=papiriFetched.map(p => {
            if (p && p.naziv !== undefined) {
                return new Papir(p.id, p.naziv);
            } else {
                console.error("Pogrešni podaci:", JSON.stringify(p, null, 2));
                 return null;
            }
            }).filter(p => p !== null);

        this.dataMap = {
            "dimenzija": this.dimenzije,
            "papir": this.papiri,
            "ram": []
        }
    }

    drawForma(container) {
        this.polja.forEach(p => {
            let lbl = document.createElement("label");
            lbl.classList.add("margin-lbl");
            lbl.innerHTML = p.naziv;
            container.appendChild(lbl);
    
            let dropdown = document.createElement("select");
            dropdown.classList.add(`dropdown-${p.klasa}`, "margin-10");
            container.appendChild(dropdown);
    
            // Dodaj opcije prema klasi
            const data = this.dataMap[p.klasa];
            console.log(`Data for ${p.klasa}:`, data); // Loguj podatke za debugovanje
            if (data) {
                data.forEach(item => {
                    let option = document.createElement("option");
                    option.value = item.id;
                    option.innerHTML = p.klasa === "dimenzija"
                        ? `${item.visina} x ${item.sirina}`
                        : item.naziv;
                    dropdown.appendChild(option);
                });
            } else {
                console.warn(`No data found for class ${p.klasa}`);
            }
    
            // Dodaj događaj za promenu ako je ovo dimenzija
            if (p.klasa === "dimenzija") {
                dropdown.classList.add('dropdown-dimenzija');
                dropdown.addEventListener("change", this.handleDimenzijaChange.bind(this));
            } else if (p.klasa === "ram") {
                dropdown.classList.add('dropdown-ram');
            } else if (p.klasa === "papir") {
                dropdown.classList.add('dropdown-papir');
            }
            container.appendChild(dropdown);
        });
    
        const btnPretrazi = document.createElement("input");
        btnPretrazi.type = "button";
        btnPretrazi.value = "Pretrazi";
        btnPretrazi.classList.add("pretraga-dugme");
        btnPretrazi.addEventListener("click", this.handleSearch.bind(this));
        container.appendChild(btnPretrazi);
    }    

    async handleDimenzijaChange(event) {
        const idDimenzije = event.target.value;
        console.log("Event target:", event.target); // Ovo će prikazati element koji je okinuo događaj
        console.log("ID dimenzije:", idDimenzije); // Provera ID-a dimenzije
        if (!idDimenzije) {
            console.error("ID dimenzije nije definisan ili je prazan");
            return;
        }
    
        const selectedDimenzija = this.dimenzije.find(dimenzija => dimenzija.id == idDimenzije);
        const ramovi = await selectedDimenzija.fetchRamByDimenzija();
        const ramDropdown = document.querySelector('.dropdown-ram');
        if (ramDropdown) {
            ramDropdown.innerHTML = ''; // Očisti prethodne opcije
            ramovi.forEach(ram => {
                let option = document.createElement("option");
                option.value = ram.id; // Pretpostavljam da ram ima id
                option.innerHTML = ram.materijal; // Popuni sa materijalom
                ramDropdown.appendChild(option);
            });
        }
    }    
       
    async handleSearch(){ 
        const dimenzijaId = document.querySelector('.dropdown-dimenzija').value;
        const papirId = document.querySelector('.dropdown-papir').value;
        const ramId = document.querySelector('.dropdown-ram').value;

        const queryParams = new URLSearchParams();
        if(dimenzijaId) queryParams.append('dimenzija', dimenzijaId)
        if(papirId && papirId !== "undefined") queryParams.append('papir', papirId);
        if(ramId && ramId !== "undefined") queryParams.append('ram', ramId);
    

        const queryString = queryParams.toString();
        const fotografije = await Fotografija.searchFotografije(queryString);
        console.log("Preuzete fotografije:", fotografije);
        this.drawRezultati(fotografije);
    }

    drawRezultati(fotografije){
        const rezultatiContainer = document.querySelector('.rezultatiPretrage');
        console.log("Rezultati container:", rezultatiContainer); // Provera kontejnera
    
        if (!rezultatiContainer) {
            console.error("Element sa klasom 'rezultatiPretrage' nije pronađen.");
            return;
        }
    
        rezultatiContainer.innerHTML = '';  // Očisti prethodne rezultate
    
        fotografije.forEach(fotografija => {
            console.log("Fotografija objekat:", fotografija); // Provera objekta

            if(fotografija.brFotografija > 0){
                const fotografijaDiv = document.createElement("div");
                fotografijaDiv.classList.add("fotografija");

                const infoDiv = document.createElement("div");
                infoDiv.classList.add("fotografija-info");

                // Proverite šta se dešava sa svojstvima
                console.log(fotografija.naziv, fotografija.dimenzija, fotografija.papir);

                const naslov = document.createElement("p");
                naslov.innerHTML = `${fotografija.nazivSlike} - ${fotografija.dimenzija?.visina} x ${fotografija.dimenzija?.sirina} (${fotografija.nazivPapira})`;
                infoDiv.appendChild(naslov);

                const img = document.createElement("img");
                img.src = `https://localhost:7137${fotografija.slika}`; 
                infoDiv.appendChild(img);

                const btnKupi = document.createElement("input");
                btnKupi.type="button";
                btnKupi.value="Kupi";
                btnKupi.classList.add("kupi-dugme");
                btnKupi.addEventListener("click", () => {
                    this.handleKupovina(fotografija.id);
                });

                fotografijaDiv.appendChild(infoDiv);
                fotografijaDiv.appendChild(btnKupi);
                rezultatiContainer.appendChild(fotografijaDiv);
            }
        });
    }
    
    drawNaslov(container){
        this.prodavnica.forEach(p => {
            let prod = document.createElement("div");
            p.draw(prod);
            container.appendChild(prod);
        });
    }

    draw(container){
         const naslov = document.createElement("div");
         naslov.classList.add("naslov");
         this.drawNaslov(naslov);
         container.appendChild(naslov);

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

    async handleKupovina(id) {
        const result = await Fotografija.kupiFotografiju(id);
        if (result) {
            alert(result);
            this.handleSearch(); // Pretpostavljam da ovo osvežava listu prikazanih fotografija
        }
    }    
}