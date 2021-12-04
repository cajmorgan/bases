#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <string.h>
#include <math.h>

using namespace std;

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

  int swapTable(int byte) {
    if (byte >= 0 && byte <= 9) {
      return byte + 48;
    }

    if (byte > 9 && byte < 36) {
      return byte + 55;
    }

    switch(byte) {
      case 36:
        return ' ';
      case 37:
        return '$';
      case 38: 
        return '%';
      case 39:
        return '*';
      case 40:
        return '+';
      case 41: 
        return '-';
      case 42:
        return '.';
      case 43:
        return '/';
      case 44:
        return ':';
      default:
        return byte;
    }

  }

  void encode(string data) {
    /** Make vector */
    vector<int> bytes;
    int counter = 0;
    int byteIndex = 0;
    while(counter != data.length()) {
      int byte = 0;

      if (counter == data.length() - 1) {
        byte = data[counter];
        bytes.push_back(byte);
        break;
      }

      byte = (data[counter] * 256) + data[counter + 1];
      bytes.push_back(byte);
      counter += 2;
    }

    vector<std::array<int, 3>> remainders;
    for (int i = 0; i < bytes.size(); i++) {
      array<int, 3> remainderTriples = {0};
      int byte = bytes[i];
      for (int j = 0; j < 3; j++) {
        int remainder = byte % 45;
        byte = (byte - remainder) / 45;
        remainderTriples[j] = remainder;
      }

      remainders.push_back(remainderTriples);
    }

    vector<int> result;
    for(int i = 0; i < remainders.size(); i++) {
      for (int j = 0; j < 3; j++) {
        result.push_back(swapTable(remainders[i][j]));
      }
    }

    result.pop_back();
    for (int i = 0; i < result.size(); i++) {
      cout << (char)result[i];
    }
     
  }

  void decode() {

  }
};


int main() {
  string input ("Hello!! sup");
  Base45 base45;
  base45.encode(input);
  return 0;
}