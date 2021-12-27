#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string.h>
#include <math.h>

using namespace std;

/*** BASE45 LIB C++ ***\
  The Alphanumeric mode is defined to use 45 characters as specified in
   this alphabet.

                  Table 1: The Base45 Alphabet

  Value Encoding  Value Encoding  Value Encoding  Value Encoding
         0 A            17 R            34 i            51 z
         1 B            18 S            35 j            52 0
         2 C            19 T            36 k            53 1
         3 D            20 U            37 l            54 2
         4 E            21 V            38 m            55 3
         5 F            22 W            39 n            56 4
         6 G            23 X            40 o            57 5
         7 H            24 Y            41 p            58 6
         8 I            25 Z            42 q            59 7
         9 J            26 a            43 r            60 8
        10 K            27 b            44 s            61 9
        11 L            28 c            45 t            62 +
        12 M            29 d            46 u            63 /
        13 N            30 e            47 v
        14 O            31 f            48 w            64 =
        15 P            32 g            49 x
        16 Q            33 h            50 y
***/


#define SEXTET 63
#define OCTET 255

#define NO_PADDING 3
#define ONE_PADDING 2
#define TWO_PADDING 1

class Base64 {

  unsigned char swap_table(int byte) {
    char return_char;
    if (byte >= 0 && byte <= 25) {
      return_char = byte + 65;
      return return_char;
    }

    if (byte >= 26 && byte <= 51) {
      return_char = byte + 71;
      return return_char;
    }

    if (byte >= 52 && byte <= 61) {
      return_char = byte - 4;
      return return_char;
    }

    switch(byte) {
      case 62: {
        return_char = byte - 19;
        return return_char;
      } case 63: {
        return_char = byte - 16;
        return return_char;
      } case 64: {
        return_char = byte - 3;
        return return_char;
      } default: {
        return byte;
      }
      
    }
    
  }

  int swap_back_table(int byte) {
    char return_char;

    if (byte >= 48 && byte <= 57) {
      return_char = byte + 4;
      return return_char;
    }

    if (byte >= 65 && byte <= 90) {
      return_char = byte - 65;
      return return_char;
    }

    if (byte >= 97 && byte <= 122) {
      return_char = byte - 71;
      return return_char;
    }

    switch(byte) {
      case 43: {
        return_char = byte + 19;
        return return_char;
      } case 47: {
        return_char = byte + 16;
        return return_char;
      } case 61: {
        return_char = byte + 3;
        return return_char;
      } default: {
        return byte;
      }
    }  
    
  }


  public:

  vector<unsigned char> encode(vector<unsigned char> data) {
    vector<vector<unsigned int>> first_group_bits;

    for (int i = 0; i < data.size(); i) {
      vector<unsigned int> group;

      while(group.size() != 3 && data[i]) {
        group.push_back(data[i]);
        i++;
      }
      
      first_group_bits.push_back(group);
    }

    vector<vector<int>> second_group_bits;

    for (int i = 0; i < first_group_bits.size(); i++) {
      unsigned int joined = 0;
      int number_of_octets = first_group_bits[i].size();

      for (int j = 0; j < number_of_octets; j++) {
        joined |= first_group_bits[i][j];

        if (j != number_of_octets - 1)
          joined = joined << 8;
      }

      vector<int> sextets;
      switch(number_of_octets) {
        case NO_PADDING: {
          for (int step = NO_PADDING; step >= 0; step--) {
            int first = (joined >> (step*6)) & SEXTET;
            sextets.push_back(first);
          }

          break;
        } case ONE_PADDING: {
          int first = joined >> 10 & SEXTET;
          int second = joined >> 4 & SEXTET;
          int third = joined << 2 & SEXTET;
          sextets.push_back(first);
          sextets.push_back(second);
          sextets.push_back(third);
          sextets.push_back(64);
          break;
        } case TWO_PADDING: {
          int first = joined >> 2 & SEXTET;
          int second = joined << 4 & SEXTET;
          sextets.push_back(first);
          sextets.push_back(second);
          sextets.push_back(64);
          sextets.push_back(64);
          break;
        }
      
        default: 
          break;
      } 

      second_group_bits.push_back(sextets);
    }

    int counter = 0;
    vector<unsigned char> encoded_string;
    for (int i = 0; i < second_group_bits.size(); i++) {
      for (int j = 0; j < second_group_bits[i].size(); j++) {
        encoded_string.push_back(swap_table(second_group_bits[i][j]));
        counter++;
      }
    }

    return encoded_string;
  }

  vector<unsigned char> decode(vector<unsigned char> data) {
    vector<vector<int>> group_return_bits; 

    for (int i = 0; i < data.size(); i) {
      vector<int> group;
      while(group.size() != 4 && data[i]) {
        int swap_back_data = swap_back_table(data[i]);
        if (swap_back_data != 64)
          group.push_back(swap_back_data);
        i++;
      }

      group_return_bits.push_back(group);
    }
  

    vector<vector<unsigned char>> group_return_octets;

    for (int i = 0; i < group_return_bits.size(); i++) {
      unsigned int joined = 0;
      int number_of_sextets = group_return_bits[i].size();
      for (int j = 0; j < number_of_sextets; j++) {
        joined |= group_return_bits[i][j];
        if (j != number_of_sextets-1) 
          joined = joined << 6;
      }

      vector<unsigned char> octets;
      switch(number_of_sextets) {
        case 4: {
          for (int step = number_of_sextets/2; step >= 0; step--) {
            unsigned char first = (joined >> (8*step)) & OCTET;
            octets.push_back(first);
          }

          break;
        } case 3: {
          unsigned char first = (joined >> 10) & OCTET;
          unsigned char second = (joined >> 2) & OCTET;
          octets.push_back(first);
          octets.push_back(second);
          break;
          
        } case 2: {
          unsigned char first = (joined >> 4) & OCTET;
          octets.push_back(first);
          break;
        }
      }

      group_return_octets.push_back(octets);
    } 

    int counter = 0;
    vector<unsigned char> decoded_string;
    for (int i = 0; i < group_return_octets.size(); i++) {
      for (int j = 0; j < group_return_octets[i].size(); j++) {
        decoded_string.push_back(group_return_octets[i][j]);
        counter++;
      }
    }

    return decoded_string;
  }

};

const char *arg_list[] = {"-d", "-e"};

int main(int argc, char **argv) {
  string arg;
  Base64 base64; 
  vector<unsigned char> input;
  vector<unsigned char> output;

  /** Get args*/
  if (argc < 3) {
    cout << "No arguments supplied, -e for encode & -d for decode\n";
    return 1;
  }

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[1], arg_list[i]) == 0) {
      arg = argv[1];
      for (int i = 0; i < strlen(argv[2]); i++) {
        input.push_back(argv[2][i]);
      }
      break;
    }
  }

  if (arg.length() == 0) {
    cout << "Unknown args, -e for encode & -d for decode\n";
    return 1;
  } 

  
  if (arg == "-e") {
    output = base64.encode(input);
  } else if (arg == "-d") {
    output = base64.decode(input);
  }
  
  for (int i = 0; i < output.size(); i++) {
    cout << output[i];
  }

  cout << "\n";

  return 0;
}