int led[]={22,23,24,25};
int sw[]={26,27,28,29};
int i;
int j;
int k;

void setup()
{
  for(i=1;i<=4;i++)
  pinMode(led[i],OUTPUT);
  pinMode(sw[i],INPUT);
}

void loop()
{
  if(digitalRead(sw[0])==1)
  {
    for(i=1; i<=3; i++)
    {
      for(j=0; j<=3; j++)
      {
        digitalWrite(led[j],HIGH);
        delay(1000);
        digitalWrite(led[j],LOW);
      }
    }
  }
  if(digitalRead(sw[1])==1)
  {
    for(i=1; i<=2; i++)
    {
      for(k=3; k>=0; k--)
      {
        digitalWrite(led[k],HIGH);
        delay(1000);
        digitalWrite(led[k],LOW);
      }
    }
  }
  if(digitalRead(sw[2])==1)
  {
    for(i=1; i<=3; i++)
    {
      for(j=0; j<=3; j++)
      {
        digitalWrite(led[j],HIGH);
        delay(1000);
      }
      for(k=0; k<=3; k++)
      {
        digitalWrite(led[k],LOW);
      }
      delay(1000);
    }
  }
  if(digitalRead(sw[3])==1)
  {
    for(i=1; i<=2; i++)
    {
      for(j=3; j>=0; j--)
      {
        digitalWrite(led[j], HIGH);
        delay(1000);
      }
      for(k=0; k<=3; k++)
      {
        digitalWrite(led[k],LOW);
      }
      delay(1000);
    }    
  }
}
