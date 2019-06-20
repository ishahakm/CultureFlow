#include <iostream>
using namespace std;

const int rows = 8;
const int columns = 12;
const int numLocations = ((rows * columns) / 3) + 1;
const int stepsPerWell = 225;
const int stepsToEject = 500;

void populateLocations(long (&locations)[numLocations][2], int choice = 1){
  if (choice == 1){
    //Location for Triplet Well #1
    locations[0][0] = 0;
    locations[0][1] = 0;

    //Location for Triplet Well #2
    locations[1][0] = 225;
    locations[1][1] = 0;

    //Location for Triplet Well #3
    locations[2][0] = 450;
    locations[2][1] = 0;

    //Location for Triplet Well #4
    locations[3][0] = 675;
    locations[3][1] = 0;

    //Location for Triplet Well #5
    locations[4][0] = 900;
    locations[4][1] = 0;

    //Location for Triplet Well #6
    locations[5][0] = 1125;
    locations[5][1] = 0;

    //Location for Triplet Well #7
    locations[6][0] = 1350;
    locations[6][1] = 0;

    //Location for Triplet Well #8
    locations[7][0] = 1575;
    locations[7][1] = 0;

    //Location for Triplet Well #9
    locations[8][0] = 1575;
    locations[8][1] = -675;

    //Location for Triplet Well #10
    locations[9][0] = 1350;
    locations[9][1] = -675;

    //Location for Triplet Well #11
    locations[10][0] = 1125;
    locations[10][1] = -675;

    //Location for Triplet Well #12
    locations[11][0] = 900;
    locations[11][1] = -675;

    //Location for Triplet Well #13
    locations[12][0] = 675;
    locations[12][1] = -675;

    //Location for Triplet Well #14
    locations[13][0] = 450;
    locations[13][1] = -675;

    //Location for Triplet Well #15
    locations[14][0] = 225;
    locations[14][1] = -675;

    //Location for Triplet Well #16
    locations[15][0] = 0;
    locations[15][1] = -675;

    //Location for Triplet Well #17
    locations[16][0] = 0;
    locations[16][1] = -1350;

    //Location for Triplet Well #18
    locations[17][0] = 225;
    locations[17][1] = -1350;

    //Location for Triplet Well #19
    locations[18][0] = 450;
    locations[18][1] = -1350;

    //Location for Triplet Well #20
    locations[19][0] = 675;
    locations[19][1] = -1350;

    //Location for Triplet Well #21
    locations[20][0] = 900;
    locations[20][1] = -1350;

    //Location for Triplet Well #22
    locations[21][0] = 1125;
    locations[21][1] = -1350;

    //Location for Triplet Well #23
    locations[22][0] = 1350;
    locations[22][1] = -1350;

    //Location for Triplet Well #24
    locations[23][0] = 1575;
    locations[23][1] = -1350;

    //Location for Triplet Well #25
    locations[24][0] = 1575;
    locations[24][1] = -2025;

    //Location for Triplet Well #26
    locations[25][0] = 1350;
    locations[25][1] = -2025;

    //Location for Triplet Well #27
    locations[26][0] = 1125;
    locations[26][1] = -2025;

    //Location for Triplet Well #28
    locations[27][0] = 900;
    locations[27][1] = -2025;

    //Location for Triplet Well #29
    locations[28][0] = 675;
    locations[28][1] = -2025;

    //Location for Triplet Well #30
    locations[29][0] = 450;
    locations[29][1] = -2025;

    //Location for Triplet Well #31
    locations[30][0] = 225;
    locations[30][1] = -2025;

    //Location for Triplet Well #32
    locations[31][0] = 0;
    locations[31][1] = -2025;

    //Location for Eject
    locations[32][0] = locations[numLocations - 2][0] + stepsToEject;
    locations[32][1] = -((columns / 3) - 1) * (3 * stepsPerWell);

  }
  else {
    int tripletCol = 1;
    for (int i = 0; i < numLocations - 1; i++){
      if (i % rows == 0){
        for (int j = 0; j < rows; j++){
          switch (tripletCol){
            case 1: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 2: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 3: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 4: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 5: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 6: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 7: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 8: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 9: locations[i+j][0] = stepsPerWell * j;
                    locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 10: locations[i+j][0] = stepsPerWell * j;
                     locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 11: locations[i+j][0] = stepsPerWell * j;
                     locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 12: locations[i+j][0] = stepsPerWell * j;
                     locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 13: locations[i+j][0] = stepsPerWell * j;
                     locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 14: locations[i+j][0] = stepsPerWell * j;
                     locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 15: locations[i+j][0] = stepsPerWell * j;
                     locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 16: locations[i+j][0] = stepsPerWell * j;
                     locations[i+j][1] = -(tripletCol - 1) * (3 * stepsPerWell);
                     break;
            default: break;
          }
        }
        tripletCol = tripletCol + 1;
      }
    }
    locations[numLocations - 1][0] = locations[numLocations - 2][0] + stepsToEject;
    locations[numLocations - 1][1] = -((columns / 3) - 1) * (3 * stepsPerWell);
  }
}

int main(){
	long locations[numLocations][2];
	populateLocations(locations, 1);

	cout << locations[numLocations][0] << " " << locations[numLocations][1] << endl;

	return 0;
}
