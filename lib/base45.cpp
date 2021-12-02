#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <string.h>
#include <math.h>

/*** BASE45 LIB C++ ***\
  The Alphanumeric mode is defined to use 45 characters as specified in
   this alphabet.

                  Table 1: The Base45 Alphabet

   Value Encoding  Value Encoding  Value Encoding  Value Encoding
      00 0            12 C            24 O            36 Space
      01 1            13 D            25 P            37 $
      02 2            14 E            26 Q            38 %
      03 3            15 F            27 R            39 *
      04 4            16 G            28 S            40 +
      05 5            17 H            29 T            41 -
      06 6            18 I            30 U            42 .
      07 7            19 J            31 V            43 /
      08 8            20 K            32 W            44 :
      09 9            21 L            33 X
      10 A            22 M            34 Y
      11 B            23 N            35 Z
***/

class Base45 {
  public:


  void encode(char data[]) {
    /** Make vector */
    std::vector<int> bytes;
    std::vector<std::array<int, 3>> remainders;
    int length = strlen(data);
    int counter = 0;
    int byteIndex = 0;
    while(counter != length) {
      int byte = 0;

      if (counter == length - 1) {
        byte = data[counter];
        bytes.push_back(byte);
        break;
      }

      byte = (data[counter] * 256) + data[counter + 1];
      bytes.push_back(byte);
      counter += 2;
    }

    for (int i = 0; i < bytes.size(); i++) {
      std::array<int, 3> remainderTriples = {0};
      int byte = bytes[i];
      for (int j = 0; j < 3; j++) {
        int remainder = byte % 45;
        byte = (byte - remainder) / 45;
        remainderTriples[j] = remainder;
      }

      remainders.push_back(remainderTriples);
    }

    for(int i = 0; i < remainders.size(); i++) {
      for (int j = 0; j < 3; j++) {
        std::cout << remainders[i][j] << " ";
      }
      std::cout << "\n";
    }
     
  }

  void decode() {

  }
};


int main() {
  char input[] = "Hello!!";
  Base45 base45;
  base45.encode(input);
  return 0;
}