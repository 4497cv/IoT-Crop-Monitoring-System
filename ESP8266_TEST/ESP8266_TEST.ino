/****************************************
 * Include Libraries
 ****************************************/
#include "lm35dz.h"
#include "cd4051be.h"

static const float h2[7]={-0.0956, 0.0468, 0.3148, 0.4525, 0.3148, 0.0468, -0.0956};
static float hn[7]={1,0,0,0,0,0,0};
static float xn[7];

/****************************************
 * Main Functions
 ****************************************/
void setup()
{
  /* multiplexer module initialization */
  cd4051be_init();
  /* establish serial communication */
  Serial.begin(115200);
  cd4051be_setChannel(CH_0);
}

void loop()
{
  int i;
  volatile float x0;
  volatile float y;
  
  /* lectura de valor del ADC  */
  x0 = LM35DZ_get_temperature_celsius();

  /* tomar muestras iniciales */
  for(i = 6; i >= 1; i--) xn[i] = xn[i-1];

  xn[0] = x0;
  
  y=0;
  /* realizar convolución de la entrada con la respuesta h1/h2 */
  for(i = 0; i < 7 ; i++) y += xn[i]* h2[i];
  /* Salida de la señal de audio resultante hacia el DAC */
  Serial.println(y);
  y=0;
  delay(500);
}
