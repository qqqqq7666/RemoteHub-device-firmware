int val;
int a;
void setup()
{
  Serial.begin(9600);
}

int mul(int num1, int num2)
{
  int result=num1*num2;
  return result;
}

void loop() 
{
  if(Serial.available()>0)
  a=Serial.read();
  val=mul(a,4);
  Serial.println(val);
  exit(0);
}
