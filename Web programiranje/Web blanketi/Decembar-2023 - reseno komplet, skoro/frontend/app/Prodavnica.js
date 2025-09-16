import { Proizvod } from "./Proizvod.js";

export class Prodavnica{
    constructor(id, naziv){
        this.id=id;
        this.naziv=naziv;
        this.proizvodi = [];
    }

    async draw(container){
        const prodavnicaDiv = document.createElement("div");
        prodavnicaDiv.classList.add("prodavnicaDiv");

        const nazivDiv = document.createElement("h3");
        nazivDiv.classList.add("nazivDiv");
        nazivDiv.textContent = this.naziv;

        const forma = document.createElement("div");
        forma.classList.add("formaZaUnos");
        const proizvod = new Proizvod(this.id);
        proizvod.draw(forma);
        prodavnicaDiv.appendChild(forma);

        const informacije = document.createElement("div");
        informacije.classList.add("informacije");
        informacije.appendChild(nazivDiv);
        await this.loadProizvodi(informacije);

        prodavnicaDiv.appendChild(informacije);
        container.appendChild(prodavnicaDiv);
    }

    async loadProizvodi(container) {
        try {
            const response = await fetch(`https://localhost:7137/ProizvodUProdavnici/${this.id}`);
            const proizvodi = await response.json();
        
            proizvodi.forEach(proizvod => {
                const proizvodDiv = document.createElement("div");
                proizvodDiv.classList.add("proizvodDiv");
        
                const proizvodInfo = document.createElement("div");
                proizvodInfo.classList.add("proizvodInfo");
        
                const nazivDiv = document.createElement("span");
                nazivDiv.classList.add("proizvodNaziv");
                nazivDiv.textContent = `${proizvod.naziv}: ${proizvod.kolicina}`;
        
                proizvodInfo.appendChild(nazivDiv);
        
                const proizvodActions = document.createElement("div");
                proizvodActions.classList.add("proizvodActions");
        
                const proizvodProgress = document.createElement("div");
                proizvodProgress.classList.add("proizvodProgress");
        
                const progressBar = document.createElement("div");
                progressBar.classList.add("progress-bar");
                progressBar.style.width = `${proizvod.kolicina}%`;
                progressBar.style.backgroundColor = proizvod.color || '#00f'; // Postavi boju na osnovu `proizvod.color`
        
                proizvodProgress.appendChild(progressBar);

                const kolicinaInput = document.createElement("input");
                kolicinaInput.type = "number";
                kolicinaInput.min = "1";
                kolicinaInput.max = "100";
                kolicinaInput.classList.add("input-kolicina-updown");
    
                const btnProdaj = document.createElement("input");
                btnProdaj.type = "button";
                btnProdaj.classList.add("prodaj-dugme");
                btnProdaj.value = "Prodaj";
                btnProdaj.addEventListener("click", () => this.dodajKolicinu(proizvod.id, kolicinaInput.value, container));
        
                proizvodActions.appendChild(proizvodProgress);
                proizvodActions.appendChild(kolicinaInput);
                proizvodActions.appendChild(btnProdaj);
        
                proizvodDiv.appendChild(proizvodInfo);
                proizvodDiv.appendChild(proizvodActions);
        
                container.appendChild(proizvodDiv);
            });
        } catch (error) {
            console.error("Greška pri učitavanju proizvoda:", error);
        }
    }
    
    async dodajKolicinu(proizvodId, kolicina, container) {
        if (!kolicina || isNaN(kolicina) || kolicina <= 0) {
            alert("Unesite validnu količinu.");
            return;
        }

        try {
            const response = await fetch(`https://localhost:7137/ProdajProizvod/${this.id}/${proizvodId}/${kolicina}`, {
                method: 'PUT'
            });

            if (!response.ok) {
                throw new Error('Greška pri dodavanju količine.');
            }

            alert("Količina uspešno ažurirana!");
            // Opciono: Ponovno učitaj proizvode nakon ažuriranja
            container.innerHTML = '';
            await this.loadProizvodi(container);
        } catch (error) {
            console.error("Greška:", error);
            alert("Došlo je do greške prilikom ažuriranja količine.");
        }
    }
}