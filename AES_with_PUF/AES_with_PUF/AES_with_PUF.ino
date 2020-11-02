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
int temp_char;
bool PUT_mode = true;
bool flag = true;
byte PUF_code[512];
byte RN;
unsigned char plaintext[4][4];
unsigned char cipherkey[4][4];

unsigned char S_Box[16][16] = {
  {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 
   0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
  {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 
   0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
  {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 
   0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
  {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 
   0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
  {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 
   0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
  {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 
   0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
  {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 
   0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
  {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 
   0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
  {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 
   0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
  {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 
   0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
  {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 
   0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
  {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 
   0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
  {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 
   0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
  {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 
   0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
  {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 
   0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
  {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 
   0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};

unsigned char Inv_S_Box[16][16] = {
  {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 
   0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
  {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 
   0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
  {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 
   0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
  {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 
   0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
  {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 
   0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
  {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 
   0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
  {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 
   0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
  {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 
   0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
  {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 
   0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
  {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 
   0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
  {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 
   0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
  {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 
   0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
  {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 
   0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
  {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 
   0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
  {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 
   0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
  {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 
   0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
};

unsigned char Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

void read_PUF();
void read_RN();
void print_state(unsigned char state[4][4]);
void decrypt(unsigned char plaintext[4][4], unsigned char cipher_key[4][4]);
void encrypt(unsigned char plaintext[4][4], unsigned char cipher_key[4][4]);
void inv_keyschedule(unsigned char key[4][4], int round);
void keyschedule(unsigned char key[4][4], int round);
void addroundkey(unsigned char state[4][4], unsigned char roundkey[4][4]);
void inv_mixcolumns(unsigned char state[4][4]);
void mixcolumns(unsigned char state[4][4]);
unsigned char GFMul(unsigned char a, unsigned char b);
void inv_shiftrows(unsigned char state[4][4]);
void shiftrows(unsigned char state[4][4]);
void inv_sub_bytes(unsigned char state[4][4]);
void sub_bytes(unsigned char state[4][4]);


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


void loop() {
  int row, col;
  
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
        int i;
        
        read_PUF();
        for (i = 0; i < 16; i++) {
          cipherkey[int(i % 4)][int(i / 4)] = (unsigned char)PUF_code[i]; 
        }
      }
    } else { // read data
      if (command_str == 'R') {
        read_RN();
        Serial.print("Read TRG\n");
        Serial.print(RN);
        Serial.print("\n");
      }
    }
    /*
     *  Read plaintext and Encrypt
     */
    if (command_str == 'E') {
      // read plaintext
      int count = 0;
      while (count < 16) {
        if (Serial.available() > 0) {
          temp_char = Serial.read();
          if (temp_char != 0x0d && temp_char != '\n')
            plaintext[int(count % 4)][int(count / 4)] = temp_char;
          count = count + 1;
        }
      }
      Serial.print("Input text\n");
      print_state(plaintext);
      // encrypt
      encrypt(plaintext, cipherkey);
      Serial.print("Encrypted text\n");
      print_state(plaintext);
    }
    if (command_str == 'D') {
      decrypt(plaintext, cipherkey);
      Serial.print("Original text\n");
      print_state(plaintext);
    }
    
  }
}

void read_PUF(){
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

void sub_bytes(unsigned char state[4][4]) {
    unsigned char row, col;
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            row = (state[i][j] >> 4);
            col = (state[i][j] & (0x0f));
            state[i][j] = S_Box[row][col];
        }
    }
}

void inv_sub_bytes(unsigned char state[4][4]) {
    unsigned char row, col;
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            row = (state[i][j] >> 4);
            col = (state[i][j] & (0x0f));
            state[i][j] = Inv_S_Box[row][col];
        }
    }

}

void shiftrows(unsigned char state[4][4]) {
    unsigned char temp1, temp2;
    int row, col;
    
    // second row
    temp1 = state[1][0];
    for (col = 0; col < 3; col++) {
        state[1][col] = state[1][col + 1]; 
    }
    state[1][3] = temp1;

    // third row
    col = 0;
    temp1 = state[2][col];
    state[2][col] = state[2][col + 2];
    state[2][col + 2] = temp1;

    col = 1;
    temp1 = state[2][col];
    state[2][col] = state[2][col + 2];
    state[2][col + 2] = temp1;

    // fourth row
    temp1 = state[3][3];
    for (col = 3; col > 0; col--) {
        state[3][col] = state[3][col - 1]; 
    }
    state[3][0] = temp1;
}

void inv_shiftrows(unsigned char state[4][4]) {
    unsigned char temp1, temp2;
    int row, col;
    
    // second row
    temp1 = state[1][3];
    for (col = 3; col > 0; col--) {
        state[1][col] = state[1][col - 1]; 
    }
    state[1][0] = temp1;

    // third row
    col = 0;
    temp1 = state[2][col];
    state[2][col] = state[2][col + 2];
    state[2][col + 2] = temp1;

    col = 1;
    temp1 = state[2][col];
    state[2][col] = state[2][col + 2];
    state[2][col + 2] = temp1;

    // fourth row
    temp1 = state[3][0];
    for (col = 0; col < 3; col++) {
        state[3][col] = state[3][col + 1]; 
    }
    state[3][3] = temp1;
}

unsigned char GFMul(unsigned char a, unsigned char b) { 
  unsigned char p = 0;
  unsigned char hi_bit_set;
    int i;

  for (i = 0; i < 8; i++) {
    if ((b & 1) != 0) {
      p ^= a;
    }
    hi_bit_set = (a & (0x80));
    a <<= 1;
    if (hi_bit_set != 0) {
      a ^= 0x1b; 
    }
    b >>= 1;
  }
  return p;
}

void mixcolumns(unsigned char state[4][4]) {
    int col, i;
    unsigned char temp[4];

    for (col = 0; col < 4; col++) {
        for (i = 0; i < 4; i++)
            temp[i] = state[i][col];

        state[0][col] = GFMul(2, temp[0]) ^ GFMul(3, temp[1]) ^ temp[2] ^ temp[3];
        state[1][col] = temp[0] ^ GFMul(2, temp[1]) ^ GFMul(3, temp[2]) ^ temp[3];
        state[2][col] = temp[0] ^ temp[1] ^ GFMul(2, temp[2]) ^ GFMul(3, temp[3]);
        state[3][col] = GFMul(3, temp[0]) ^ temp[1] ^ temp[2] ^ GFMul(2, temp[3]);
    }
}

void inv_mixcolumns(unsigned char state[4][4]) {
    int col, i;
    unsigned char temp[4];

    for (col = 0; col < 4; col++) {
        for (i = 0; i < 4; i++)
            temp[i] = state[i][col];

        state[0][col] = GFMul(0x0e, temp[0]) ^ GFMul(0x0b, temp[1]) ^ GFMul(0x0d, temp[2])  ^ GFMul(0x09, temp[3]);
        state[1][col] = GFMul(0x09, temp[0]) ^ GFMul(0x0e, temp[1]) ^ GFMul(0x0b, temp[2])  ^ GFMul(0x0d, temp[3]);
        state[2][col] = GFMul(0x0d, temp[0]) ^ GFMul(0x09, temp[1]) ^ GFMul(0x0e, temp[2])  ^ GFMul(0x0b, temp[3]);
        state[3][col] = GFMul(0x0b, temp[0]) ^ GFMul(0x0d, temp[1]) ^ GFMul(0x09, temp[2])  ^ GFMul(0x0e, temp[3]);
    }
}

void addroundkey(unsigned char state[4][4], unsigned char roundkey[4][4]) {
    int i,j;
     
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            state[i][j] = state[i][j] ^ roundkey[i][j];
        }
    }
}

void keyschedule(unsigned char key[4][4], int round) {
    int i, j, row, col;

    for (i = 0; i < 4; i++) {
        row = (key[(i + 1) % 4][3] >> 4); // rotate row
        col = (key[(i + 1) % 4][3] & (0x0f)); // rotate row
        key[i][0] ^= S_Box[row][col];

        if (i == 0) // first row
            key[i][0] ^= Rcon[round - 1]; 
    }

    for (col = 1; col < 4; col++) {
        for (row = 0; row < 4; row++) {
            key[row][col] ^= key[row][col - 1];
        }
    }
}

void inv_keyschedule(unsigned char key[4][4], int round) {
    int i, j, row, col;

    for (col = 3; col > 0; col--) {
        for (row = 0; row < 4; row++) {
            key[row][col] ^= key[row][col - 1];
        }
    }

    for (i = 0; i < 4; i++) {
        row = (key[(i + 1) % 4][3] >> 4); // rotate row
        col = (key[(i + 1) % 4][3] & (0x0f)); // rotate row
        key[i][0] ^= S_Box[row][col];

        if (i == 0) // first row
            key[i][0] ^= Rcon[round - 1]; 
    }
}

void encrypt(unsigned char plaintext[4][4], unsigned char cipher_key[4][4]) {
    int round;

    addroundkey(plaintext, cipher_key);

    for (round = 1; round < 10; round++) {
        sub_bytes(plaintext);
        shiftrows(plaintext);
        mixcolumns(plaintext);
        keyschedule(cipher_key, round);
        addroundkey(plaintext, cipher_key);

    }

    sub_bytes(plaintext);
    shiftrows(plaintext);
    keyschedule(cipher_key, 10);
    addroundkey(plaintext, cipher_key);


}

void decrypt(unsigned char plaintext[4][4], unsigned char cipher_key[4][4]) {
    int round;

    addroundkey(plaintext, cipher_key);
    inv_keyschedule(cipher_key, 10);


    for (round = 10; round > 1; round--) {
        inv_shiftrows(plaintext);
        inv_sub_bytes(plaintext);
        addroundkey(plaintext, cipher_key);
        inv_keyschedule(cipher_key, round - 1);
        inv_mixcolumns(plaintext);
    }
    inv_shiftrows(plaintext);
    inv_sub_bytes(plaintext);
    addroundkey(plaintext, cipher_key);

}

void print_state(unsigned char state[4][4]) {
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            Serial.print(char(state[i][j]));
            Serial.print(",");
        }
        Serial.print("\n");
    }
}
