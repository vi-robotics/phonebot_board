/* SerialPassthrough
    Written by Brian Schmalz of Schmalz Haus LLC (brian@schmalzhaus.com) for Microchip, Dec 2017

    This sketch simply provides a way to directly talk to an RN4870 or RN4871 module. It assumes that
    the USB serial connection is Serial, and the serial connection to the BLE module is on Serial1.

    If you open up a terminal emulator like TeraTerm and connect to your board's serial port,  you can
    talk through the microcontroller to the BLE module. This is very useful for figuring out what commands
    the BLE module understand, and debugging connection problems.
*/

/* These pins are correct for RGB Station boards. Adjust to fit your hardware. */
#include <Servo.h>

#define BLE_MODE_PIN  4
#define BLE_RESET_PIN 12
#define MAX_BUF (128)

#define SERIAL_HOST Serial
#define SERIAL_BLE  Serial1

#define CMD_HEADER '['
#define CMD_FOOTER ']'
#define NUM_JOINTS (8)

int  buf_idx = 0;
char data[MAX_BUF] = {};
bool recv = false;
int jval[NUM_JOINTS] = {};
const int spins[] = {6, 2, 3, 13, 5, 10, 9, 8};
Servo servos[NUM_JOINTS] = {};

void setup() {
  // configure servos
  for (int i = 0; i < NUM_JOINTS; ++i) {
    pinMode( spins[i], OUTPUT);
    servos[i].attach(spins[i]);
  }

  // configure ble
  pinMode(BLE_MODE_PIN, OUTPUT);
  digitalWrite(BLE_MODE_PIN, HIGH);
  pinMode(BLE_RESET_PIN, OUTPUT);
  digitalWrite(BLE_RESET_PIN, LOW);
  delay(100); //
  digitalWrite(BLE_RESET_PIN, HIGH);
  delay(100); //settle

  // configure serial
  SERIAL_HOST.begin(115200);
  SERIAL_BLE.begin(57600);

  while (!SERIAL_HOST || !SERIAL_BLE) {
    ; // wait for both serial ports to connect.
  }

  // initialize ble + begin service
  //  SERIAL_BLE.write("$$$");
  //  SERIAL_BLE.write("SS,C0\r");
  //  SERIAL_BLE.write("---\r");
}

void execute() {
  // cmd format = [??? ??? ??? ??? ???]

  int tmp[NUM_JOINTS] = {};
  int jidx = 0;

  // interpret command
  for (int i = 0; i < buf_idx; ++i) {
    if (data[i] == ' ') {
      ++jidx;
      continue;
    }
    int v = data[i] - '0';
    if (v < 0 || v > 9) break;

    tmp[jidx] *= 10;
    tmp[jidx] += v;
  }

  // invalid command - return
  if (jidx != NUM_JOINTS) return;
  //SERIAL_HOST.println("SUCCESS");

  // copy joint values
  for (int i = 0; i < NUM_JOINTS; ++i) {
//    SERIAL_HOST.print(tmp[i]);
//    SERIAL_HOST.print(',');
    jval[i] = tmp[i];
  }
//  SERIAL_HOST.println("");
}

void loop() // run over and over
{
  if (SERIAL_HOST.available())
    SERIAL_BLE.write(SERIAL_HOST.read());

  while (SERIAL_BLE.available()) {
    char b = SERIAL_BLE.read();
    SERIAL_HOST.write(b);
    if (recv) {
      if (buf_idx < MAX_BUF) {
        data[buf_idx] = b;
        ++buf_idx;
      } else {
        // buffer overflow - clear flag
        recv = false;
      }
    }
    if (b == CMD_HEADER) {
      // TODO : more robust header parsing
      recv = true;
      buf_idx = 0;
    }
    if (b == CMD_FOOTER) {
      // TODO :
      recv = false;
      execute();
    }
  }

  for (int i = 0; i < NUM_JOINTS; ++i) {
    servos[i].write( jval[i] );
  }
}

