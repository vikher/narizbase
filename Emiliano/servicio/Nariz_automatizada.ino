/********************************************************************
 * Automatización de la Nariz Electrónica a través de un
 * algoritmo de red neuronal previamente entrenada para la detección 
 * de Papaya, Naranja y Manzana.
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
 *       [1 0 0] - Papaya
 *       [0 1 0] - Naranja
 *       [0 0 1] - Manzana
 ********************************************************************/

#include <math.h>

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

void setup(){
  Serial.begin(9600);
}

//{434,209,178,154,243};
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
  
  float salida[NodosSalida];
  
  RedNeuronal(valoresEntrada, salida);
  
  //Pocesamiento de la salida
  Serial.print("[ ");
  for(int i=0; i < NodosEntrada; i++){
    Serial.print(entradasOriginales[i]);
    Serial.print(", ");
  }
  Serial.print("] -> ");
  
  Serial.print("[ ");
  for(int i = 0; i < NodosSalida; i++){
    Serial.print(round(salida[i]));  
    Serial.print(" ");
  }
  Serial.println("]");
  delay(1000);
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
    {1.01995198382119,-0.461332425288987,-2.22175898911184,2.55503287792048,0.258631105313892}, 
    {-0.196568294772324,5.45311701423804,-5.99896747331592,-0.888654834570042,1.24669593650149},
    {0.0973412612946058,-0.453837487408206,0.941743545570309,0.313066806586723,4.27732857068036},
    {0.00484846480160411,3.10047178904322,1.39304218562446,-4.63391081255369,-1.01679302177671},
    {2.76941794395513,1.57571160231193,-0.400817501683741,1.50584986633342,-3.09491047182937}
  };
  
  float bh[NodosOcultos] = {-1.09659722661574,-0.201210660592570,4.30442603530485,-1.66821496565658,1.96045563475218};
  
  float wy[NodosSalida][NodosOcultos] = {
    {0.347098873448549,-3.47124220521613,-0.279033057884052,4.86256727674867,-2.36163392086547},
    {-1.79582191042740,6.77883952039709,-4.64244245682428,0.0904454327289272,3.35529267401077},
    {3.40332432162020,-2.86764114414406,4.85980803970527,-4.56245106320951,-1.70369478952257}
  };
  
  float by[NodosSalida] = {-1.03659054983903,-1.39745892646399,-1.02120949414766};

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
