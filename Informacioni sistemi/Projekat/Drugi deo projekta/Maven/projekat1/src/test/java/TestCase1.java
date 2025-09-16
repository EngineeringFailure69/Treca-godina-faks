import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

import java.util.List;

import javax.ejb.EJB;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

//Testiramo da li se automobili ispravno unose u bazu, u bazi ne smeju postojati dva automobila sa istim nazivom modela, jer za tim nema potrebe 
public class TestCase1 {
	
	//private EntityManager em;
	
	private static AutomobilServiceImpl service;
	
	@BeforeClass
	   public static void initTest()
	   {
		   service=new AutomobilServiceImpl();
		   System.out.println("BeforeClass: Uspesno inicijalizovan test");
	   }
		
	  //Preduslov:Ne postoji automobil istog modela	
	   @Before
	   public void testPrecondition()
	   {
		   String a1=service.getAutomobil("v60");
	       assertNull(a1);
		   System.out.println("Before: Preduslov");

	   }
	   
	   //Dodajemo auto modela v60
	   @Test
	   public void testAssertions() 
	   {
		   service.createCar("Volvo", "v60", 2018, 10000f);
		   System.out.println("Test:izvrsen");

	   }
	   
	   //Post-uslov:Postoji auto modela v60
	   @After
	   public void testPostcondition()
	   {
		   String a1=service.getAutomobil("v60");
	       assertNotNull(a1);
	       System.out.println("After: post-uslov");
	   }
	   
	   //Brisemo test podatke iz baze
	   @AfterClass
	   public static void clearTest()
	   {
		   service.delCar("v60");
		   System.out.println("AfterClass:obrisani test podaci");

	   }

}
