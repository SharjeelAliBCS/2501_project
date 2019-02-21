#include "mapLoader.h"

mapLoader::mapLoader(int f)
{
	using namespace std;

	ifstream in("map.csv");

	string line, field;

	vector< vector<string> > array;  // the 2D array
	vector<string> v;                // array of values for one line only
	float j = -20.0;

	while (getline(in, line))    // get next line in file
	{

		v.clear();
		stringstream ss(line);
		//for(int i =0;i<s)
		//cout << line+"\n";
		float i = -20.0;
		while (getline(ss, field, ','))  // break line into comma delimitted fields
		{
			//cout << field+"\n";
			if (field == "0") {
				glm::vec3 out = glm::vec3(i, j, 0.0f);
				backGround[0]->setPosition(out);
				backGround[0]->render(shader);
			}
			else if (field == "1") {
				glm::vec3 out = glm::vec3(i, j, 0.0f);
				backGround[1]->setPosition(out);
				backGround[1]->render(shader);
			}
			else if (field == "3") {
				glm::vec3 out = glm::vec3(i, j, 0.0f);
				backGround[2]->setPosition(out);
				backGround[2]->render(shader);
			}
			else if (field == "4") {
				glm::vec3 out = glm::vec3(i, j, 0.0f);
				backGround[3]->setPosition(out);
				backGround[3]->render(shader);
			}
			else if (field == "6") {
				glm::vec3 out = glm::vec3(i, j, 0.0f);
				backGround[4]->setPosition(out);
				backGround[4]->render(shader);
			}
			i++;
		}
		j++;
	}
}
