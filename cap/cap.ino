#define TP1_ADC 14
#define TP2_ADC 15
#define TP3_ADC 16
#define TP1_small 8
#define TP1_big 9
#define TP2_small 10
#define TP2_big 11
#define TP3_small 12
#define TP3_big 13

void discharge()
{
  Serial.println(("discharging..."));
  pinMode(TP1_small,OUTPUT);
  pinMode(TP3_small,OUTPUT);
  pinMode(TP1_big,OUTPUT);
  pinMode(TP3_big,OUTPUT);
  digitalWrite(TP1_small,LOW);
  digitalWrite(TP3_small,LOW);
  digitalWrite(TP1_big,LOW);
  digitalWrite(TP3_big,LOW);
  delay(1000);
  Serial.println(("done."));
  
}

void chargeSmallC()
{
  Serial.println(("charging..."));
  pinMode(TP1_small,OUTPUT);
  pinMode(TP3_small,OUTPUT);
  pinMode(TP1_big,INPUT);
  pinMode(TP3_big,INPUT);
  digitalWrite(TP1_small,HIGH);
  digitalWrite(TP3_small,LOW);
  delay(1000);
  Serial.println(("done"));
  
}

bool isBig(){
  bool result = true;

  pinMode(TP1_small,OUTPUT);
  pinMode(TP3_small,OUTPUT);
  pinMode(TP1_big,INPUT);
  pinMode(TP3_big,INPUT);
  digitalWrite(TP1_small,HIGH);
  digitalWrite(TP3_small,LOW);
  delay(10);
  int Uc1 = analogRead(TP1_ADC);
  int Uc3 = analogRead(TP3_ADC);
  int Uc = Uc1 - Uc3;
  if(Uc >= 1000) {
    result = false; 
  }
  return result;
}

double measureBigC()
{
  int v_r1, v_r3;
  double v_d1, v_d3, v1, v3;
  discharge();
  pinMode(TP1_big,OUTPUT);
  pinMode(TP3_big,OUTPUT);
  pinMode(TP1_small,INPUT);
  pinMode(TP3_small,INPUT);
  digitalWrite(TP1_big,HIGH);
  digitalWrite(TP3_big,LOW);

  int count = 0;
  double uc = 0;
  while(count<=100 && uc<0.5){
    v_r1 = analogRead(TP1_ADC);
    v_r3 = analogRead(TP3_ADC);
    v_d1 = (double)v_r1;
    v_d3 = (double)v_r3;
    v1 = (v_d1/1023)*5;
    v3 = v_d3/1023*5;
    uc = v1 - v3;
    delay(10);
    count++;

  }

  double time = 0.01 * count;
  double tmp = -log(1.0 - uc/5.0);
  double result = 1000000 * time / (tmp*(509000+506000));
  return result;
}

double measureSmallC()
{
  int v_r1, v_r3;
  double v_d1, v_d3, v1, v3;
  double e = 2.7182818;
  discharge();
  chargeSmallC();

  v_r1 = analogRead(TP1_ADC);
  v_r3 = analogRead(TP3_ADC);
  v_d1 = (double)v_r1;
  v_d3 = (double)v_r3;
  double uc0 = v_d1 - v_d3;

  int count = 0;
  double uc = 0;
  pinMode(TP1_small,INPUT);
  pinMode(TP3_small,INPUT);
  pinMode(TP1_big,OUTPUT);
  pinMode(TP3_big,OUTPUT);
  digitalWrite(TP1_big,LOW);
  digitalWrite(TP3_big,LOW);
  
  v_r1 = analogRead(TP1_ADC);
  v_r3 = analogRead(TP3_ADC);
  v_d1 = (double)v_r1;
  v_d3 = (double)v_r3;
  uc = v_d1 - v_d3;
  while(uc > (uc0/e)){
    v_r1 = analogRead(TP1_ADC);
    v_r3 = analogRead(TP3_ADC);
    v_d1 = (double)v_r1;
    v_d3 = (double)v_r3;
    uc = v_d1 - v_d3;
    delay(1);
    count++;

  }

  double time = count;
  // Serial.println(time);
  double result = 1000 * time / (509000+506000);
  return result;

}

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = 0;
  while(i < 50){
    Serial.println("---------------");
    discharge();
    bool big = isBig();
    double c;
    if(big){
      Serial.println("This is a big c.");
      c = measureBigC();
    }
    else{
      Serial.println("This is a small c.");
      // c = measureBigC();
      c = measureSmallC();
    }

    Serial.println(c);
    discharge();
    i++;
  }
  exit(0);
}
