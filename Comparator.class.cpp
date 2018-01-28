#include "Comparator.class.h"

Comparator::Comparator() {}

void Comparator::addRecord(std::vector< std::vector< Feature > > feature, int id) {
	Record record;
	for(int i = 0; i < feature.size(); i++) {
		std::vector<int> vec;
		for(int j = 0; j < feature[i].size(); j++) {
			int angle = feature[i][j].angle;
			vec.push_back(angle);
		}
	record.feat.push_back(vec);
	vec.clear();
	}

	if(id == 0) {
//		record.id = this->ID;
//		this->ID++;
	} else {
		record.id = id;
	}
				
	records.push_back(record);
	saveRecords();	
}

int Comparator::compare(std::vector < std::vector< Feature > > query) {
	float match = 0;
	float bestMatch = 0;
	int id = 1000;

	for(int i = 0; i < records.size(); i++) {
		match = checkRecord(query, records[i].feat);
		std::cout << match << std::endl;
		if(match > bestMatch) {
			bestMatch = match;
			id = records[i].id;
		}	
	}
	return id;
	
}

float Comparator::checkRecord(std::vector< std::vector< Feature > > query, std::vector< std::vector< int > > record) {
	float match = 0.0f;
	for(int i = 0; i < record.size(); i++) {
		for(int j = 0; j < record[i].size(); j++) {
			match += checkSimilarity(query[i], record[i], 4)/record[i].size();
		}
		
	}
	match /= record.size();
	return match;
}

float Comparator::checkSimilarity(std::vector<Feature> query, std::vector<int> record, int treshold) {
	float similarity = 0.0f;
	if(query.size() >= record.size()) { 
		for(int i = 0; i < query.size(); i++) {
			int counter = 0;
			for(int j = 0; j < record.size(); j++) {
				if(std::abs(query[i].angle - record[j]) < treshold) {
					similarity += (float)1/query.size();
				}
			}
		}
	
	return similarity;	
	
	}

	else if(query.size() < record.size()) { 
		for(int i = 0; i < record.size(); i++) {
			int counter = 0;
			for(int j = 0; j < query.size(); j++) {
				if(std::abs(record[i] - query[j].angle) < treshold) {
					similarity += (float)1/record.size();
				}
			}
		}
	}
	return similarity;
}

void Comparator::saveRecords() {
	std::fstream base;
	base.open("base.txt", std::ios::in | std::ios::out | std::ios::trunc);	
	for(int i = 0; i < records.size(); i++) {
		for(int j = 0; j < records[i].feat.size(); j++) {
			for(int k = 0; k < records[i].feat[j].size(); k++) { 
					base << records[i].feat[j][k];
					base << ",";
			}
			
			base << ":";
		}
		base << records[i].id << "." << std::endl;
	}
	base.close();
	

}
void Comparator::loadRecords() {
	std::fstream base;
	base.open("base.txt", std::ios::in | std::ios::out);
	std::string rec;
	records.clear();
	while(getline(base, rec)) {
		Record record;
		std::string f;
		std::vector<int> vec;		
		for(int i = 0; i < rec.length(); i++) {
			if(rec[i] == ',') {
				vec.push_back(atoi(f.c_str()));
				f.clear();
			}
			else if(rec[i] == ':') {
				record.feat.push_back(vec);
				vec.clear();
				f.clear();
			} else if(rec[i] == '.') {				
				record.id = atoi(f.c_str());
			} else {
				f += rec.at(i);
			}
		}
		records.push_back(record);				
	}
	base.close();
}

