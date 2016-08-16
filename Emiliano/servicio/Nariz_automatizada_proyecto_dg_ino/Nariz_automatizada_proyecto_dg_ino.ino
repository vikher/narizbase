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
const int NodosOcultos = 25; 
const int NodosSalida = 12;
 
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
    {1.55408395937549,-1.1043226704367,-0.625150290104056,1.24415381461713,0.865250653343528},
    {-0.333987097784205,-0.721206585341943,3.37352488434145,-5.07322911980152,-2.07693340364123},
    {3.23458950700433,0.347643021946314,-1.28135412457736,0.94899980165098,2.41451308411358},
    {0.73755571842081,-0.307103639499357,1.31610689318514,1.0891671541182,-1.25840823115218},
    {1.88387842681684,-1.65183585259758,-1.64030545169325,-2.34994972284738,0.556300483132932},
    {-1.35461454319773,-2.30456394544789,1.74222638351933,4.64579347860536,-4.08920369046555},
    {0.911512878262343,-0.687964937002176,-2.46693089254783,3.83277146721375,2.6263573246251},
    {1.60980792120236,1.48846694682671,1.84308113164075,-1.90318108441105,-1.93327299769628},
    {-1.87181456572532,4.23498891806437,-1.39276939550489,0.270771734751143,4.77378624393709},
    {-3.10625185832877,2.66204697303493,-2.62654469546129,2.02255102281772,1.77333964466985},
    {3.16854667310274,1.29749804121902,0.647678122325151,-2.95896800399441,-1.76792085284138},
    {-1.49521881128769,0.162684816915563,-1.79043840734895,2.64758258162315,-2.40661731091662},
    {-5.01819955938409,-3.30659996574816,5.64265639836916,-0.389009408609506,6.31937420930363},
    {-1.63076639825592,-0.997729023851589,-0.818577350898095,1.31979462160145,5.58784845085715},
    {-2.61656425386974,8.2977006266966,-4.27825739821356,-1.5461371969564,0.793943321903053},
    {0.878922030081364,-0.328108064649365,-3.1773166170187,0.081559071001873,-2.36868934127933},
    {-1.47736692735067,1.66772112628762,0.955522208402223,-0.196235029200871,0.59805155584256},
    {-3.36885899509364,-4.59955782636701,-1.90820280643489,1.4138927195772,4.25399082272429},
    {1.19819274043971,-1.06120482471591,-1.58137548688622,2.50679014058705,1.61501257433724},
    {-0.540765676968998,1.61595754714596,0.796249351744687,-0.283902490772289,-0.832290099808185},
    {1.80196111891944,-2.34067588872055,-2.08705122395094,-2.99778322388381,1.63442160408197},
    {-1.45168708486448,-1.05310278091344,-1.8741268561232,-5.99355290690346,-0.0616270122960302},
    {2.49845729747482,1.32399288203247,-2.16710349394517,1.87923628810006,2.6600615494305},
    {2.53996399982555,5.14185135979011,2.54327162130693,-1.15077371703066,1.113372181441},
    {0.80389908913203,0.586142511660586,-1.28908862978698,-1.36198806236759,-0.388669177147121}
  };
  
  float bh[NodosOcultos] = {-3.25673418641350,
                          -4.67455287457812,
                          -2.39804069646148,
                          -2.93957449457904,
                          -1.46547280646267,
                          -4.41059680162024,
                          1.93339038022594,
                          -0.809827058896212,
                          2.14438740572098,
                          1.69080652722725,
                          -0.131990014101936,
                          0.493648372916902,
                          1.58854041908499,
                          -0.322399657615589,
                          0.0582852552055929,
                          -1.69901263478675,
                          -1.93846044860296,
                          -4.36334133417541,
                          2.14788928277565,
                          -2.52372088439491,
                          1.60589968685329,
                          -1.27876722451295,
                          1.52318364566735,
                          7.91223838358097,
                          3.50250393587091};
  
  float wy[NodosSalida][NodosOcultos] = {
    {2.24302499404172,-3.92702259741370,-2.17715966347729,2.91615488417604,-3.41820192994365},
    {-2.09640007592253,7.71309853984393,-4.06959432223125,0.403685510572570,2.39010108250865},
    {1.86633665173433,-4.39788864190079,6.46096038222172,-3.02420647628816,-1.04196844711011}
  };
  
  float by[NodosSalida] = {-2.39041814960659,
                          -0.308761091635939,
                          -0.0658656948663389,
                          -0.667276550492073,
                          -0.624646858334158,
                          -1.78572147857071,
                          -0.662508614747756,
                          -1.38389872142537,
                          -2.05258748595340,
                          0.245103016758037,
                          0.406938924298525,
                          0.415625338410665};

  float minh[NodosEntrada] = {136,39,50,37,31};
  float maxh[NodosEntrada] = {539,165,286,106,477};
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
