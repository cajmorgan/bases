#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string.h>
#include <math.h>
#include <sstream>

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

  unsigned char swapBackTable(unsigned char byte) {
    if (byte >= 48 && byte <= 57) {
      return byte - 48;
    }

    if (byte > 64 && byte < 91) {
      return byte - 55;
    }

    switch(byte) {
      case ' ':
        return 36;
      case '$':
        return 37;
      case '%': 
        return 38;
      case '*':
        return 39;
      case '+':
        return 40;
      case '-': 
        return 41;
      case '.':
        return 42;
      case '/':
        return 43;
      case ':':
        return 44;
      default:
        return byte;
    }

  }

  /** ENCODER **/
  vector<unsigned char> encode(vector<unsigned char> data) {
    vector<unsigned int> bytes;
    int counter = 0;
    int byteIndex = 0;
    while(counter < data.size()) {
      unsigned int byte = 0;
      if (counter == data.size() - 1)
        byte = data[counter];
      else
        byte = (data[counter] * 256) + data[counter + 1];
      
      bytes.push_back(byte);
      cout << byte;
      cout << "\n";
      counter += 2;
    }

    vector<array<unsigned char, 3>> remainders;
    for (int i = 0; i < bytes.size(); i++) {
      array<unsigned char, 3 > remainderTriples = {0};
      unsigned int byte = bytes[i];
      for (int j = 0; j < 3; j++) {
        unsigned char remainder = byte % 45;
        byte = (byte - remainder) / 45;
        remainderTriples[j] = remainder;
      }

      remainders.push_back(remainderTriples);
    }

    vector<unsigned char> result;
    for(int i = 0; i < remainders.size(); i++) {
      for (int j = 0; j < 3; j++) {
        if (i == remainders.size() -1 && remainders[i][2] == 0) {
          if (remainders[i][j] == 0)
            break;
        }
        result.push_back(swapTable(remainders[i][j]));

      }

    }

    return result;
  }

  /** DECODER **/
  vector<unsigned char> decode(vector<unsigned char> data) {
    bool breakIt = false;
    vector<array<unsigned char, 3>> remainders; 
    for (int i = 0; i < data.size(); i) {
      array<unsigned char, 3> remaindersTriples = {0};
      /** Clean out zeros**/
      for (int j = 0; j < 3; j++) {
        remaindersTriples[j] = swapBackTable(data[i]);
        i++;
      }

      remainders.push_back(remaindersTriples);
    }
   
    vector<unsigned int> byteSequence;
    for (int i = 0; i < remainders.size(); i++) {
      unsigned int returnByte = 0;
      for (int j = 0; j < 3; j++) {
        returnByte += remainders[i][j] * pow(45, j);
      }

      byteSequence.push_back(returnByte);
    }

    vector<unsigned char> restoredBytes;
    for (int i = 0; i < byteSequence.size(); i += 1) {
      unsigned char remainder = byteSequence[i] % 256;
      unsigned char firstToPush = (byteSequence[i] - remainder) / 256;
      if (firstToPush != 0)
        restoredBytes.push_back(firstToPush);
      restoredBytes.push_back(remainder);
    }

    return restoredBytes;
  }
};