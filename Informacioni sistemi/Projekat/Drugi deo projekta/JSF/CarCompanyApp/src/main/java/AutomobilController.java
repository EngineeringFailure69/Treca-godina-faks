import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.ejb.EJB;
import javax.faces.bean.ManagedBean;

@ManagedBean
public class AutomobilController {

	private String marka="";
	private String model="";
	private String godiste="";
	private String cena="";
	private String bid="";
    private String aid="";   
    private String ckid="";
    
    @EJB
    private AutomobilService service;
    
    public  String getMarka() {
	return marka;
}
    public void setMarka(String marka) {
	this.marka=marka;
    }
    
    public  String getModel() {
		return model;
	}
        public void setModel(String model) {
		this.model=model;
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
        
        
        public  String getGodiste() {
		return godiste;
	}
        public void setGodiste(String godiste) {
		this.godiste=godiste;
	}
        
        public  String getCena() {
    		return cena;
    	}
            public void setCena(String cena) {
    		this.cena=cena;
    	}
        
            public void brisiAutomobil(String bid){
        		service.deleteCar(bid);
        	}
            
                
        	
        	public void dodajAutomobil(String marka, String model, String godiste, String cena) {
        		service.createCar(marka, model, godiste, cena);
        	}
                public void azurirajAutomobil(String aid, String cena) {
        		service.updatePrice(aid, cena);
        	}
                

                public List<Automobil> sviAutomobili(){
            		List<Automobil> automobili=service.spisakAutomobila();
            		return automobili;
            	}
}
