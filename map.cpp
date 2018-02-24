#include "globals.h"
#include <fstream>
#include <sstream>
using namespace std;


namespace map {
	
	int width = 0, height = 0, layers = 0;
	vector<vector<int>> tmap;
	
	int loadmap(const std::string& fname) {
		fstream fs(fname, fstream::in);
		string input, s;
		while (getline(fs, s))
			input += s;
		fs.close();
		
		// get width and height
		width = height = 0;
		{
			int pos1 = input.find("width=\"") + 7;
			int pos2 = input.find("\"", pos1);
			s = input.substr(pos1, pos2-pos1);
			stringstream ss(s);
			ss >> width;
			//printf("[%d]\n", width);
		}
		{
			int pos1 = input.find("height=\"") + 8;
			int pos2 = input.find("\"", pos1);
			s = input.substr(pos1, pos2-pos1);
			stringstream ss(s);
			ss >> height;
			//printf("[%d]\n", height);
		}
		
		// get row data 1-3
		int lpos = 0;
		vector<string> data;
		while (true) {
			string start = "<data encoding=\"csv\">",  end = "</data>";
			int pos1 = input.find(start, lpos) ;
			if (pos1 == string::npos)  break;
			pos1 += start.length();
			int pos2 = input.find(end, pos1);
			lpos = pos2;
			s = input.substr(pos1, pos2-pos1);
			data.push_back(s);
			//printf("[%s]\n", s.c_str());
		}
		
		// parse rows to number list
		tmap = {};
		for (auto& d : data) {
			for (char& c : d)
				if (c == ',')  c = ' ';
			tmap.push_back({});
			stringstream ss(d);
			int n = 0;
			while (ss >> n)
				tmap.back().push_back(n);
		}
		layers = tmap.size();
		
		printf("map load OK:  %d,%d,%d\n", width, height, layers);
		return 0;
	}

} // end map
