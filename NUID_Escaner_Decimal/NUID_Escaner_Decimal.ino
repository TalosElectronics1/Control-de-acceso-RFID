#include <SPI.h>//Libreria para bus SPI
#include <MFRC522.h>//Libreria para el kit RFID
#include <Wire.h>//Libreria para bus I2C
#include <LiquidCrystal_I2C.h>//Libreria para pantalla LCD I2C

//Definir pines de conexion
#define SS_PIN 10
#define RST_PIN 9
#define Buzzer_pin 8
#define Cerradura 7

LiquidCrystal_I2C lcd(0x27,16,2);//crea una instancia de pantalla I2C

String NUID = "";//String NUID
char * usuario_1="18375222219";//NUID del tag
const int Tiempo = 3;//Tiempo que dura la cerradura abierta

MFRC522 rfid(SS_PIN, RST_PIN); // Crea una instancia MFRC522


void setup() 
{ 
  Serial.begin(9600);
  pinMode(Buzzer_pin, OUTPUT);//Define el pin buzzer como salida
  pinMode(Cerradura,OUTPUT);//Define el pin Cerradura como salida
  digitalWrite(Buzzer_pin,HIGH);//El buzzer con HIGH esta apagado
  digitalWrite(Cerradura,HIGH);//EL rele con HIGH se apaga
 
  lcd.init();//Inicializa la pantalla
  lcd.backlight();//Enciende la luz de fondo
  lcd.setCursor(0,0);//Ir a la cordenada 0,0
  lcd.print("   Control");//Escribe el mensaje
  lcd.setCursor(0,1);//Ir a la segunda fila
  lcd.print(" De Acceso V1.0");//Escribe el mensaje
  delay(500);//Espera 500ms

  SPI.begin(); // Inicializa el Bus SPI necesario para el RFID
  rfid.PCD_Init(); // Inicializa el sensor MFRC522 

}
 
void loop() 
{
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  
  for (byte i = 0; i < rfid.uid.size; i++) 
  {
    
    if (rfid.uid.uidByte[i] < 0x10) 
    {
       NUID=NUID + "0";
    }
    NUID = NUID + rfid.uid.uidByte[i];
  }
 
  delay(100);
  digitalWrite(Buzzer_pin,LOW);
  delay(100);
  digitalWrite(Buzzer_pin,HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tarjeta:");
  lcd.setCursor(0,1);
  lcd.print(NUID);
  Serial.print("NUID:");
  Serial.println(NUID);
  
  if(NUID==usuario_1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Acceso concedido");
    lcd.setCursor(0,1);
    lcd.print("Tiempo: ");
    digitalWrite(Cerradura,LOW);
    
    for(int i = 1; i <= Tiempo; i++)
    {
      /* code */
      lcd.setCursor(7,1);
      lcd.print(i);
      delay(1000);
    }
    lcd.clear();
    digitalWrite(Cerradura,HIGH);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Acceso denegado ");
    lcd.setCursor(0,1);
    lcd.print(NUID);
    delay(1000*Tiempo);
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Pase una nueva");
  lcd.setCursor(0,1);
  lcd.print("    Tarjeta");
  NUID = "";
 

}