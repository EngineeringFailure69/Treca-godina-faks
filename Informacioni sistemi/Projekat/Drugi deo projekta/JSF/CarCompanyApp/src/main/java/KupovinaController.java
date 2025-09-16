import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.ejb.EJB;
import javax.faces.bean.ManagedBean;

@ManagedBean
public class KupovinaController {
	
	private String kupac="";
	private String auto="";
	private String datum="";
	private String bid="";
	private String aid="";
	
	@EJB
	private KupovinaService service;
	
	public String getKupac() 
	{
		return kupac;
	}
	public void setKupac(String idkupca) 
	{
		this.kupac=idkupca;
	}
	
	public String getAuto() 
	{
		return auto;
	}
	public void setAuto(String idauta) 
	{
		this.auto=idauta;
	}
	
	public String getDatum() 
	{
		return datum;
	}
	public void setDatum(String datum_kupovine) 
	{
		this.datum=datum_kupovine;
	}
	
	public String getBid() 
	{
		return bid;
	}
	public void setBid(String bid) 
	{
		this.bid=bid;
	}
	
	public String getAid() 
	{
		return aid;
	}
	public void setAid(String aid) 
	{
		this.aid=aid;
	}
	
    public void brisiKupovina(String bid){
		service.deleteKupovina(bid);
	}
    
    public void dodajKupovinu(String kupac, String auto, String datum) {
		service.createKupovina(kupac, auto, datum);
	}
        public void azurirajKupovina(String aid, String kupac,String auto, String datum) {
		service.updateKupovina(aid, kupac, auto, datum);
	}
        

        public List<Kupovina> sveKupovine(){
    		List<Kupovina> kupovine=service.spisakKupovina();
    		return kupovine;
    	}
}
