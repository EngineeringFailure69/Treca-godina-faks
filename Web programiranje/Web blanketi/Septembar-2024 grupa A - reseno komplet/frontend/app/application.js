import { Stan } from "./Stan.js";
//import { Stan } from "./Stan.js";

export class Application{
    constructor(IdFetched){
        this.polja=[
            {naziv: "Biraj stan", klasa: "birajStan"}
        ];

        this.IDs = IdFetched.map(p =>{
            console.log(p);
            return new Stan(p.id);
        });
    }

    draw(container){
        const bodyContainer = document.createElement("div");
        bodyContainer.classList.add("bodyContainer");

        const KriterijumiForma = document.createElement("div");
        KriterijumiForma.classList.add("KriterijumiForma");

        const kriterijumiPretrage = document.createElement("div");
        kriterijumiPretrage.classList.add("kriterijumiPretrage");
        this.drawForma(kriterijumiPretrage);
       KriterijumiForma.appendChild(kriterijumiPretrage);

        const rezultatiPretrageForma = document.createElement("div");
        rezultatiPretrageForma.classList.add("rezultatiPretrageForma");
        KriterijumiForma.appendChild(rezultatiPretrageForma);

        const rezultatiPretrageElementi = document.createElement("div");
        rezultatiPretrageElementi.classList.add("rezultatiPretrageElementi");
        KriterijumiForma.appendChild(rezultatiPretrageElementi);
        bodyContainer.appendChild(KriterijumiForma);
        bodyContainer.appendChild(rezultatiPretrageElementi);

        container.appendChild(bodyContainer);
    }

    async drawForma(container){
        const div = document.createElement("div");
        div.classList.add("divZaLabeluIDd");

        this.polja.forEach(p => {
            let lbl = document.createElement("label");
            lbl.innerHTML = `${p.naziv}:`;
            lbl.classList.add("margin-10");
            div.appendChild(lbl);

            let dropdown = document.createElement("select");
            dropdown.classList.add(`dropdown-${p.klasa}`, "margin-10");
            div.appendChild(dropdown);
        });

        container.appendChild(div);

        const btnPretrazi = document.createElement("input");
        btnPretrazi.type="button";
        btnPretrazi.value="Prikaz informacija";
        btnPretrazi.classList.add("btnPretrazi");
        btnPretrazi.addEventListener("click", this.handleSearch.bind(this));
        container.appendChild(btnPretrazi);

        await this.fetchIDStanova().then(stanoviID => {
            const dropdown = container.querySelector('.dropdown-birajStan');
            stanoviID.forEach(stanID =>{
                const option = document.createElement('option');
                option.value = stanID.id;
                option.textContent = stanID.id;
                dropdown.appendChild(option);
            });
        });
    }

    async fetchIDStanova(){
        try{
            const response = await fetch("https://localhost:7080/VratiSveID")
            const responseData = await response.json();
            console.log(responseData);
            return responseData;
        }
        catch(error){
            console.log("Greska prilikom preuzimanja ID stanova: ", error);
            return [];
        }
    }

    async handleSearch(){
        const stanId = document.querySelector('.dropdown-birajStan').value;
        console.log("ID: ", stanId);
        try{
            const responseData = await fetch(`https://localhost:7080/PodaciOStanu/${stanId}`).then((response)=>response.json());
            const responseData2 = await fetch(`https://localhost:7080/VratiInfoOStanu/${stanId}`).then((response) => response.json()); 
            console.log(responseData);
            console.log(responseData2);
            this.drawRezultati(responseData);
            this.drawRezultatiElementi(responseData2);
        }
        catch(error){
            console.log("Greska prilikom preuzimanja ID stanova: ", error);
            return [];
        }
    }

    drawRezultati(responseData){
        console.log(responseData);
        const rezultatiContainer = document.querySelector('.rezultatiPretrageForma');
        rezultatiContainer.innerHTML='';

            const stanDiv = document.createElement("div");
            stanDiv.classList.add("stanDiv");

            const infoDiv = document.createElement("div");
            infoDiv.classList.add("infoDiv");

            const brojStana = document.createElement("p");
            brojStana.innerHTML = `Broj stana: ${responseData.id}`;
            brojStana.classList.add("labela1");
            infoDiv.appendChild(brojStana);

            const imeVlasnika = document.createElement("label");
            imeVlasnika.innerHTML = `Ime vlasnika: ${responseData.imeVlasnika}`;
            imeVlasnika.classList.add("labela1");
            infoDiv.appendChild(imeVlasnika);

            const povrsina = document.createElement("label");
            povrsina.innerHTML = `Povrsina (m^2): ${responseData.povrsina}`;
            povrsina.classList.add("labela1");
            infoDiv.appendChild(povrsina);

            const BrojClanova = document.createElement("label");
            BrojClanova.innerHTML = `Broj clanova: ${responseData.brClanova}`;
            BrojClanova.classList.add("labela1");
            infoDiv.appendChild(BrojClanova);

            const btnIzracunaj = document.createElement("input");
            btnIzracunaj.type="button";
            btnIzracunaj.classList.add("btnIzracunaj");
            btnIzracunaj.value = "Izracunaj ukupno zaduzenje";
            btnIzracunaj.addEventListener("click", this.Izracunaj);
            infoDiv.appendChild(btnIzracunaj);

            stanDiv.appendChild(infoDiv);
            rezultatiContainer.appendChild(stanDiv);
    }

    drawRezultatiElementi(responseData2){
        const rezultatiContainer = document.querySelector('.rezultatiPretrageElementi');
        rezultatiContainer.innerHTML='';

        responseData2.forEach(data => {

            console.log(data);

            const stanDiv = document.createElement("div");
            stanDiv.classList.add("stanDiv2");

            const infoDiv = document.createElement("div");
            infoDiv.classList.add("infoDiv2");

            const Mesec = document.createElement("p");
            Mesec.innerHTML = `Mesec: ${data.mesec}`;
            Mesec.classList.add("labela2");
            infoDiv.appendChild(Mesec);

            const Voda = document.createElement("label");
            Voda.innerHTML = `Voda: ${data.voda}`;
            Voda.classList.add("labela2");
            infoDiv.appendChild(Voda);

            const struja = document.createElement("label");
            struja.innerHTML = `Struja: ${data.struja}`;
            struja.classList.add("labela2");
            infoDiv.appendChild(struja);

            const Usluge = document.createElement("label");
            Usluge.innerHTML = `Komunalne usluge: ${data.komunalneUsluge}`;
            Usluge.classList.add("labela2");
            infoDiv.appendChild(Usluge);

            
            const Placen = document.createElement("label");
            Placen.innerHTML = `Placen: ${data.placen}`;
            Placen.classList.add("labela2");
            if(data.placen === "Da"){
                infoDiv.classList.add("labela2Da");
            }
            else{
                infoDiv.classList.add("labela2Ne");
            }

            infoDiv.appendChild(Placen);
            
            stanDiv.appendChild(infoDiv);
            rezultatiContainer.appendChild(stanDiv);
        });
    }

    async Izracunaj(){
        const stanId = document.querySelector('.dropdown-birajStan').value;

        console.log("ID: ", stanId);
        try{
            const responseData = await fetch(`https://localhost:7080/VratiTroskove/${stanId}`).then((response)=>response.json());
            console.log(responseData);
            const btn = document.querySelector('.btnIzracunaj').value=responseData;
        }
        catch(error){
            console.log("Greska prilikom preuzimanja ID stanova: ", error);
            return [];
        }
    }
}