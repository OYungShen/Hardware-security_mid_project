const byte set_mode = 2; // MODE PIN
const byte send_req = 4; // REQUEST PIN
const byte ready_flag = 3; // GETT READY
const byte valid_flag = 5; // GET VALID
const byte pin6 = 6;
const byte pin7 = 7;
const byte pin8 = 8;
const byte pin9 = 9;
const byte pin10 = 10;
const byte pin11 = 11;
const byte pin12 = 12;
const byte pin13 = 13;
int command_str;
bool PUT_mode = true;
bool flag = true;
byte PUF_code1[512];
byte PUF_code2[512];
byte RN;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(set_mode, OUTPUT);
  pinMode(send_req, OUTPUT);
  pinMode(ready_flag, INPUT);
  pinMode(valid_flag, INPUT);
  pinMode(pin6, INPUT);
  pinMode(pin7, INPUT);
  pinMode(pin8, INPUT);
  pinMode(pin9, INPUT);
  pinMode(pin10, INPUT);
  pinMode(pin11, INPUT);
  pinMode(pin12, INPUT);
  pinMode(pin13, INPUT);
  
  digitalWrite(set_mode, LOW);
  digitalWrite(send_req, LOW);
}

void read_PUF(byte PUF_code[512]){
  int count = 0;
  int state = 0;
  int ready_state = 0;
  int valid_state = 0;
  
  while (count < 512) {
    PUF_code[count] &= 0x0; // init byte
    
    if (state == 0) { // Get ready pin and send request
      ready_state = digitalRead(ready_flag); 
      if (ready_state == HIGH) { // if ready 
        flag = true; // For next state
        state = 1; // For next state
        digitalWrite(send_req, HIGH); // send request
        digitalWrite(send_req, LOW);
        // digitalWrite(send_req, HIGH); 
        // Serial.print("Ready\n");

        
        // Get valid and read data
        while (flag){
          valid_state = digitalRead(valid_flag);
          if (valid_state == HIGH) { // if valid
            // Serial.print("Valid\n");
            // read data
            PUF_code[count] |= digitalRead(pin6);
            PUF_code[count] |= digitalRead(pin7) << 1;
            PUF_code[count] |= digitalRead(pin8) << 2;
            PUF_code[count] |= digitalRead(pin9) << 3;
            PUF_code[count] |= digitalRead(pin10) << 4;
            PUF_code[count] |= digitalRead(pin11) << 5;
            PUF_code[count] |= digitalRead(pin12) << 6;
            PUF_code[count] |= digitalRead(pin13) << 7;
            count = count + 1; // next byte
            flag = false;
            state = 0;
            // Serial.print(count);
          }
          
        }
      }
    } 
  }
}

void read_RN() {
  int state = 0;
  int ready_state = 0;
  int valid_state = 0;
  bool finish = false;
  
  RN &= 0x0; // init byte

  while (!finish){
    if (state == 0) { // Get ready and send request
      ready_state = digitalRead(ready_flag);
      if (ready_state == HIGH) {
        // Serial.print("Ready\n");
        state = 1;
        digitalWrite(send_req, HIGH);
        digitalWrite(send_req, LOW);
      }
    } else { // Get valid and read data
      valid_state = digitalRead(valid_flag);
      if (valid_state == HIGH) {
        // Serial.print("Valid\n");
        state = 0;
        
        RN |= digitalRead(pin6);
        RN |= digitalRead(pin7) << 1;
        RN |= digitalRead(pin8) << 2;
        RN |= digitalRead(pin9) << 3;
        RN |= digitalRead(pin10) << 4;
        RN |= digitalRead(pin11) << 5;
        RN |= digitalRead(pin12) << 6;
        RN |= digitalRead(pin13) << 7;
        finish = true;
      }
    }
  }
}

void check() {
  int i;
  bool test = true;
  
  read_PUF(PUF_code1);
  read_PUF(PUF_code2);

  for(i = 0; i < 512; i++) {
    if (PUF_code1[i] != PUF_code2[i])
      test = false;
  }

  if (test) {
    Serial.print("Correct\n");
  } else {
    Serial.print("False\n");
  }

}

void print_4bytes(byte PUF_code[512]) {
  int i, j;

  for (i = 0; i < 128; i++) {
    Serial.print("  ");
    for (j = 0; j < 4; j++) {
      if (PUF_code[i * 4 + j] < 16)
        Serial.print("0");
      Serial.print(PUF_code[i * 4 + j], HEX);
    }
    Serial.print("\n");
  }
}

void loop() {
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    command_str = Serial.read();
    if (command_str == 'P') { // PUF mode
      digitalWrite(set_mode, LOW);
      Serial.print("Enter PUF mode\n");
      PUT_mode = true;
    } else if (command_str == 'T'){ // TRN mode
      digitalWrite(set_mode, HIGH);
      Serial.print("Enter TRG mode\n");
      PUT_mode = false;
    }
    
    if (PUT_mode) { // read data
      if (command_str == 'R') {
        read_PUF(PUF_code1);
        // check();
        Serial.print("PUF code : \n");
        print_4bytes(PUF_code1);
        
      }
    } else { // read data
      if (command_str == 'R') {
        read_RN();
        Serial.print("Read TRG\n");
        Serial.print(RN);
        Serial.print("\n");
      }
    }
  }
}
