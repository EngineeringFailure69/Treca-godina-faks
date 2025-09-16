import { Application } from "./application.js";

const ispiti = await fetch("https://localhost:7137/PreuzmiIspite").then((response) => response.json());
const rokovi = await fetch("https://localhost:7137/PreuzmiRokove").then((response) => response.json());

const app = new Application(ispiti, rokovi);
app.draw(document.body);