#define DIN 14
#define OE 15
#define STCP 16
#define SHCP 17
#define MR 18
#define BUTTON0 2
#define BUTTON1 3
#define BUTTON2 4
#define BUTTON3 5

uint8_t MegaArray[6] = {0};

void clr()
{
  digitalWrite(MR, LOW);
  digitalWrite(MR, HIGH);
}


void updateArrow(bool Rst)
{
  static uint8_t state = 5;
  if (Rst) state = 5;
  //pins 37 38 39
  MegaArray[4] &= 0x1F;
  MegaArray[4]  |= (1 << (state & 0x7));
  state++;
  if (state >= 8)
    state = 5;
}

void updateBar(bool Rst)
{
  static uint8_t state = 0;
  if (Rst) state = 0;
  MegaArray[5]  = (state == 0) ? (1 << 4) : ((1 << state) - 1);
  state++;
  if (state > 5)
    state = 0;
}

void update7Segment(bool Rst)
{
  static const uint8_t sevenSegment[16] =
  { 0xEE, 0x28, 0xDA, 0xBA,
    0x3C, 0xB6, 0xF6, 0x2A,
    0xFE, 0xBE, 0x7E, 0xF4,
    0xC6, 0xF8, 0xD6, 0x56
  };
  static uint8_t state = 0;
  if (Rst) state = 0;
  MegaArray[1] = sevenSegment[state];
  state++;
  if (state > sizeof(sevenSegment) - 1)
    state = 0;
}

void update8Segment(bool Rst)
{
  static const uint8_t eightSegment[16] =
  {
    0xB2,  0x50, 0xC6, 0xC3,
    0x19, 0x8B, 0x8F, 0x90,
    0xCF, 0xDA, 0xCD, 0x27,
    0xA2, 0x17, 0x8E, 0x8C
  };
  static uint8_t state = 0;
  if (Rst) state = 0;
  MegaArray[0] = eightSegment[state];
  state++;
  if (state > sizeof(eightSegment) - 1)
    state = 0;
}

void update19Segment(bool Rst)
{
  static const uint32_t ningthteenSegment[] =
  {
    0x128B4E,//0
    0x0004C,//1
    0x12851A,//2
    0x128056,//3
    0x000c58,//4
    0x128c52,//5
    0x128D52,//6
    0x088006,//7
    0x128D5A,//8
    0x128C5A,//9
    0x00025C,//А
    0x128D22,//Б
    0x128D26,//В
    0x008902,//Г
    0x1322CA,//Д
    0x128D12,//Е
    0x128D13,//Ё
    0x083224,//Ж
    0x128426,//З
    0x000B4C,//И
    0x000B4D,//Й
    0x000D24,//К
    0x00224A,//Л
    0x00194C,//М
    0x000D58,//Н
    0x12894A,//О
    0x00894A,//П
    0x008D1A,//Р
    0x128902,//С
    0x08A002,//Т
    0x121058,//У
    0x08AC1A,//Ф
    0x001224,//Х
    0x1209C8,//Ц
    0x001058,//Ч
    0x1A2948,//Ш
    0x1A29C8,//Щ
    0x18A050,//Ъ
    0x0A0D48,//Ы
    0x182050,//Ь
    0x12845A,//Э
    0x182D4A,//Ю
    0x008E5A //Я
  };
  static uint8_t state = 0;
  if (Rst) state = 0;
  MegaArray[2] = (ningthteenSegment[state] & 0xff);
  MegaArray[3] = ((ningthteenSegment[state] >> 8) & 0xff);
  MegaArray[4] &= 0xE0;
  MegaArray[4] |= ((ningthteenSegment[state] >> 16) & 0xff);
  state++;
  if (state > sizeof(ningthteenSegment) / 4 - 1)
    state = 0;
}

void strobe()
{
  digitalWrite(STCP, HIGH);
  digitalWrite(STCP, LOW);
}

void writeBit(bool data)
{
  digitalWrite(DIN, data);
  digitalWrite(SHCP, HIGH);
  digitalWrite(SHCP, LOW);
  digitalWrite(DIN, LOW);
}

void writeByte(uint8_t data)
{
  for (uint8_t i = 0; i < 8; ++i)
  {
    writeBit((data & (1 << i)) ? HIGH : LOW);
  }
}

void writeArray(uint8_t *data, uint8_t size)
{
  for (uint8_t i = 0; i < size; ++i)
  {
    writeByte(data[i]);
  }
  strobe();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // put your setup code here, to run once:
  for (uint8_t i = 2; i <= 5; ++i)
  {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  for (uint8_t i = 14; i < 18 ; ++i)
  {
    pinMode(i, OUTPUT);
  }
  digitalWrite(DIN, LOW);
  digitalWrite(OE, LOW);
  digitalWrite(STCP, LOW);
  digitalWrite(SHCP, LOW);
  digitalWrite(MR, HIGH);
}

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long prevButton = 0;        // will store last time LED was updated
unsigned long prevUpdate = 0;        // will store last time LED was updated

// constants won't change:
const long intervalButton = 50;           // interval at which to blink (milliseconds)
const long intervalUpdate = 250;           // interval at which to blink (milliseconds)

bool checkButton0()
{
  bool result = false;
  static uint8_t prevState = HIGH;
  uint8_t currentState = digitalRead(BUTTON0);
  if (!currentState & prevState)
  {
    result = true;
  }
  prevState = currentState;
  return result;
}

bool checkButton1()
{
  bool result = false;
  static uint8_t prevState = HIGH;
  uint8_t currentState = digitalRead(BUTTON1);
  if (!currentState & prevState)  {
    result = true;
  }
  prevState = currentState;
  return result;
}
bool checkButton2()
{
  bool result = false;
  static uint8_t prevState = HIGH;
  uint8_t currentState = digitalRead(BUTTON2);
  if (!currentState & prevState) {
    result = true;
  }
  prevState = currentState;
  return result;
}
bool checkButton3()
{
  bool result = false;
  static uint8_t prevState = HIGH;
  uint8_t currentState = digitalRead(BUTTON3);
  if (!currentState & prevState) {
    result = true;
  }
  prevState = currentState;
  return result;
}

void loop() {
  static uint8_t state = 0;
  static bool autoPlaySegments = true;
  static bool autoPlayBar = true;

  static bool Reset = false;
  static bool updateBarOnce = false;
  static bool updateSegmentsOnce = false;

  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  static bool updateData = false;
  if (currentMillis - prevButton >= intervalButton) {
    prevButton = currentMillis;
      Reset = checkButton2();
      if (checkButton0())
      {
        autoPlaySegments = false;
        updateSegmentsOnce = true;
      }
      if (checkButton1())
      {
        autoPlayBar = false;
        updateBarOnce = true;
      }
      if (checkButton3())
      {
        autoPlaySegments = true;
        autoPlayBar = true;
      }
  }

  if (currentMillis - prevUpdate >= intervalUpdate) {
    prevUpdate = currentMillis;
    if ((autoPlaySegments == true) || (updateSegmentsOnce) || Reset)
    {
      update7Segment(Reset);
      update8Segment(Reset);
      update19Segment(Reset);
      updateData = true;
      updateSegmentsOnce = false;
    }
    if ((autoPlayBar == true) || (updateBarOnce) || Reset)
    {
      updateArrow(Reset);
      updateBar(Reset);
      updateData = true;
      updateBarOnce = false;
    }
  }
  if (updateData == true)
  {
    writeArray(MegaArray, 6);
    updateData = false;
  }


}
