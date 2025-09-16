import java.sql.Date;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import javax.ejb.Remote;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.TypedQuery;

@Remote(AutomobilService.class)
@Stateless
public class AutomobilServiceImpl implements AutomobilService {
	
	public AutomobilServiceImpl()
	{
		   EntityManagerFactory emf = Persistence
	               .createEntityManagerFactory("CarCompanyPU");
	       em = emf.createEntityManager();
	}
	
    private EntityManager em;
 
    public static void main(String[] args) {
 
    	AutomobilServiceImpl a =new AutomobilServiceImpl();
       // EntityManagerFactory emf = Persistence
         //       .createEntityManagerFactory("CarCompanyPU");
      // em = emf.createEntityManager();
        
        //a.createCar("V", "X", 2022, 26000f);
    	//a.updatePrice(9, 30000f);
    	//a.deleteCar(9);
        /*createCar("Volvo", "XC40", 2020, 20000f);
        createCar("Volvo", "XC60", 2021, 22000f);
        createCar("Volvo", "EX30", 2019, 19000f);*/
        //createCar("Volvo", "V60", 2018, 18000f);
        //a.listAllCars();
    }
    @Override
    public void createCar(String marka, String model, int godiste, float cena) {
        em.getTransaction().begin();
        Automobil emp = new Automobil(marka, model, godiste, cena);
        em.persist(emp);
        em.getTransaction().commit();
    }
    
    @Override
    public void deleteCar(int id) {
    	Automobil auto = em.find(Automobil.class, id);
    	em.getTransaction().begin();
    	em.remove(auto);
    	em.getTransaction().commit();
    }
    
    @Override
    public void updatePrice(int id, float cena) {
        Automobil auto = em.find(Automobil.class, id);
        em.getTransaction().begin();
        auto.setCena(cena);
        em.getTransaction().commit();
     }
    
    @Override
    public void listAllCars()
    {
      
        TypedQuery<Automobil> query = em.createQuery("SELECT e FROM Automobil e", Automobil.class);
        List<Automobil> automobilisvi = query.getResultList();
    
       for (Automobil e1: automobilisvi) {
    	   System.out.println("ID:"+e1.getId() + " Marka: "+e1.getMarka() + 
    			   " Model: "+e1.getModel() + " Godiste: " + e1.getGodiste() + " Cena: " + e1.getCena());
       }
      
   }
    
    @Override
    public String getAutomobil(String model) 
    {
    	
    	int f=0;
    	TypedQuery<Automobil> query = em.createQuery("SELECT e FROM Automobil e", Automobil.class);
        List<Automobil> automobilisvi = query.getResultList();
    	
        for (Automobil e1: automobilisvi) {
        	String model1=e1.getModel();
     	   if(model1.equals(model)) 
     	   {
     		  f=1;
     	   }
     	   else 
     	   {
     		   f=0;
     	   }
        }
        
        if(f==1)
        	return model;
        else
        	return null;
        
    	//Automobil automobil = em.find(Automobil.class, model);
	    //return automobi;
    	
    }
    
    @Override
    public void delCar(String model) 
    {
    	
    	int f=0;
    	TypedQuery<Automobil> query = em.createQuery("SELECT e FROM Automobil e", Automobil.class);
        List<Automobil> automobilisvi = query.getResultList();
    	
        for (Automobil e1: automobilisvi) {
        	String model1=e1.getModel();
     	   if(model1.equals(model)) 
     	   {
     		  f=1;
     	   }
     	   else 
     	   {
     		   f=0;
     	   }
        }
        
        if(f==1)
        {
        	for (Automobil e1: automobilisvi) {
            	String model1=e1.getModel();
         	   if(model1.equals(model)) 
         	   {
         			Automobil auto = em.find(Automobil.class, e1.getId());
                	em.getTransaction().begin();
                	em.remove(auto);
                	em.getTransaction().commit();
         	   }
        }
    }
}
}