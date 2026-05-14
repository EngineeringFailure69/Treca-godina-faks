#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd (1, 4, 5, 6, 7, 8);

int d1 = 1, d2 = 2; //razmak izmedju merenja u sekundama
int n1 = 4, n2 = 8; //broj merenja
int sensorValue = 0;
float temp = 0;
int sensorPin = A0;
int measureCounter = 0;
int sumMeasure = 0;
double avgMeasure = 0;
byte dButtonPin = 2, nButtonPin = 3;
int currentDValue = d1, currentNValue = n1,  memoryDCounter = 0, memoryNCounter = 2; 

volatile SemaphoreHandle_t measureTask, calculateAvgTask; //hendleri za semafore

void setup()  
 { 
   lcd.begin(16, 2);
   pinMode(sensorPin, INPUT);
   pinMode(dButtonPin, INPUT);
   pinMode(nButtonPin, INPUT);
   
   EEPROM.write(0, d1); //(adresa_memorije_na_koju_pisem, podatak_koji_pisem)
   EEPROM.write(1, d2);
   EEPROM.write(2, n1);
   EEPROM.write(3, n2);
   
   attachInterrupt(digitalPinToInterrupt(dButtonPin), changeDValue, RISING);
   attachInterrupt(digitalPinToInterrupt(nButtonPin), changeNValue, RISING);
   
   xTaskCreate(measurementTask, "Do all measurements", 128, NULL, 1, NULL); //kreiram task - dokumentacija
   xTaskCreate(calculateTask, "Do temperature calculations", 128, NULL, 1, NULL);
   
   measureTask = xSemaphoreCreateBinary();
   calculateAvgTask = xSemaphoreCreateBinary();
   
   xSemaphoreGive(measureTask); //predajem semafor prvom tasku 
 }

void loop()
 {    
 }
 
 //Taskovi
 
 void measurementTask(void* args)
 {
   for(;;)
   {
      if(xSemaphoreTake(measureTask, portMAX_DELAY))
      {
	 if(currentNValue > 20)
	    currentNValue = 20;
	 while(measureCounter <= currentNValue)
	 {
	    if(measureCounter == currentNValue)
	    {
	       vTaskDelay(1000 / portTICK_PERIOD_MS); //cekam jos jednu sekundu nakon poslednjeg merenja, pa predajem semafor drugom tasku
	       //vTaskDelay(pdMS_TO_TICKS(1000));
	       xSemaphoreGive(calculateAvgTask); //oslobadjam semafor i predajem ga drugom tasku
	    }
	    lcd.clear();
	    lcd.print("Merenje");
	    vTaskDelay(currentDValue*1000 / portTICK_PERIOD_MS);  // delay od d sekundi nakon svakog merenja, nisam bio siguran da
	    //li se prvo merenje vrsi odmah, ili nakon d sekundi, pa sam ga stavio da se vrsi nakon d sekundi
	    //vTaskDelay(pdMS_TO_TICKS(currentDValue*1000));
	    sensorValue = analogRead(sensorPin);
	    temp = (double)sensorValue / 1024;
	    temp = temp * 5;
	    temp = temp - 0.5;
	    temp = temp * 100;
	    sumMeasure += temp;
	    measureCounter++;
	}
      }
   }
 }
 
 void calculateTask(void* args)
 {
   for(;;)
   {
      if(xSemaphoreTake(calculateAvgTask, portMAX_DELAY))
      {
	 avgMeasure = (double)sumMeasure / currentNValue;//measureCounter;
	 lcd.clear();
	 lcd.print(avgMeasure);
	 vTaskDelay(2000 / portTICK_PERIOD_MS); //Odlazem predaju za 2 sekunde
	 //vTaskDelayUntil(pdMS_TO_TICKS(2000));
	 measureCounter = 0;
	 sumMeasure = 0;
	 xSemaphoreGive(measureTask);
      }
   }
 }
 
 //Interrupt-ovi
 
 void changeDValue()
 {
    if(memoryDCounter>=1)
      memoryDCounter = 0;
   else
      memoryDCounter++;

   currentDValue = EEPROM.read(memoryDCounter); //citam iz EEPROM sa adrese memoryDCounter
 }
 
 void changeNValue()
 {
   if(memoryNCounter>=3)
      memoryNCounter = 2;
   else
      memoryNCounter++;
   
   currentNValue = EEPROM.read(memoryNCounter);
   if(currentNValue > 20)
      currentNValue = 20;
 }