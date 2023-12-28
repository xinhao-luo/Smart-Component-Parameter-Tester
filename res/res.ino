#define TP1_ADC 14 // or 5?
#define TP2_ADC 15
#define TP3_ADC 16
#define TP1_small 8
#define TP1_big 9
#define TP2_small 10
#define TP2_big 11
#define TP3_small 12
#define TP3_big 13




/*TP1:
  PC0, A0 D14
  PB0, D8 small 680
  PB1  D9 big 470k
*/

/*TP2:
  PC1, A1 D15
  PB2, D10
  PB3  D11
*/

/*TP3:
  PC2, A2 D16
  PB4, D12
  PB5  D13
*/






double measureBigR()
{
  int v_r1, v_r3;
  double v_d1, v_d3;
  double v1, v3;
  double vx;
  double r1, r2;
  double R1, R2;

// small resistor
// 470k置空，680一个vcc一个gnd
  pinMode(TP1_small, INPUT);
  pinMode(TP1_big, OUTPUT);
  pinMode(TP3_small, OUTPUT);
  pinMode(TP3_big, INPUT);

  digitalWrite(TP1_big, HIGH);
  digitalWrite(TP3_small, LOW);
  v_r1 = analogRead(TP1_ADC);
  v_r3 = analogRead(TP3_ADC);
  v_d1 = (double)v_r1;
  v_d3 = (double)v_r3;
  v1 = (v_d1/1023)*5;
  v3 = v_d3/1023*5;
  vx = v1 - v3;
  r1 = (509000+680)*vx/(5-vx);

  digitalWrite(TP1_big, LOW);
  digitalWrite(TP3_small, HIGH);
  v_r1 = analogRead(TP1_ADC);
  v_r3 = analogRead(TP3_ADC);
  v_d1 = (double)v_r1;
  v_d3 = (double)v_r3;
  v1 = (v_d1/1023)*5;
  v3 = v_d3/1023*5;
  vx = v3 - v1;
  r2 = (509000+680)*vx/(5-vx);

  R1 = (r1+r2)/2;

  return R1;
}






double measureSmallR()
{
  int v_r1, v_r3;
  double v_d1, v_d3;
  double v1, v3;
  double vx;
  double r1, r2;
  double R1, R2;

// small resistor
// 470k置空，680一个vcc一个gnd
  pinMode(TP1_small, OUTPUT);
  pinMode(TP1_big, INPUT);
  pinMode(TP3_small, OUTPUT);
  pinMode(TP3_big, INPUT);

  digitalWrite(TP1_small, HIGH);
  digitalWrite(TP3_small, LOW);
  v_r1 = analogRead(TP1_ADC);
  v_r3 = analogRead(TP3_ADC);
  v_d1 = (double)v_r1;
  v_d3 = (double)v_r3;
  v1 = (v_d1/1023)*5;
  v3 = v_d3/1023*5;
  // Serial.println(v1);
  // Serial.println(v3);
  vx = v1 - v3;
  r1 = (679+680)*vx/(5-vx);

  digitalWrite(TP1_small, LOW);
  digitalWrite(TP3_small, HIGH);
  v_r1 = analogRead(TP1_ADC);
  v_r3 = analogRead(TP3_ADC);
  v_d1 = (double)v_r1;
  v_d3 = (double)v_r3;
  v1 = (v_d1/1023)*5;
  v3 = v_d3/1023*5;
  vx = v3 - v1;
  r2 = (679+680)*vx/(5-vx);
  R1 = (r1+r2)/2;

  if(abs(vx)>4.9)
  {
    R2 = measureBigR();
    return R2;
  }
  else
    return R1;
}



void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = 0;
  while(i < 50){
    double r = measureSmallR();
    Serial.println(r);
    delay(500);
    i++;
  }
  exit(0);
}


