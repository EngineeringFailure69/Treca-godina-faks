import { Application } from "./application.js";

const prodavnice = await fetch("https://localhost:7137/VratiSveProdavnice").then((response) => response.json());

const app = new Application(prodavnice);
app.draw(document.body);