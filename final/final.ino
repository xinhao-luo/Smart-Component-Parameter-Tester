#define TP1_ADC 14
#define TP2_ADC 15
#define TP3_ADC 16
#define TP1_small 8
#define TP1_big 9
#define TP2_small 10
#define TP2_big 11
#define TP3_small 12
#define TP3_big 13

// global variables
const int TP[3][3] = {{TP1_ADC, TP1_small, TP1_big},{TP2_ADC, TP2_small, TP2_big},{TP3_ADC, TP3_small, TP3_big}};
int b,c,e;
bool isPNP;
int measureType = 0;// 1:R 2:C 3:BJT

// resistance
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


// capacitance
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

// BJT
int findB()
{
  int cnt;
  for(int cwd = 0;cwd < 3;cwd++){
    pinMode(TP[cwd][1], OUTPUT);
    digitalWrite(TP[cwd][1], !isPNP);
    cnt = 0;

    for(int i = 1;i <=2;i++){
      int pin = (cwd+i)%3;
      pinMode(TP[pin][0],OUTPUT);
      digitalWrite(TP[pin][0], isPNP);

      delay(5);
      int tmp = analogRead(TP[cwd][0]);
      // Serial.println("1:");
      // Serial.println(cwd);
      // Serial.println(tmp);
      // Serial.println(isPNP);
      if(!isPNP && tmp <= 220 && tmp >= 100 ||
          isPNP && tmp <= 1023 -100 && tmp >= 1023-220)
        cnt++;
      // Serial.println("2:");
      // Serial.println(cwd);
      // Serial.println(cnt);
      pinMode(TP[pin][0], INPUT);
    }
    pinMode(TP[cwd][1], INPUT);
    if(cnt == 2)
      return cwd;
  }
  return 3;
}

double get_beta(double& beta, double& UBEQ)
{
  pinMode(TP[b][2],OUTPUT);
  digitalWrite(TP[b][2], !isPNP);

  pinMode(TP[c][0],OUTPUT);
  digitalWrite(TP[c][0], !isPNP);
  pinMode(TP[e][1],OUTPUT);
  digitalWrite(TP[e][1], isPNP);

  delay(20);
  int Ub = analogRead(TP[b][0]);
  int Ue = analogRead(TP[e][0]);
  UBEQ = abs(Ub-Ue)*5/1023.0;
  //Serial.println(Ue);
  //Serial.println(Ub);
  beta = double(510000.0/680.0)*(double)abs(Ue - isPNP * 1023)/(double)abs((!isPNP)*1023-Ub);
  pinMode(TP[b][2], INPUT);
  pinMode(TP[c][0], INPUT);
  pinMode(TP[e][1], INPUT);
  return beta, UBEQ;
}

void tran(double& beta, double& UBEQ)
{
  isPNP = 0;
  b = findB();
  if(b==3){
    isPNP = 1;
    b = findB();
  }
  Serial.println(isPNP);
  Serial.println("Its type: PNP(1) or NPN(0):");
  Serial.println(isPNP);
  Serial.println("Its b:TP");
  Serial.println(b+1);

  //findCE();
  c = (b+1)%3;
  e = (b+2)%3;
  beta, UBEQ = get_beta(beta, UBEQ);
  if(beta<50){
  	int tmp = c;
    c = e;
    e = tmp;
  }
  Serial.println("Its c:TP");
  Serial.println(c+1);
  Serial.println("Its e:TP");
  Serial.println(e+1);
  beta, UBEQ = get_beta(beta, UBEQ);
}

// integration
void dischargeSecret()
{
  pinMode(TP1_small,OUTPUT);
  pinMode(TP3_small,OUTPUT);
  pinMode(TP1_big,OUTPUT);
  pinMode(TP3_big,OUTPUT);
  digitalWrite(TP1_small,LOW);
  digitalWrite(TP3_small,LOW);
  digitalWrite(TP1_big,LOW);
  digitalWrite(TP3_big,LOW);
  delay(1000);
  
}



void mR()
{
  double r = measureSmallR();
  Serial.println(r);
  delay(500);
  return;
}

void mC()
{

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
  return;
}

void mBJT()
{
  double beta, UBEQ;
  tran(beta, UBEQ);
  Serial.println(beta);
  Serial.println(UBEQ);
  delay(5000);
  return;

}
void findType()
{
  int tmp;
  isPNP = 0;
  tmp = findB();
  if(tmp==3){
    isPNP = 1;
    tmp = findB();
  }
  if(tmp != 3){
    measureType = 3;
    return;
  }
  dischargeSecret();
  int v_r1, v_r3;
  pinMode(TP1_small,OUTPUT);
  pinMode(TP3_small,OUTPUT);
  pinMode(TP1_big,INPUT);
  pinMode(TP3_big,INPUT);
  digitalWrite(TP1_small,HIGH);
  digitalWrite(TP3_small,LOW);
  v_r1 = analogRead(TP1_ADC);
  v_r3 = analogRead(TP3_ADC);
  int v1 = v_r1 - v_r3;
  // Serial.println(v1);
  delay(100);

  v_r1 = analogRead(TP1_ADC);
  v_r3 = analogRead(TP3_ADC);
  int v2 = v_r1 - v_r3;
  // Serial.println(v2);
  tmp = abs(v1-v2);
  if(tmp > 200){
    measureType = 2;
    return;
  }
  measureType = 1;
  return;

}


void setup() {
  Serial.begin(9600);
  findType();
  if(measureType == 1){
    Serial.println("This is a resistance.");
  }
  else if(measureType == 2){
    Serial.println("This is a capacitance");
  }
  else if(measureType == 3){
    Serial.println("This is a transistor");
  }
  else{
    Serial.println("unknown type encounted");
  }
  delay(1000);

}

void loop() {
  int i = 0;
  while(i < 50){
    if(measureType == 1){
      mR();
    }
    else if(measureType == 2){
      mC();
    }
    else if(measureType == 3){
      mBJT();
    }
    else{
      break;
    }
    i++;
  }
  exit(0);

}
