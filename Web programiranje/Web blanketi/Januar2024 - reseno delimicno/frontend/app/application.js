import { Korisnik } from "./Korisnik.js";

export class Application{
    constructor(korisnickaFetched){
        this.polja = [
            {naziv: "Soba: ", klasa: "soba"},
            {naziv: "Korisnik:", klasa: "korisnik"},
            {naziv: "Nadimak:", klasa: "nadimak"},
            {naziv: "Boja: ", klasa: "boja"}
        ];

        this.korisnickaImena = korisnickaFetched.map(p=>{
            console.log(p);
            return new Korisnik(p.id, p.korisnicko);
        });
    }

    draw(container){
        const bodyContainer = document.createElement("div");
        bodyContainer.classList.add("bodyContainer");

        const forma = document.createElement("div");
        forma.classList.add("formaZaUnos");
        this.drawForma(forma);
        bodyContainer.appendChild(forma);

        const prikaz = document.createElement("div");
        prikaz.classList.add("prikazSoba");
        this.drawSobe(prikaz);
        bodyContainer.appendChild(prikaz);

        container.appendChild(bodyContainer);
    }

    drawForma(container){
        this.polja.forEach(p => {
            let lbl = document.createElement("label");
            lbl.innerHTML = p.naziv;
            lbl.classList.add("margin-10");
            container.appendChild(lbl);

            let unos;
            if(p.klasa === "korisnik") {
                unos = document.createElement("select");
                unos.classList.add("margin-10", `input-${p.klasa}`);

                this.korisnickaImena.forEach(korisnicko => {
                    const option = document.createElement('option');
                    option.value = korisnicko.id;
                    option.textContent = korisnicko.korisnicko;
                    unos.appendChild(option);
                });
            }
            else if(p.klasa === "boja"){
                unos = document.createElement("input");
                unos.type = "color";
                unos.classList.add("margin-10", `input-${p.klasa}`);
            }
            else{
                unos = document.createElement("input");
                unos.classList.add("margin-10", `input-${p.klasa}`);
            }
            container.appendChild(unos);
        });

        const btnDodaj = document.createElement("input");
        btnDodaj.type = "button";
        btnDodaj.value = "Dodaj";
        btnDodaj.classList.add("dodaj-dugme");
        container.appendChild(btnDodaj);
    }

    async drawSobe(container) {
        try {
            const response = await fetch("https://localhost:7137/VratiPotrebneInformacije");
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            const sobe = await response.json();
            console.log('Sobe:', sobe);
    
            // Proveri da li je sobe definisan i da li je niz
            if (!Array.isArray(sobe)) {
                throw new Error('Podaci o sobama nisu u ispravnom formatu');
            }
    
            // Sada prođi kroz sobe i iscrtaj ih
            sobe.forEach(soba => {
                // Proveri da li soba ima potrebne ključeve
                if (!soba.soba || !Array.isArray(soba.clanovi)) {
                    throw new Error('Podaci o sobi nisu u ispravnom formatu');
                }
    
                // Kreiraj element za prikaz sobe
                const sobaDiv = document.createElement("div");
                sobaDiv.classList.add("soba-container");
    
                // Naslov sobe
                const sobaNaslov = document.createElement("h3");
                sobaNaslov.classList.add("soba-naslov");
                sobaNaslov.textContent = `${soba.soba}`;
                sobaDiv.appendChild(sobaNaslov);
    
                // Naslov za članove
                const clanoviNaslov = document.createElement("p");
                clanoviNaslov.textContent = "Članovi:";
                sobaDiv.appendChild(clanoviNaslov);
    
                // Kreiraj listu korisnika za ovu sobu
                const korisniciList = document.createElement("ul");
                sobaDiv.appendChild(korisniciList);
    
                // Dodaj svakog korisnika u listu
                soba.clanovi.forEach(korisnik => {
                    if (!korisnik.ime || !korisnik.korisnicko) {
                        throw new Error('Podaci o korisniku nisu u ispravnom formatu');
                    }
    
                    const korisnikItem = document.createElement("li");
                    korisnikItem.textContent = `${korisnik.ime} (${korisnik.korisnicko})`;
                    korisnikItem.style.color = this.getRandomColor();
                    korisniciList.appendChild(korisnikItem);
                });

                const btnPrebrojJedinstvene = document.createElement("input");
                btnPrebrojJedinstvene.type = "button";
                btnPrebrojJedinstvene.value = "Prebroj jedinstvene";
                btnPrebrojJedinstvene.classList.add("dugme-prebroj-jedinstvene");
                sobaDiv.appendChild(btnPrebrojJedinstvene);
    
                // Dodaj sve u prikaz
                container.appendChild(sobaDiv);
            });
        } catch (error) {
            console.error("Greška prilikom preuzimanja soba:", error);
        }
    }

    getRandomColor() {
        const letters = '0123456789ABCDEF';
        let color = '#';
        for (let i = 0; i < 6; i++) {
            color += letters[Math.floor(Math.random() * 16)];
        }
        return color;
    } 
}