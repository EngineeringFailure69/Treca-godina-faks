import { Ispit } from "./Ispit.js";
import { Rok } from "./Rok.js";

export class Application{
    constructor(ispitiFetched, rokoviFetched){
        this.polja=[
            { naziv: "Ispit:", klasa: "ispit" },
            { naziv: "Rok:", klasa: "rok"}
        ];

        this.polja2=[
            { naziv: "Indeks: ", klasa: "indeks"},
            { naziv: "Ocena: ", klasa: "ocena"}
        ];

        this.headers=[
            { tekst: "Indeks" }, 
            { tekst: "Ime" },
            { tekst: "Prezime" },
            { tekst: "Predmet" },
            { tekst: "Ispitni rok" },
            { tekst: "Ocena" },
        ];

        this.ispiti = ispitiFetched.map(p=>{
            console.log(p);
            return new Ispit(p.id, p.naziv);
        });

        this.rokovi = rokoviFetched.map(p=>{
            console.log(p);
            return new Rok(p.id, p.naziv);
        });
    }

    draw(container){
        const bodyContainer = document.createElement("div");
        bodyContainer.classList.add("bodyContainer");

        const forma = document.createElement("div");
        forma.classList.add("kriterijumiPretrage");
        this.drawForma(forma);
        bodyContainer.appendChild(forma);

        const tabela = document.createElement("div");
        tabela.classList.add("tabela");
        bodyContainer.appendChild(tabela);

        container.appendChild(bodyContainer);
    }

    drawForma(container){
        this.polja.forEach(p=>{
            let lbl = document.createElement("label");
            lbl.classList.add("margin-10");
            lbl.innerHTML = p.naziv;
            container.appendChild(lbl);

            let izbor;
            if(p.klasa === "ispit"){
                izbor = document.createElement("select");
                izbor.classList.add("margin-10", `izbor-${p.klasa}`);

                this.ispiti.forEach(ispit=>{
                    const option = document.createElement('option');
                    option.value = ispit.id;
                    option.textContent = ispit.naziv;
                    izbor.appendChild(option);
                });
            }
            else if(p.klasa === "rok"){
                izbor = document.createElement("div");
                izbor.classList.add("rok-checkboxes");
    
                this.rokovi.forEach(rok => {
                    const checkboxLabel = document.createElement("label");
                    checkboxLabel.innerHTML = rok.naziv;
                    checkboxLabel.classList.add("rok-label");
    
                    const checkbox = document.createElement("input");
                    checkbox.type = "checkbox";
                    checkbox.value = rok.id;
                    checkbox.classList.add("margin-10", "checkbox-rok");
    
                    checkboxLabel.appendChild(checkbox);
                    //checkboxLabel.appendChild(document.createTextNode(rok.naziv));
                    izbor.appendChild(checkboxLabel);
                });
            }
            container.appendChild(izbor);
        });

        const btnNadji = document.createElement("input");
        btnNadji.type = "button";
        btnNadji.value = "Nadji";
        btnNadji.classList.add("nadji-dugme");
        btnNadji.addEventListener("click", this.handleSearch.bind(this));
        container.appendChild(btnNadji);

        const unosDiv = document.createElement("div");
        unosDiv.classList.add("unosDiv");

        this.polja2.forEach(p=>{
            let lbl = document.createElement("label");
            lbl.classList.add("margin-10");
            lbl.innerHTML = p.naziv;
            unosDiv.appendChild(lbl);

            let unos;
            if(p.klasa === "indeks"){
                unos = document.createElement("input");
                unos.type = "number";
                unos.min = "10000";
                unos.max = "20000";
                unos.classList.add(`unos-${p.klasa}`);
            }
            else{
                unos = document.createElement("input");
                unos.type = "number";
                unos.min = "6";
                unos.max = "10";
                unos.classList.add(`unos-${p.klasa}`);
            }
            unosDiv.appendChild(unos);
        });

        container.appendChild(unosDiv);

        const btnUpisi = document.createElement("input");
        btnUpisi.type = "button";
        btnUpisi.classList.add("upisi-dugme");
        btnUpisi.value = "Upisi";
        btnUpisi.addEventListener("click", this.upisi.bind(this));
        container.appendChild(btnUpisi);
    }

    async handleSearch(){
        const ispitId = document.querySelector('.izbor-ispit').value;
        const rokId = document.querySelectorAll('.checkbox-rok');

        const izabraniRokovi = [];
    
        rokId.forEach(checkbox => {
            if (checkbox.checked) {
                izabraniRokovi.push(checkbox.value);
            }
        });
        console.log("Ispit ID: ", ispitId);
        console.log("Rokovi ID: ", izabraniRokovi);
        const queryString = izabraniRokovi.map(rokId => `idRokova=${rokId}`).join('&');
        try{
            const responseData = await fetch(`https://localhost:7137/PreuzmiInformacijeOStudentima/${ispitId}?${queryString}`).then((response) => response.json());
            console.log(responseData);
            this.drawRezultati(responseData);
        }
        catch(error){
            console.log("Greska prilikom preuzimanja ID ispita: ", error);
            return [];
        }
    }

    drawRezultati(responseData){
        const tabela = document.querySelector('.tabela');
        tabela.innerHTML = '';

        const table = document.createElement("table");
        table.classList.add("rezultati-tabela");

        const headerRow = document.createElement("tr");
        this.headers.forEach(header => {
            const th = document.createElement("th");
            th.textContent = header.tekst;
            headerRow.appendChild(th);
        });

        table.appendChild(headerRow);

        responseData.forEach(data => {
            const row = document.createElement("tr");

            const cells = [
                data.studentIndeks,
                data.studentIme, 
                data.studentPrezime,
                data.ispitNaziv,
                data.rokNaziv,
                data.ocena
            ];

            cells.forEach(cellText =>{
                const td = document.createElement("td");
                td.textContent = cellText;
                row.appendChild(td);
            });

            table.appendChild(row);
        });
        const tabelaContainer = document.querySelector(".tabela");
        tabelaContainer.appendChild(table);
    }
    
    async upisi() {
        const ispitId = document.querySelector('.izbor-ispit').value;
        const rokIdElements = document.querySelectorAll('.checkbox-rok');
        const indeks = document.querySelector('.unos-indeks').value;
        const ocena = document.querySelector('.unos-ocena').value;
        
        // Filtriramo samo označene checkbox-ove
        const checkedCheckboxes = Array.from(rokIdElements).filter(checkbox => checkbox.checked);
        
        if (checkedCheckboxes.length > 1) {
            alert("Morate izabrati samo jedan ispitni rok prilikom upisa");
            // Poništavamo sve označene checkbox-ove
            checkedCheckboxes.forEach(checkbox => checkbox.checked = false);
        } else if (checkedCheckboxes.length === 1) {
            // Ako je tačno jedan checkbox označen, uzimamo njegov ID
            const selectedRokId = checkedCheckboxes[0].value;
            console.log("Rok ID: ", selectedRokId);
            
            try {
                // Pozivamo backend API za dodavanje položenog ispita
                const response = await fetch(`https://localhost:7137/DodajPolozeniIspit/${indeks}/${ispitId}/${selectedRokId}/${ocena}`, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                });
                
                const result = await response.text(); // Pretpostavljamo da API vraća tekstualnu poruku
                
                if (response.ok) {
                    alert(result); // Prikazujemo poruku o uspehu
                    
                    // Nakon uspešnog upisa, preuzimamo informacije o studentu i prikazujemo ih
                    const responseData = await fetch(`https://localhost:7137/VratiInformacijeOStudentu/${indeks}`).then(response => response.json());
                    console.log(responseData);
                    this.drawRezultati(responseData); // Ispravno ime funkcije
                } else {
                    alert(`Greška: ${result}`); // Prikazujemo grešku
                }
            } catch (error) {
                console.error('Došlo je do greške:', error);
                alert('Došlo je do greške prilikom upisa.');
            }
        } else {
            alert('Nijedan ispitni rok nije izabran.');
        }
    }    
}