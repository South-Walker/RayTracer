#include <iostream>
#include <dirent.h>
#include <string.h>
#include <fstream>
#include <sstream>
 
using namespace std;

int pixel[100000000];
int count;
int main(int argc, char** argv)
{
	DIR *dirptr = NULL;
	struct dirent *entry;
	if ((dirptr = opendir(argv[1])) == NULL)
	{
		cout << "can't find " << argv[1] << " (e.g. ./output)" << endl;
		return 1;
	}
	string sdir = argv[1];
	string line;
	int height, width;
	int now;
	int filecount = 0;
	while (entry = readdir(dirptr))
	{
		if (DT_REG == entry->d_type)
		{
			filecount++;
			ostringstream filepath;
			filepath << argv[1] << "/" << entry->d_name;
			ifstream filereader(filepath.str());
			getline(filereader, line);
			filereader >> height >> width;
			getline(filereader, line);
			getline(filereader, line);
			count = height * width * 3;
			for (int i = 0; i < count; i++)
			{
				filereader >> now;
				pixel[i] += now;
			}
		}
	}
	closedir(dirptr);
	for (int i = 0; i < count; i++)
	{
		pixel[i] /= filecount;
	}
	ostringstream filepath;
	filepath << argv[1] << "/" << "output.ppm";
	ofstream filewriter(filepath.str());
	filewriter << "P3" << endl
		<< height << " " << width << endl
		<< "255";
	for (int i = 0; i < count; i++)
	{
		if (i % 3 == 0)
			filewriter << endl;
		filewriter << pixel[i] << " ";
	}
	filewriter << endl;
	filewriter.close();
	return 0;
}
