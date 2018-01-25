#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "Structs.h"
struct Record {
std::vector < std::vector< Feature > > feat;
int id;
std::string name;
};

class Comparator {

public:

//static int ID;

Comparator();

void addRecord(std::vector< std::vector< Feature > > feature, int id = 0);
int compare(std::vector < std::vector< Feature > > query); 

private:
float checkRecord(std::vector< std::vector< Feature > > query, std::vector< std::vector< Feature > > record);
float checkSimilarity(std::vector<Feature> query, std::vector<Feature> record, int treshold = 5); 
std::vector<Record> records;
 
};

//int Comparator::ID = 0;

