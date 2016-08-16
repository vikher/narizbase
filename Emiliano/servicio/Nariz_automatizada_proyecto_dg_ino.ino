/********************************************************************
 * Automatización de la Nariz Electrónica a través de un
 * algoritmo de red neuronal previamente entrenada para la detección 
 * de Uva, Limón y Platano.
 *
 * Parametros de la red neuronal
 * Red Neuronal con:
 *    5 entradas, 
 *    5 neuronas en la capa oculta,
 *    3 neuronas en la capa de salida
 *    Función de transferencia: sigmoidal tanto para la capa oclta como
 *    para la capa de salida.
 *    Valores de Salida:
 *       [0 0 0] - Reposo
 *       [1 0 0] - Uva
 *       [0 1 0] - Limón
 *       [0 0 1] - Platano
 *    Autor: Jacob Avila Camacho
 ********************************************************************/

#include <math.h>
#include <LiquidCrystal.h>

/********************************************************************
 * Configuración de las entradas analógicas para los Sensores de Gas
 ********************************************************************/
int sensorH = A0;
int sensor5 = A1;
int sensor4 = A2;
int sensor3 = A3;
int sensor2 = A4;
int sensor1 = A5;

const int NodosEntrada = 5; 
const int NodosOcultos = 5; 
const int NodosSalida = 3;
 
float entradasOriginales[NodosEntrada];
float salida[NodosSalida];

/**************************************************************
 * LCD y Shield
 **************************************************************/
int boton1 = 10;
int ledRojo = 6;
LiquidCrystal lcd(12,11,5,4,3,2);

void setup(){
  Serial.begin(9600);
  pinMode(ledRojo, OUTPUT);
  pinMode(boton1, INPUT);
  lcd.begin(16, 2);
  lcd.print("Inicializando");
  digitalWrite(ledRojo, HIGH);
}

void loop(){
  float valoresEntrada[NodosEntrada] = {
      analogRead(sensor1),
      analogRead(sensor2),
      analogRead(sensor3),
      analogRead(sensor4),
      analogRead(sensor5)
  };
  for(int i = 0; i < NodosEntrada; i++){
    entradasOriginales[i] = valoresEntrada[i];
  }
  
  RedNeuronal(valoresEntrada, salida);
  imprimeSalida();
  delay(300);
}

/*************************************************************
 * Funcion para mostrar resultados tanto en el puerto serial
 * como en el LCD
 ************************************************************/
void imprimeSalida(){
  String cadenaSalida = "[";
  String cadenaDisplay = "";
  for(int i=0; i < NodosEntrada; i++){
    cadenaSalida += (int)entradasOriginales[i];
    cadenaSalida += ", ";
    cadenaDisplay += (int)entradasOriginales[i];
    cadenaDisplay += ",";
  }
  cadenaSalida += "]";
  cadenaSalida += " -> [ ";
  for(int i = 0; i < NodosSalida; i++){
    cadenaSalida += round(salida[i]);  
    cadenaSalida += " ";
  }
  cadenaSalida += "]";
  Serial.println(cadenaSalida);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(cadenaDisplay);
  delay(500);
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.scrollDisplayLeft();
  lcd.setCursor(3,1);
  int a = round(salida[0]);
  int b = round(salida[1]);
  int c = round(salida[2]);

   if(a == 1 && b == 0 && c == 0){
    lcd.print("     Lemon");
   }else if(a == 0 && b == 1 && c == 0){
     lcd.print("     Lemon");
   }else if(a == 0 && b == 0 && c == 1){
     lcd.print("     Lemon");
   }else{
     lcd.print("      ");
   }
   delay(500);
}

/**********************************************************************************
 * Algoritmo de la Red Neuonal 
 * 
***********************************************************************************/
void RedNeuronal (float *valoresEntrada, float *salida){
  
  /********************************************************************
   * Pesos y Bias resultantes del entrenamiento
   * wh - Pesos capa oculta
   * bh - Bias de la capa oculta
   * wy - Pesos de la capa de salida
   * by - Bias de la capa de salida
   * minh, maxh, miny, maxy, minX y maxX - valores minimos y maximos
   * en las capas de entrada, oculta y salida
  *********************************************************************/
  float wh[NodosOcultos][NodosEntrada] = {
    {0.960535765922126,-0.490671169757107,-2.25964108276715,2.50108093463283,0.194586259540010}, 
    {0.469078278607841,5.48986523355049,-4.78114212148943,-4.99721858514895,2.80680594532198},
    {-1.57636870134975,-0.584887409376274,2.68218068976828,-1.25514649960260,4.43993330637979},
    {-0.575121305873188,2.82730697304552,1.04113772154707,-5.14422524933625,-1.64028299621063},
    {2.63760244648591,2.83163383189333,2.89952632926326,-0.360162596517542,-2.66582192791981}
  };
  
  float bh[NodosOcultos] = {-1.02772180221916,-1.77963962419822,2.64182372116023,-0.977852307879433,1.63223721886768};
  
  float wy[NodosSalida][NodosOcultos] = {
    {2.24302499404172,-3.92702259741370,-2.17715966347729,2.91615488417604,-3.41820192994365},
    {-2.09640007592253,7.71309853984393,-4.06959432223125,0.403685510572570,2.39010108250865},
    {1.86633665173433,-4.39788864190079,6.46096038222172,-3.02420647628816,-1.04196844711011}
  };
  
  float by[NodosSalida] = {-2.90288788916134,-1.10540564073844,0.497940084169251};

  float minh[NodosEntrada] = {179,89,73,93,98};
  float maxh[NodosEntrada] = {510,249,228,155,336};
  float miny = -1;
  float maxy = 1;
  float minX = 0;
  float maxX = 1;
  float yh[NodosOcultos];
  /**********************************************************************
   * Procesamiento de los valores de entrada de acuerdo a los máximos y
   * mínimos
   **********************************************************************/
  for(int i = 0; i < NodosEntrada; i++){
    valoresEntrada[i] = valoresEntrada[i] - minh[i];
    valoresEntrada[i] = valoresEntrada[i] * (1/(maxh[i] - minh[i]));
    valoresEntrada[i] = valoresEntrada[i] * (maxy - miny);
    valoresEntrada[i] = valoresEntrada[i] + miny;
  }
  /***************************************************************************
   * Calculo de las salidas de la capa oculta utilizando funcion sigmoidal
   ***************************************************************************/
  for(int i = 0; i < NodosOcultos; i++){
    float suma = bh[i];
    for(int j = 0; j < NodosEntrada; j++){
      suma += (valoresEntrada[j] * wh[i][j]);
    }
    yh[i] = (2/(1 + exp(-2*suma)) - 1);
  }
  /***************************************************************************
   * Calculo de las salidas de la capa de salida utilizando funcion sigmoidal
   ***************************************************************************/
  for(int i = 0; i < NodosSalida; i++){
    float suma = by[i];
    for(int j = 0; j < NodosOcultos; j++){
      suma +=  (yh[j] * wy[i][j]);
    }
    salida[i] = (2/(1 + exp(-2*suma)) - 1);
  }
  /**********************************************************************
   * Procesamiento de las salidas de la capa de salida para obtener
   * la salida de la red neuronal
   **********************************************************************/
  for(int i = 0; i < NodosSalida; i++){
    salida[i] = salida[i] - miny;
    salida[i] = salida[i] * (1/(maxy - miny));
    salida[i] = salida[i] * (maxX - minX);
    salida[i] = salida[i] + minX;
  }
}
