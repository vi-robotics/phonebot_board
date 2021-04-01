#ifndef __BLE_H__
#define __BLE_H__

//#include "common.h"
#define BAUD_HOST (115200)
#define BAUD_BLE  (57600)
#define SERIAL_HOST Serial
#define SERIAL_BLE Serial1

#define BLE_MODE_PIN  4
#define BLE_RESET_PIN 12

#define CMD_MAX_BUF (128)
#define CMD_HEADER ('[')
#define CMD_FOOTER (']')
#define CMD_DELIM  (' ')

class BLE {
    friend class PhoneBot;
    //bool passthrough_ = true;
    int buf_idx_      = 0;
    bool recv_ = false; // currently receiving
    byte data_[CMD_MAX_BUF];
    typedef void (*msg_cb_t)(const char*);
    msg_cb_t msg_cb;
    
  public:
    BLE(msg_cb_t msg_cb):msg_cb(msg_cb) {

    }
    void begin() {
      // set mode + reset
      // currently disabled?
      //      pinMode(BLE_MODE_PIN, OUTPUT);
      //      digitalWrite(BLE_MODE_PIN, HIGH);
      //      pinMode(BLE_RESET_PIN, OUTPUT);
      //      digitalWrite(BLE_RESET_PIN, LOW);
      //      delay(100); //
      //      digitalWrite(BLE_RESET_PIN, HIGH);
      //      delay(100); //settle

      // TODO : figure out if it is necessary to initialize the robot
      // by setting the baud rate

      SERIAL_HOST.begin(BAUD_HOST);
      SERIAL_BLE.begin(BAUD_BLE);
      while (!SERIAL_BLE) {
        ;
      }

      // initialize RN4871:
      // cmd mode -> enable service -> passthrough
      delay( 500 );
      SERIAL_BLE.write("$$$");
      delay( 500 );
      SERIAL_BLE.write("SS,C0\r");
      delay( 500 );
      SERIAL_BLE.write("---\r");
      delay( 500 );
    }

    void recv(byte b) {
      switch (b) {
        case CMD_HEADER:
          // control: cmd begin
          recv_ = true;
          buf_idx_ = 0;
          return;
        case CMD_FOOTER:
          // control: cmd end
          recv_ = false;
          data_[buf_idx_] = '\0'; // terminate data
          msg_cb(data_);
          return;
        default:
          // data: fill buf
          break;
      }

      if (!recv_) return;

      // handle data input

      if (buf_idx_ < CMD_MAX_BUF) {
        data_[buf_idx_] = b;
        ++buf_idx_;
      } else {
        recv_ = false;
      }

    }
    void step() {
      if (SERIAL_HOST.available()) {
        // always passthrough host-->ble
        SERIAL_BLE.write( SERIAL_HOST.read() );
      }
      if ( SERIAL_BLE.available() ) {
        byte b = SERIAL_BLE.read();
        SERIAL_HOST.write(b);
        recv(b);
      }
    }

};

#endif
