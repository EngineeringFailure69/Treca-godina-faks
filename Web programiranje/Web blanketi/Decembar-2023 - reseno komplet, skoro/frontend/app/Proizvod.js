export class Proizvod{
    constructor(prodavnicaId){
        this.prodavnicaId=prodavnicaId;

        this.polja = [
            {naziv: "Naziv:", klasa: "naziv"},
            {naziv: "Kategorija:", klasa: "kategorija"},
            {naziv: "Cena:", klasa: "cena"},
            {naziv: "Kolicina:", klasa: "kolicina"}
        ];

        this.color = this.getRandomColor(); 
    }
    
    getRandomColor() {
        // Funkcija za generisanje nasumične boje
        const letters = '0123456789ABCDEF';
        let color = '#';
        for (let i = 0; i < 6; i++) {
            color += letters[Math.floor(Math.random() * 16)];
        }
        return color;
    }

    async draw(container) {
        this.polja.forEach(p => {
            let lbl = document.createElement("label");
            lbl.classList.add("margin-10");
            lbl.innerHTML = p.naziv;
            container.appendChild(lbl);

            let element;
            if (p.klasa === "kategorija") {
                element = document.createElement("select");
            } else {
                element = document.createElement("input");
                element.type = p.klasa === "cena" || p.klasa === "kolicina" ? "number" : "text";
            }

            element.classList.add(`input-${p.klasa}`, "margin-10");
            container.appendChild(element);
        });

        const btnDodaj = document.createElement("input");
        btnDodaj.type = "button";
        btnDodaj.value = "Dodaj proizvod";
        btnDodaj.classList.add("dodaj-dugme");
        btnDodaj.addEventListener("click", this.dodajProizvod.bind(this));
        container.appendChild(btnDodaj);

        await this.fetchKategorije().then(kategorije => {
            const dropdown = container.querySelector('.input-kategorija');
            kategorije.forEach(kategorija => {
                const option = document.createElement('option');
                option.value = kategorija.kategorija;
                option.textContent = kategorija.kategorija;
                dropdown.appendChild(option);
            });
        });
    }

    async fetchKategorije(){
        try{
            const response = await fetch("https://localhost:7137/VratiSveKategorije");
            const responseData = await response.json();
            return responseData;
        }
        catch(error){
            console.log("Greska pri preuzimanju kategorija: ", error);
            return [];
        }
    }

    async dodajProizvod(){
        const naziv = document.querySelector('.input-naziv').value;
        const kategorija = document.querySelector('.input-kategorija').value;
        const cena = document.querySelector('.input-cena').value;
        const kolicina = document.querySelector('.input-kolicina').value;

        if(!naziv || !kategorija || !cena || !kolicina){
            alert("Sva polja moraju biti popunjena");
            return;
        }

        const proizvod = {
            naziv: naziv,
            kategorija: kategorija,
            cena: cena,
            kolicina: kolicina
        };

        try{
            let response = await fetch('https://localhost:7137/DodajProizvod' ,  {
                method: 'POST',
                headers:{
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(proizvod)
            });

            if(!response.ok){
                throw new Error('Greska pri dodavanju proizvoda');
            }

            const noviProizvod = await response.json();
            console.log(noviProizvod);

            response = await fetch(`https://localhost:7137/DodajProizvodUProdavnicu/${noviProizvod}/${this.prodavnicaId}/${kolicina}` , {
                method: 'POST'
            });

            if(!response.ok){
                throw new Error('Greska pri dodavanju proizvoda u prodavnicu');
            }

            alert('Proizvod je uspešno dodat u prodavnicu!');

        } catch (error) {
            console.error('Greška:', error);
            alert('Došlo je do greške prilikom dodavanja proizvoda.');
        }
    } 
}