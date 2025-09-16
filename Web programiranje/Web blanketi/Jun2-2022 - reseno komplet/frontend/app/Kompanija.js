export class Kompanija{
    constructor(id, naziv, prosecnaZarada, cena, brojDanaZaIsporuku){
        this.id=id;
        this.naziv=naziv;
        this.prosecnaZarada=prosecnaZarada;
        this.cena=cena;
        this.brojDanaZaIsporuku=brojDanaZaIsporuku;
    }

    static async searchKompanijeVozila(zapremina, tezina, datumPrijema, datumDostave, cenaOd, cenaDo) {
        try {
            const response = await fetch(`https://localhost:7137/PronadjiKompanijuIVozilo/${zapremina}/${tezina}/${datumPrijema}/${datumDostave}/${cenaOd}/${cenaDo}`);
            if (!response.ok) {
                const errorText = await response.text();
                throw new Error(`Ne mogu da preuzmem rezultate pretrage: ${errorText}`);
            }
            const data = await response.json();
            console.log("Vraceni podaci: ", data);
            return Array.isArray(data) ? data : [];
        } catch (error) {
            console.error("Greška pri pretrazi vozila i firmi:", error);
            return [];
        }
    }

    draw(dostave) {
        const rezultatiContainer = document.querySelector('.rezultatiPretrage');
        console.log("Rezultati container:", rezultatiContainer); // Provera kontejnera

        if (!rezultatiContainer) {
            console.error("Element sa klasom 'rezultatiPretrage' nije pronađen.");
            return;
        }

        if (Array.isArray(dostave)) { // Proveri da li je dostave niz
            dostave.forEach(dostava => {
                console.log("Dostava objekat:", dostava); // Provera objekta

                const dostavaDiv = document.createElement("div");
                dostavaDiv.classList.add("dostava");

                const infoDiv = document.createElement("div");
                infoDiv.classList.add("dostava-info");

                // Proveri i pristupi svojstvima objekta
                const nazivKompanije = document.createElement("p");
                nazivKompanije.innerHTML = `Naziv: ${dostava.kompanija ? dostava.kompanija.naziv : 'N/A'}`;
                infoDiv.appendChild(nazivKompanije);

                const img = document.createElement("img");
                img.src = `https://localhost:7137${dostava.slikaPath}`;
                img.classList.add("slika-automobila");
                infoDiv.appendChild(img);

                const cenaDostave = document.createElement("p");
                cenaDostave.innerHTML = `Cena: ${dostava.kompanija ? dostava.kompanija.cena : 'N/A'} din`;
                infoDiv.appendChild(cenaDostave);

                const prosecnaZaradaKompanije = document.createElement("p");
                prosecnaZaradaKompanije.innerHTML = `Prosecna zarada: ${dostava.kompanija ? dostava.kompanija.prosecnaZarada : 'N/A'} din`;
                infoDiv.appendChild(prosecnaZaradaKompanije);

                const btnIsporuci = document.createElement("input");
                btnIsporuci.type = "button";
                btnIsporuci.value = "Isporuci";
                btnIsporuci.classList.add("isporuci-dugme");
                btnIsporuci.addEventListener("click", () => {
                    this.handleIsporuka(dostava.kompanija.id);
                });

                dostavaDiv.appendChild(infoDiv);
                dostavaDiv.appendChild(btnIsporuci);
                rezultatiContainer.appendChild(dostavaDiv);

                console.log("Dodao dostavaDiv:", dostavaDiv);
            });
        } else {
            console.error("Dostave nije niz:", dostave);
        }
    }

    async handleIsporuka(id) {
        console.log("Kliknuto dugme za isporuku, ID:", id);
        try{
            const response = await fetch(`https://localhost:7137/Isporuci/${id}` , {
                method: 'PUT',
            });

            if(!response.ok){
                const errorText = await response.text();
                throw new Error(`Greska pri isporuci: ${errorText}`)
            }
            return await response.text();
        }
        catch(error){
            console.error("Greska pri isporuci:", error);
            return null;
        }
    }
}