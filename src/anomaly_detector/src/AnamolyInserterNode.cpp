/*
 * AnamolyInserterNode.cpp
 *
 *  Created on: Dec 28, 2018
 *      Author: Kiran and pankaj
 */

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <time.h>



#define MAX_SAMPLING_RATE 7
#define MIN_SAMPLING_RATE 3
/*
 * A class to create and write data in a csv file.
 */
class CSVWriter
{
	std::string inputfileName_;
	std::string outputfilename_;
	std::string delimeter_;
	std::vector<std::vector<std::string> > dataList_;
 
public:

	// Function to fetch data from a CSV File
	void  getData();
	int linesCount_;

	CSVWriter(std::string ifilename, std::string ofilename, std::string delm = ",") :
			inputfileName_(ifilename),outputfilename_(ofilename), delimeter_(delm), linesCount_(0)
	{}
	/*
	 * Member function to store a range as comma seperated value
	 */
	template<typename T>
	void addDatainRow(T data, int index);
};

/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
void CSVWriter::getData()
{
	std::ifstream ifs;
	ifs.open(inputfileName_.c_str(), std::ifstream::in);

	if (ifs.is_open()) 
	{
		std::string line = "";
		// Iterate through each line and split the content using delimeter
		while (getline(ifs, line))
		{
			std::vector<std::string> vec;
			boost::algorithm::split(vec, line, boost::is_any_of(delimeter_));
			dataList_.push_back(vec);
			linesCount_++;
		}
		// Close the File
		ifs.close();
	}
}
 
/*
 * This Function accepts a range and adds all the elements in the range
 * to a random location, seperated by delimeter (Default is comma)
 */
template<typename T>
void CSVWriter::addDatainRow(T data, int i)
{
	std::fstream filein, fileout;
	
	//Open a new file for updating
	fileout.open(outputfilename_, std::ios::out);
	
	if (fileout.is_open())
	{
		std::vector<std::vector<std::string>>::iterator outeritr;
		std::vector<std::string>::iterator inneritr;
	
		if(dataList_.size() > 0)
		{
			outeritr = dataList_.begin();
			inneritr = (*outeritr).begin();

			//Insert data after the given index
			dataList_.insert(outeritr + i, data);

			for (; outeritr != dataList_.end(); )
			{ 
				inneritr = (*outeritr).begin();
				for(; inneritr != (*outeritr).end();)
				{
					fileout << *inneritr;
					if (++inneritr != (*outeritr).end())
						fileout << delimeter_;
				}
				fileout << "\n";
				outeritr++;
			}
		}
	
	
	 
		// Close the file
		fileout.close();
	}
}


int main(int argc, char **argv)
{
    int insertSamples = 0 ;
    
    if (argc != 2) 
    {
    	insertSamples = MIN_SAMPLING_RATE;
    } 
    else 
    {
       insertSamples = atoi(argv[1]);
    }

     int index = 0;
     
     /* initialize random seed: */
     srand (time(NULL));


     // removing the existing file 
    remove("/etc/tmp/anomalyInserted.csv");

    // Creating an object of CSVWriter
    CSVWriter writer("/etc/tmp/anomaly.csv", "/etc/tmp/anomalyInserted.csv");

    // Get the data from CSV File
    writer.getData();


     std::vector<std::string> dataList[] = {    {"0xea","25", "17.8", "0","0","1","0","1546436834" },
						{ "0xea","28", "17.8", "0","1","0","0","1546436835" },
     						{ "0xea","12", "17.8", "0","0","0","1","1546436836" },
     						{ "0xea","26", "17.8", "1","0","0","0","1546436837" },
     						{ "0xea","25.4", "11.2", "0","0","1","0","1546436838"},
						{ "0xea","26", "18.4", "1","0","1","0","1546436839" },
     						{ "0xea","25.4", "12.1", "0","1","1","0","1546436840"}
						
					   };
      
     
     
     if(0 < writer.linesCount_)
     {
	index = rand() % writer.linesCount_ + 1;    
     	//std::cout << "inserted at index = " << index << std::endl;
     }
     else
	std::cout << "Error:: not able to open file or file does not have any data" << std::endl;
     
     if( insertSamples > 0  && insertSamples <= MAX_SAMPLING_RATE) 
     { 
	     std::cout << "inserting "  << insertSamples << " samples at index "  << index  << std::endl; 
	     for(int i = 0; i < insertSamples; i++)
	     {
		  // Adding vector to CSV File
		  writer.addDatainRow(dataList[i], index++);
	     }
    }
    else
	std::cout << "error:samples to be injected can not be greater than 7" << std::endl;
 
  return 0;
}



