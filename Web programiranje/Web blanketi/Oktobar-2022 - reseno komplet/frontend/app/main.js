import { Application } from "./application.js";

const prodavnica = await fetch("https://localhost:7137/VratiProdavnicu").then((response)=>response.json());
const dimenzija = await fetch("https://localhost:7137/UzmiSveDimenzije").then((response)=>response.json());
const papir = await fetch("https://localhost:7137/VratiSvePapire").then((response)=>response.json());
//const materijal = await fetch("https://localhost:7137/UzmiRamPoDimenziji").then((response)=>response.json());

const app = new Application(prodavnica, dimenzija, papir);
app.draw(document.body);