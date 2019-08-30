/*
                    Mode 0                Mode 1
            ┌─────────────────────┬─────────────────────┐
   Screen 0 │  Mode 0 : Screen 0  |  Mode 1 : Screen 0  │
            ├─────────────────────┼─────────────────────┤
   Screen 1 │  Mode 0 : Screen 1  |  Mode 1 : Screen 1  │
            ├─────────────────────┼─────────────────────┤
   Screen 2 │  Mode 0 : Screen 2  |  Mode 1 : Screen 2  │
            ├─────────────────────┼─────────────────────┘
   Screen 3 │  Mode 0 : Screen 3  |
            └─────────────────────┘
*/

#include <SimpleTimer.h>
#include <Bounce2.h>

#define BUTTON_PIN                    13           //BUTTON pin

boolean MODE                        = 0;           //MODE 0 or 1
byte SCREEN_NUMBER                  = 0;           //current SCREEN
byte SCREENS_IN_MODE_0              = 4;           //number of SCREENS in MODE 0
byte SCREENS_IN_MODE_1              = 3;           //number of SCREENS in MODE 1

boolean BUTTON_FLAG                 = 0;           //button status flag
unsigned long BUTTON_PRESS_START    = 0;           //
unsigned long BUTTON_PRESS_INTERVAL = 1000;        //interval for LONG press
unsigned long RESET_TIME            = 3000;        //interval for VEEEEEERY LONG press

Bounce debouncer = Bounce();
SimpleTimer timer;

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  debouncer.attach(BUTTON_PIN);                    //customize Bouncer
  debouncer.interval(5);                           //interval

  SHOW();

  timer.setInterval(50L, CHECK_BUTTON);           //check button every 50 ms
  timer.setInterval(2000L, SHOW);                 //show smth on "screen" every 2000 ms
}

void loop()
{
  timer.run();
}

void SHOW()
{
  switch (SCREEN_NUMBER)
  {
    case 0: SCREEN_0(); break;
    case 1: SCREEN_1(); break;
    case 2: SCREEN_2(); break;
    case 3: SCREEN_3(); break;
  }
}

void SCREEN_0()
{
  if (!MODE)
  {
    Serial.println(" ███ | ░░░");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ");
    Serial.println();
  } else
  {
    Serial.println(" ░░░ | ███");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ");
    Serial.println();
  }
}

void SCREEN_1()
{
  if (!MODE)
  {
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ███ | ░░░");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ");
    Serial.println();
  } else
  {
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ███");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ");
    Serial.println();
  }
}

void SCREEN_2()
{
  if (!MODE)
  {
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ███ | ░░░");
    Serial.println(" ░░░ | ");
    Serial.println();
  } else
  {
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ███");
    Serial.println(" ░░░ | ");
    Serial.println();
  }
}

void SCREEN_3()
{
  if (!MODE)
  {
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ░░░ | ░░░");
    Serial.println(" ███ | ");
    Serial.println();
  }
}

void CHECK_BUTTON()
{
  boolean changed = debouncer.update();
  if (changed)
  {
    int value = !debouncer.read();                   //vice versa (INPUT_PULLUP)
    if (value == 1 && BUTTON_FLAG == 0)              //button was pressed
    {
      BUTTON_FLAG = 1;
      BUTTON_PRESS_START = millis();
      Serial.println(F("Button pressed..."));
    }
    if (value == 0 && BUTTON_FLAG == 1)
    {
      BUTTON_FLAG = 0;
      SCREEN_NUMBER++;
      if ((MODE == 0) && (SCREEN_NUMBER > SCREENS_IN_MODE_0 - 1)) SCREEN_NUMBER = 0;
      if ((MODE == 1) && (SCREEN_NUMBER > SCREENS_IN_MODE_1 - 1)) SCREEN_NUMBER = 0;

      SHOW();

      Serial.print("Quick press. MODE: ");
      Serial.print(MODE);
      Serial.print("  SCREEN_NUMBER: ");
      Serial.println(SCREEN_NUMBER);
    }
  }
  if (BUTTON_FLAG == 1 && (millis() - BUTTON_PRESS_START > BUTTON_PRESS_INTERVAL))
  {
    BUTTON_FLAG = 0;
    MODE = !MODE;
    SCREEN_NUMBER = 0;
    SHOW();
    Serial.print(F("Long press.  MODE: "));
    Serial.print(MODE);
    Serial.print("  SCREEN_NUMBER: ");
    Serial.println(SCREEN_NUMBER);
  }

  if (!debouncer.read() && (millis() - BUTTON_PRESS_START > RESET_TIME))
  {
    Serial.println("VEEEERY LONG PRESS");
  }
}
