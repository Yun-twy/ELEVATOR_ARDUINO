//pin setup
const int led3 = 13;
const int led22 = 12;
const int led21 = 11;
const int led2 = 10;
const int led12 = 9;
const int led11 = 8;
const int led1 = 7;
const int buttonled3 = 6;
const int buttonled2 = 5;
const int buttonled1 = 4;
const int button3 = 17;
const int button2 = 16;
const int button1 = 15;

//constant
const int UP = 1;
const int DOWN = 0;
const int MAX_FLOOR = 3;

//global variable setup
int totalArray[7] = {led1, led11, led12, led2, led21, led22, led3};
int floorArray[3] = {led1, led2, led3};

int nowStep = 0; //led number
int nowFloor = 0; // start from 1st floor
int direction = UP;
int goal = 2;
bool callArray[MAX_FLOOR] = {false, false, false}; //waiting list
int buttonArray[MAX_FLOOR] = {button1, button2, button3};
int buttonLedArray[MAX_FLOOR] ={buttonled1, buttonled2, buttonled3};
int count = 0; // each count takes 0.01second
int countB = 0; 
int newCall = 0; //update check

void setup()
{
  //Elevator led setting
  pinMode(led3, OUTPUT);
  pinMode(led22, OUTPUT);
  pinMode(led21, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led12, OUTPUT);
  pinMode(led11, OUTPUT);
  pinMode(led1, OUTPUT);
  //Floor led setting
  pinMode(buttonled3, OUTPUT);
  pinMode(buttonled2, OUTPUT);
  pinMode(buttonled1, OUTPUT);
  //button setting
  pinMode(button3, INPUT);
  pinMode(button2, INPUT);
  pinMode(button1, INPUT);
  //Serial setup
  Serial.begin(9600);
}

void callCheck() //when button clicked, update callArray
{ 
  for (int i=0; i<MAX_FLOOR; i++)
  {
    if (digitalRead(buttonArray[i])==HIGH)
    {
      callArray[i] = true;
      newCall += 1; 
      
      //updateDirection();
    }
  }
  return;
}


void showButton()
{
  if (newCall > 0)
  {
    for (int i=0; i<MAX_FLOOR; i++)
    {
      if (callArray[i] == true)
      {
        digitalWrite(buttonLedArray[i], HIGH);
        
        if ( nowFloor == i)// && (3*(nowFloor-1)+3 == nowStep) ) //turn off the light when the lift arrives
        {
          if (count == 10)
          {
            digitalWrite(buttonLedArray[i], LOW);
            callArray[i] = false;
            newCall -= 1;
            count = 0;
          }
          else
          {
            count += 1;
            delay(10);
          }
        } 
      }
    }
  }
}


void showElevator() //current location
{
  for (int i=0; i<7; i++)
  {
    if (nowStep ==i)
    {
      digitalWrite(totalArray[i], HIGH); //current loc on
    }
    else
    {
      digitalWrite(totalArray[i], LOW); //other locs off
    }
  }
}

void updateDirection()
{
  if (nowFloor==0)
  {
    direction = UP;
  }
  else if (nowFloor==2)
  {
    direction = DOWN;
  }
  else if (nowFloor ==1 )
  {
    if (callArray[0]== false)
    {
      direction = UP;
    }
    else if (callArray[2]==false)
    {
      direction =DOWN;
    }
  }
}

void move()
{
  for (int i=0; i<MAX_FLOOR; i++)
  {
    if (callArray[i] == true)
    {
      int start = nowFloor;
      int end = i;
      updateDirection();
      //Serial.println('----Direction----');
      //Serial.println(direction);
      if (direction==UP && start<end)
      {
        moveStep(start, end);
      }
      else if (direction==DOWN && start>end)
      {
        moveStep(start, end);
      }
    }
  }
  return;
}

void moveStep(int current, int target)
{
  if (current < target)
  {
    if ((3*(target-1)+3) != nowStep) //3(n-1)+3
    {
      if (countB==10)
      {
        nowStep += 1;
        countB =0;
        if (nowStep==3)
        {
          nowFloor=1;
        }
      }
      else
      {
        countB +=1;
        delay(100);
      }
    }
    else
    {
      nowFloor = target;   
    }
  }
  else if (current > target)
  {
    if ((3*(target-1)+3) != nowStep) //3(n-1)+3
    {
      if (countB ==10)
      {
        nowStep -= 1;
        countB =0;
        if (nowStep==3)
        {
          nowFloor=1;
        }
      }
      else
      {
        countB +=1;
        delay(100);
      }
    }
    else
    {
      nowFloor = target;  
    }
  }
    
  return;
}

void loop()//
{
  showElevator();
  callCheck();
  showButton();
  move();
  Serial.println(nowFloor);
  Serial.println(nowStep);
}
