#define DIN 21
#define OE 20
#define STCP 19
#define SHCP 18
#define MR 17

uint8_t MegaArray[12] = {0};

void clr()
{
  digitalWrite(MR, LOW);
  digitalWrite(MR, HIGH);
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

  for (uint8_t i = 17; i < 21 ; ++i)
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

void loop() {
  static uint8_t state = 0;

  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if (currentMillis - prevUpdate >= intervalUpdate)
  {
    prevUpdate = currentMillis;
    if (state)
      memset(MegaArray, 0xff, 12);
      else
      memset(MegaArray, 0, 12);
    /*memset(MegaArray, 0, 12);
    for (uint8_t i =0; i < 12; i++)
    {
      MegaArray[i]  = (1 << (state& 0x07));
    }*/
    state++;
    if (state > 1)
    {
      state = 0;
    }
    writeArray(MegaArray, 12);
  }



}
