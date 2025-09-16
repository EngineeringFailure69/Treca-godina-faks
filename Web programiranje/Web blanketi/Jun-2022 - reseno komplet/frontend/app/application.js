import { Marka } from "./Marka.js";
import { Automobil } from "./Automobil.js";

export class Application{
    constructor(markeFetched){
        this.polja=[
            {naziv: "Marka:", klasa: "marka"},
            {naziv: "Model:", klasa: "model"},
            {naziv: "Boja:", klasa: "boja"}
        ];

        this.marke = markeFetched.map(p=>{
            return new Marka(p.id, p.naziv);
        });

        this.dataMap = {
            "marka": this.marke,
            "model": [],
            "boja": []
        }

        this.initializeEventListeners();
    }

    initializeEventListeners() {
        document.addEventListener('DOMContentLoaded', () => {
            const markaDropdown = document.querySelector('.dropdown-marka');
            const searchButton = document.querySelector('.pretraga-dugme');

            if (markaDropdown) {
                markaDropdown.addEventListener('change', this.handleMarkaChange.bind(this));
            }

            if (searchButton) {
                searchButton.addEventListener('click', this.handleSearch.bind(this));
            }
        });
    }

    draw(container){
        const bodyContainer=document.createElement("div");
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

            let dropdown = document.createElement("select");
            dropdown.classList.add(`dropdown-${p.klasa}`,"margin-10");
            container.appendChild(dropdown);

            const data = this.dataMap[p.klasa];
            if(data){
                data.forEach(item=>{
                    let option = document.createElement("option");
                    option.value=item.id;
                    option.innerHTML = p.klasa === "marka" 
                    ? `${item.naziv}` : null;
                    dropdown.appendChild(option);
                });
            }

            if(p.klasa === "marka"){
                dropdown.classList.add('dropdown-marka');
                dropdown.addEventListener("change", this.handleMarkaChange.bind(this))
            } else if(p.klasa === "model"){
                dropdown.classList.add('dropdown-model');
            } else if(p.klasa === "boja"){
                dropdown.classList.add('dropdown-boja');
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

    async handleMarkaChange(event){
        const idMarke = event.target.value;
        if(!idMarke){
            console.error("Id marke nije definisan");
            return;
        }

        const selectedMarka = this.marke.find(marka => marka.id == idMarke);
        const modeli = await selectedMarka.fetchModeliByMarka();
        const modelDropdown = document.querySelector('.dropdown-model');
        if(modelDropdown){
            modelDropdown.innerHTML='<option value=""></option>';
            modeli.forEach(model=>{
                let option = document.createElement("option");
                option.value = model.id;
                option.innerHTML = model.naziv;
                modelDropdown.appendChild(option);
            });
        }

        const boje = await selectedMarka.fetchBojaByMarka();
        const bojaDropdown = document.querySelector('.dropdown-boja');
        if(bojaDropdown){
            bojaDropdown.innerHTML='<option value=""></option>';
            boje.forEach(boja=>{
                let option = document.createElement("option");
                option.value = boja.id;
                option.innerHTML = boja.naziv;
                bojaDropdown.append(option);
            });
        }
    }

    async handleSearch() {
        const markaId = document.querySelector('.dropdown-marka').value;
        const modelId = document.querySelector('.dropdown-model').value;
        const bojaId = document.querySelector('.dropdown-boja').value;
    
        console.log("Marka ID:", markaId);
        console.log("Model ID:", modelId);
        console.log("Boja ID:", bojaId);
    
        // Kreiraj query string sa ID-ovima, izostavljajući one koji nisu definisani
        const queryParams = new URLSearchParams();
        if (markaId) queryParams.append('marka', markaId);
        if (modelId) queryParams.append('model', modelId);
        if (bojaId) queryParams.append('boja', bojaId);
    
        const queryString = queryParams.toString();
        const automobili = await Automobil.searchAutomobili(queryString);
        this.drawRezultati(automobili);
    }    

    drawRezultati(automobili) {
        const rezultatiContainer = document.querySelector('.rezultatiPretrage');
        rezultatiContainer.innerHTML = ''; // Brisanje prethodnih rezultata
    
        automobili.forEach(auto => {
            if(auto.kolicina>0){
                const autoDiv = document.createElement("div");
                console.log("Datum poslednje prodaje:", auto.datumPoslednjeProdaje);
                autoDiv.classList.add("auto");
        
                const infoDiv = document.createElement("div");
                infoDiv.classList.add("auto-info");
        
                const marka = document.createElement("p");
                marka.innerHTML = `Marka: ${auto.markaNaziv}`;
                infoDiv.appendChild(marka);

                const Model = document.createElement("p");
                Model.innerHTML = `Model: ${auto.modelNaziv}`;
                infoDiv.appendChild(Model);

                const imgPath = `https://localhost:7137${auto.slikaPath}`;
                console.log("Full putanja slike:", imgPath);

                const img = document.createElement("img");
                img.src = `https://localhost:7137${auto.slikaPath}`;
                img.alt = `${auto.markaNaziv} - ${auto.modelNaziv}`;
                infoDiv.appendChild(img);
        
                const kolicina = document.createElement("p");
                kolicina.innerHTML = `Kolicina: ${auto.kolicina}`;
                infoDiv.appendChild(kolicina);

                const datumProdaje = document.createElement("p");
                const datum = new Date(auto.datumPoslednjeProdaje).toLocaleDateString('sr-RS'); 
                datumProdaje.innerHTML = `Datum poslednje prodaje: ${datum}`;
                infoDiv.appendChild(datumProdaje);

                const cena = document.createElement("p");
                cena.innerHTML = `Cena: ${auto.cena} RSD`;
                infoDiv.appendChild(cena);
        
                const btnKupi = document.createElement("input");
                btnKupi.type = "button";
                btnKupi.value = "Naruci";
                btnKupi.classList.add("kupi-dugme");
                btnKupi.addEventListener("click", () => {
                    this.handleKupovina(auto.markaNaziv, auto.modelNaziv);
                });
        
                autoDiv.appendChild(infoDiv);
                autoDiv.appendChild(btnKupi);
                rezultatiContainer.appendChild(autoDiv);
            }
        });
    }
    
     async handleKupovina(marka, model) {
        const result = await Automobil.naruciAutomobil(marka, model);
        if (result) {
            alert(result);
            this.handleSearch();
        }
     }          
}