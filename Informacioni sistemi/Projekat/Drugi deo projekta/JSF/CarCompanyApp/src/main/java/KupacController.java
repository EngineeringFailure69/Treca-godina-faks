
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.ejb.EJB;
import javax.faces.bean.ManagedBean;


@ManagedBean
public class KupacController  {
    
     private String adresa="";
     private String prezime="";
     private String ime="";
     private String bid="";
     private String aid="";   
     private String ckid="";  
     private String racun="";
     
	@EJB
	private KupacService service;
	public  String getAdresa() {
		return adresa;
	}
        public void setAdresa(String adresa) {
		this.adresa=adresa;
        }
                
        public  String getRacun() {
		return racun;
	}
        public void setRacun(String stanje_racuna) {
		this.racun=stanje_racuna;
        }
           
        
        
        
        public  String getAid() {
		return aid;
	}
        public void setAid(String aid) {
		this.aid=aid;
	}
        
        
        public  String getCkid() {
    		return ckid;
    	}
            public void setCkid(String ckid) {
    		this.ckid=ckid;
    	}
        
        
                
        public  String getBid() {
		return bid;
	}
        public void setBid(String bid) {
		this.bid=bid;
	}
        
        
        public  String getIme() {
		return ime;
	}
        public void setIme(String ime) {
		this.ime=ime;
	}
        
        public  String getPrezime() {
    		return prezime;
    	}
            public void setPrezime(String prezime) {
    		this.prezime=prezime;
    	}
        
        
       public void brisiKupac(String bid){
		service.deleteKupac(bid);
	}
    
        
	
	public void dodajKupac(String ime, String prezime,String adresa, String stanje_racuna) {
		service.createKupac(ime, prezime, adresa, stanje_racuna);
	}
        public void azurirajKupac(String aid, String stanje_racuna) {
		service.updateBankAccount(aid, stanje_racuna);
	}
        

        public List<Kupac> sviKupci(){
    		List<Kupac> kupci=service.spisakKupaca();
    		return kupci;
    	}
    
/*	
    private Kupac kupac= new Kupac();
	@EJB
	private KupacService service;
	public  Kupac getKupac() {
		return kupac;
	}
	public void setKupac(Kupac kupac) {
		this.kupac = kupac;
	}
	public void saveKupac(Kupac k) {
		service.addKupac(k);
	}*/
}

