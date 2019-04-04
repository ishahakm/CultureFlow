#include <iostream>
using namespace std;

const int rows = 8;
const int columns = 12;
const int numLocations = ((rows * columns) / 3) + 1;
const int stepsPerWell = 225;
const int stepsToEject = 500;

void populateLocations(long (&locations)[numLocations][2]){
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

int main(){
	long locations[numLocations][2];
	populateLocations(locations);

	cout << locations[numLocations][0] << " " << locations[numLocations][1] << endl;

	return 0;
}
