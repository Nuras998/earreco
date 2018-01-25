#include "Comparator.class.h"

Comparator::Comparator() {}

void Comparator::addRecord(std::vector< std::vector< Feature > > feature, int id) {
	Record record;
 
	for(int i = 0; i < feature.size(); i++) {
		std::vector<Feature> vec;
		for(int j = 0; j < feature[i].size(); j++) {
			Feature f;
			f.x = feature[i][j].x;
			f.y = feature[i][j].y;
			f.angle = feature[i][j].angle;
			f.radius = feature[i][j].radius;
			vec.push_back(f);
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

float Comparator::checkRecord(std::vector< std::vector< Feature > > query, std::vector< std::vector< Feature > > record) {
	float match = 0.0f;
	for(int i = 0; i < record.size(); i++) {
		for(int j = 0; j < record[i].size(); j++) {
			match += checkSimilarity(query[i], record[i], 4)/record[i].size();
		}
		
	}
	//std::cout << "Match: " << match << " Record size: " << record.size() << " Query size: " << query.size() << std::endl;
	match /= record.size();
	return match;
}

float Comparator::checkSimilarity(std::vector<Feature> query, std::vector<Feature> record, int treshold) {
	float similarity = 0.0f;
	if(query.size() >= record.size()) { 
		for(int i = 0; i < query.size(); i++) {
			int counter = 0;
			for(int j = 0; j < record.size(); j++) {
				if(std::abs(query[i].angle - record[j].angle) < treshold) {
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
				if(std::abs(record[i].angle - query[j].angle) < treshold) {
					similarity += (float)1/record.size();
				}
			}
		}
	}
	return similarity;
}

