#define TP1_ADC 14
#define TP2_ADC 15
#define TP3_ADC 16
#define TP1_small 8
#define TP1_big 9
#define TP2_small 10
#define TP2_big 11
#define TP3_small 12
#define TP3_big 13

const int TP[3][3] = {{TP1_ADC, TP1_small, TP1_big},{TP2_ADC, TP2_small, TP2_big},{TP3_ADC, TP3_small, TP3_big}};
int b,c,e;
bool isPNP;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = 0;
  while(i < 50){
    double beta, UBEQ;
    tran(beta, UBEQ);
    Serial.println(beta);
    Serial.println(UBEQ);
    i++;
  } 
  exit(0);
}