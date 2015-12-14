#include <MicroView.h>
#include <Time.h>

//#define MODE 2

// Full length day of week names:
char dows[][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int MODE = 1;
unsigned long NextTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Set the time in the time library:
  //setTime(HOUR, MINUTE, SECOND, DAY, MONTH, YEAR);
  SetDateTime(__DATE__, __TIME__);
  uView.begin();    // set up the MicroView
  uView.setFontType(2);
  /*
    if(MODE == 2)
    uView.setFontType(0);
  */
  uView.clear(PAGE);// erase hardware memory inside the OLED
  uView.setFontType(2);
  /*
    uView.print("Test");
    uView.display();  // display the content in the buffer
    delay(2000);
  */
  NextTime = millis() + 30000;
}

char s[10];
int testit = 0;

void loop() {
  if (millis() < NextTime) {
    uView.clear(PAGE);
    uView.setFontType(2);
    uView.setCursor(0, 0);
    uView.print("12-45");
    uView.setCursor(0, 20);
    //uView.print("23:59");
    //uView.setCursor(0,30);
    sprintf(s, "%5d", testit / 10);
    uView.print(s);
    uView.display();
    //if( ( millis() % 100 ) == 0 )
    testit++;
    return;
  }
  MODE = 1;
  if (second() <= 5 || (second() >= 30 && second() < 36) )
    MODE = 2 ;
  int hr = hour();
  /*
    if(hr>12)
    hr = hr - 12;
    if(hr==0)
    hr = 12;
  */
  // put your main code here, to run repeatedly:
  uView.setFontType(3);
  uView.setCursor(0, 0);
  uView.clear(PAGE);
  sprintf(s, "%2d %02d", hr, minute());

  if (MODE == 2) {
    uView.setFontType(0);
    sprintf(s, "%2d:%02d:%02d", hr, minute(), second());
  }
  if ((millis() % 2000) > 1500 ) {
    sprintf(s, "%2d %02d", hr, minute());
    if (MODE == 2) {
      sprintf(s, "%2d %02d %02d", hr, minute(), second());
    }
  } else {
    if (MODE != 2 ) {
      int r = 3;
      int offsety = 7;
      int offsetx = 2;
      uView.circleFill(uView.getLCDWidth() / 2 - offsetx, uView.getLCDHeight() / 2 - offsety, r, 1, 0);
      uView.circleFill(uView.getLCDWidth() / 2 - offsetx, uView.getLCDHeight() - 6 - offsety, r, 1, 0);
    }
  }
  //uView.print(s);
  prints();
  if ( MODE == 2 ) {
    sprintf(s, "%s", dows[weekday() - 1]);
    uView.setCursor(0, 12);
    //uView.print(s);
    prints();
    sprintf(s, "%2d/%02d/%02d", month(), day(), year() % 100);
    uView.setCursor(0, 24);
    //uView.print(s);
    prints();
  }

  uView.display();
}

void prints() {
  /*
    for(int i=0; i<strlen(s); i++) {
    if(s[i]=='0')
      s[i]='O';
    }
  */
  uView.print(s);
}




// sample input: date = "Dec 26 2009", time = "12:34:56"
// jan feb mar apr may jun jul aug sep oct nov dec
// Time:
// 18:27:09
// Date:
// Sep  1 2015

void SetDateTime( char *date, char *time ) {
  int hr, min, sec;
  int month, day, year;
  month = 1;
  hr = 13; min = 18; sec = 0;
  hr = (time[0] - '0') * 10 + time[1] - '0';
  min = (time[3] - '0') * 10 + time[4] - '0';
  sec = (time[6] - '0') * 10 + time[7] - '0';

  char date4 = date[4];
  if (date4 == ' ')
    date4 = '0';
  day = (date4 - '0') * 10 + date[5] - '0';
  year = (date[9] - '0') * 10 + date[10] - '0';
  if (date[0] == 'D')
    month = 12;
  if (date[0] == 'J') {
    if (date[1] == 'a')
      month = 1;
    if (date[1] == 'u') {
      if (date[2] == 'l')
        month = 7;
      if (date[2] == 'n')
        month = 6;
    }
  }
  if (date[0] == 'N')
    month = 11;
  if (date[0] == 'O')
    month = 10;
  if (date[0] == 'F')
    month = 2;
  if (date[0] == 'M') {
    if (date[2] == 'r')
      month = 3;
    if (date[2] == 'y')
      month = 5;
  }
  if (date[0] == 'A')
  {
    if (date[1] == 'p')
      month = 4;
    if (date[1] == 'u')
      month = 8;
  }
  if (date[0] == 'S')
    month = 9;



  Serial.println("Time:");
  Serial.println(time);
  Serial.println("Date:");
  Serial.println(date);
  setTime(hr, min, sec, day, month, year);
}
