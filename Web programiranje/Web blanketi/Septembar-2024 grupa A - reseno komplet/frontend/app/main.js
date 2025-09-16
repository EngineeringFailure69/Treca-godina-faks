import { Application } from "./application.js";

const IDs = await fetch("https://localhost:7080/VratiSveID").then((response)=>response.json()).catch(error => {
    console.error('Error:', error);
  });

const app = new Application(IDs);
app.draw(document.body);