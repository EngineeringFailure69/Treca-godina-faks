import { Application } from "./application.js";

const korisnici = await fetch("https://localhost:7137/VratiSvaKorisnickaImena").then((response)=>response.json());
//

const app = new Application(korisnici);
app.draw(document.body);
