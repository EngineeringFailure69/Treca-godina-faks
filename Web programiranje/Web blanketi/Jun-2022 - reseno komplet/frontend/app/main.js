import { Application } from "./application.js";

const marke = await fetch("https://localhost:7137/PreuzmiMarku").then((response)=>response.json());

const app = new Application(marke);
app.draw(document.body);