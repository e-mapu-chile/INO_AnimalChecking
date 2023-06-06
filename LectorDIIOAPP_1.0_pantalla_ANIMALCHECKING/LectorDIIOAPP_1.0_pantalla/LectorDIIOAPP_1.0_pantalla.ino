
#include <SoftwareSerial.h>
#include <Rfid134.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); // I2C / TWI
String diioNumero = "";

class RfidNotify
{
  public:
    static void OnError(Rfid134_Error errorCode)
    {
      // see Rfid134_Error for code meaning
      Serial.println();
      Serial.print("Com Error ");
      Serial.println(errorCode);
    }

    static void OnPacketRead(const Rfid134Reading& reading)
    {
      char temp[8];

      //Serial.print("DIIO: ");
      sprintf(temp, "%03u", reading.country);
      sprintf(temp, "%06lu", static_cast<uint32_t>(reading.id / 1000000));
      diioNumero = temp;
      sprintf(temp, "%06lu", static_cast<uint32_t>(reading.id % 1000000));
      diioNumero = diioNumero + temp;
      if (reading.isData)
      {
        //     Serial.print("data");
      }
      if (reading.isAnimal)
      {
        //    Serial.print("animal");
      }
      //Serial.println(diioNumero);
    }
};
SoftwareSerial hcSerial(2, 3); // RX, TX

String fromPC = "";
int luzRoja = 6;
int luzAmarilla = 7;
int luzVerde = 8;
int valorPulsador1 = 0;
int valorPulsador2 = 0;
int pulsadorPinLeer = 11;
int pulsadorPinReset = 10;

SoftwareSerial secondarySerial(4, 0); // RX, TX
Rfid134<SoftwareSerial, RfidNotify> rfid(secondarySerial);


void setup()
{
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  pinMode(8, OUTPUT);

  Serial.begin(9600);
  hcSerial.begin(9600);
  Serial.println("STARTING HC...");

  pinMode(luzRoja , OUTPUT);
  pinMode(luzAmarilla , OUTPUT);
  pinMode(luzVerde , OUTPUT);
  pinMode(pulsadorPinLeer, INPUT);
  pinMode(pulsadorPinReset, INPUT);

  Serial.println("initializing...");
  secondarySerial.begin(9600);
  rfid.begin();
  Serial.println("starting RFID...");
  miPrintLongTop("Bienvenido...Iniciando Baston", 30);
  delay(500);
  miPrint1Titulo("ANIMAL", 15, 22, "CHECKING", 5, 42, "M-A01 ", 17, 62);
  sonidoTitulo();
  delay(3000);



}

void sonidoTitulo() {

  for (int v = 0; v < 2; v++) {
    digitalWrite(luzVerde , HIGH);
    delay(300);
    digitalWrite(luzVerde , LOW);
    delay(100);
  }


}
void drawTitulo(String txt1, int x1, int y1, String txt2, int x2, int y2, String txt3,
                int x3, int y3) {
  // graphic commands to redraw the complete screen should be placed here

  //u8g.setFont(u8g_font_osb21);
  //u8g_font_fub17
  //u8g_font_helvR08
  u8g.setFont(u8g_font_fub17);
 // u8g.drawLine(0, 45, 130, 45);
  u8g.drawStr( x1, y1, txt1.c_str());
 // u8g.setFont(u8g_font_fub17);
  u8g.drawStr( x2, y2, txt2.c_str());
  u8g.drawStr( x3, y3, txt3.c_str());
}
void miPrint1Titulo(String txt1, int x1, int y1, String txt2, int x2, int y2, String txt3,
                    int x3, int y3) {
  u8g.firstPage();
  do {
    drawTitulo(txt1, x1, y1, txt2, x2, y2, txt3, x3, y3);
  } while ( u8g.nextPage() );
}

void draw(String txt1, int x1, int y1, String txt2, int x2, int y2, String txt3,
          int x3, int y3) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  //u8g_font_fub17
  //u8g_font_helvR08

  u8g.drawStr( x1, y1, txt1.c_str());

  u8g.drawStr( x2, y2, txt2.c_str());
  u8g.drawStr( x3, y3, txt3.c_str());
}
void drawTablero(String txt1, int x1, int y1, String txt2, int x2, int y2, String txt3,
                 int x3, int y3) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  u8g.drawFrame(0, 0, 120, 25);
  u8g.drawLine(80, 1, 80, 24);
  u8g.drawStr( x1, y1, txt1.c_str());
  //u8g.drawLine(0, 25, 130, 25);
  u8g.drawStr( x2, y2, txt2.c_str());
  u8g.drawStr( x3, y3, txt3.c_str());
}
void miPrint1(String txt1, int x1, int y1, String txt2, int x2, int y2, String txt3,
              int x3, int y3) {
  u8g.firstPage();
  do {
    draw(txt1, x1, y1, txt2, x2, y2, txt3, x3, y3);
  } while ( u8g.nextPage() );
}
void miPrint1T(String txt1, int x1, int y1, String txt2, int x2, int y2, String txt3,
               int x3, int y3) {
  u8g.firstPage();
  do {
    drawTablero(txt1, x1, y1, txt2, x2, y2, txt3, x3, y3);
  } while ( u8g.nextPage() );
}
void miPrintLongTop(String txt1, int velocidad ) {
  //16 ancho pantalla
  int cantidadCaracteres = txt1.length();
  for (int x = 0; x < cantidadCaracteres + 1; x++) {
    String r = txt1.substring(x, cantidadCaracteres);
    u8g.firstPage();
    do {
      draw(r, 0, 22, "", 0, 42, "", 0, 62);
    } while ( u8g.nextPage() );
    delay(velocidad);

  }
}

int flagConexion = 0;
int primeraVez = 1;

void loop()
{

  //Serial.println("serial => "+hcSerial);
  if (flagConexion == 0 && primeraVez == 1) {
    Serial.begin(9600);
    Serial.println("initializing...");
    secondarySerial.begin(9600);
    rfid.begin();
    Serial.println("starting RFID...");
    hcSerial.begin(9600);
    Serial.println("STARTING HC...");
    primeraVez = 0;
    miPrint1("Por favor", 0, 22, "conecte su", 0, 42, "celular Android", 0, 62);
  }
  if (flagConexion == 0 && primeraVez == 0) {
    // Read from HC-06
    String key = "";
    //  flagConexion = 1;
    //      digitalWrite(luzAmarilla , HIGH);
    if (hcSerial.available()) {
      while (hcSerial.available()) { // While there is more to be read, keep reading.
        //Serial.print((char)hcSerial.read());
        key = String(key + String((char)hcSerial.read()));
      }
      miPrint1("", 0, 22, "conectando...", 0, 42, "", 0, 62);
      // key = Serial.read();
      Serial.println("KEY => " + key);
      if (key == "eyJhbGc") {
        flagConexion = 1;
        digitalWrite(luzAmarilla , HIGH);
        miPrint1T(" Conexion  OK", 1, 18, "presione LEER", 0, 42, "desde su celular", 0, 62);
      } else {
        // flagConexion = 1;
        miPrint1("El dispositivo", 0, 22, "no cuenta con", 0, 42, "permisos", 0, 62);
        digitalWrite(luzAmarilla , HIGH);
        delay(1000);
        digitalWrite(luzAmarilla , LOW);
      }
    }
  }
  if (flagConexion == 1 && primeraVez == 0) {
    Serial.begin(9600);
    hcSerial.begin(9600);

    Serial.println("STARTING HC...");
    Serial.println("initializing...");
    secondarySerial.begin(9600);
    rfid.begin();
    Serial.println("starting RFID...");
    primeraVez = 2;

  }

  if (flagConexion == 1 && primeraVez == 2) {
    //****** SECTOR DE CODIGO RFID DIIO + BLUETOOTH
    //lee?
    valorPulsador1 = digitalRead(pulsadorPinLeer);
    println("pulsador "+valorPulsador1);

    // Encendemos o apagamos el pin del LED según convenga
    if (valorPulsador1 == HIGH) {
      miPrint1T(" Conexion  OK", 1, 18, "Leyendo...", 0, 42, "", 0, 62);
      rfid.loop();

      if (diioNumero != "") {
        //String numeroJ =  "-" + diioNumero + "-";
        String numeroJ =  "-" + diioNumero + "/n";
        digitalWrite(luzVerde , HIGH);
        delay(300);
        digitalWrite(luzVerde , LOW);
        writeString(numeroJ);
        hcSerial.print(numeroJ); // show the HC-06 responce

        miPrint1T(" Lectura    OK", 0, 18, "Numero DIIO", 0, 42, diioNumero, 0, 62);
        delay(1000);
        miPrint1T(" Conexion  OK", 1, 18, "Presione boton", 0, 42, "del baston", 0, 62);
        // Read from PC

        /* if (Serial.available()) {
           delay(10); //
           fromPC = (char)Serial.read();


           hcSerial.print(fromPC); // show the HC-06 responce
           Serial.print(fromPC); // echo it back to the PC

          }*/




        diioNumero = "";
      }

    }
    else {
      miPrint1T(" Conexion  OK", 1, 18, "Presione boton", 0, 42, "del baston", 0, 62);
    
    }

   /* valorPulsador2 = digitalRead(pulsadorPinReset);
    if (valorPulsador2 == HIGH) {
      primeraVez = 1;
      flagConexion = 0;
      miPrint1T(" Conexion  OUT", 1, 18, "Restaurando", 0, 42, "el baston", 0, 62);
      delay(1500);
    } else {
      miPrint1T(" Conexion  OK", 1, 18, "Presione boton", 0, 42, "del baston", 0, 62);
    }*/


  }


  digitalWrite(luzRoja , LOW);



}

void writeString(String stringData) { // Used to serially push out a String with Serial.write()

  for (int i = 0; i < stringData.length(); i++)
  {

    Serial.write(stringData[i]);   // Push each char 1 by 1 on each loop pass

  }

}// end writeString
