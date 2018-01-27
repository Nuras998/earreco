#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Structs.h"
struct Record {
std::vector < std::vector< int > > feat;
int id;
std::string name;
};

class Comparator {

public:

//static int ID;

Comparator();

void addRecord(std::vector< std::vector< Feature > > feature, int id = 0);
int compare(std::vector < std::vector< Feature > > query); 
void saveRecords(); 
void loadRecords(); 
private:
float checkRecord(std::vector< std::vector< Feature > > query, std::vector< std::vector< int > > record);
float checkSimilarity(std::vector<Feature> query, std::vector<int> record, int treshold = 5); 
std::vector<Record> records;
 
};

//int Comparator::ID = 0;

