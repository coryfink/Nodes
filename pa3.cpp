//Cory Fink
//COP3503 Project 3
//7161-8579

#include "pa3.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include<algorithm>




using namespace std;

const int MAX_VALS = 100;


//List of Functions
deque<string> saveLines(ifstream &readIn);   
//checks each vector file for the things needed
vector<string> keywordCheckPoint(vector<string> keywordReference, vector<string> foundKeys, string current);
vector<string> delimiterCheckPoint(vector<string> delimiterReference, vector<string> foundDelims, string current);
vector<string> operatorCheckPoint(vector<string> operatorReference, vector<string> foundOps, string current);
vector<string> constantCheckPoint(vector<string> foundCons, string current);
vector<string> errorCheckPoint(vector<string> foundErrs, vector<string> keywordReference, string current);
vector<string> identifierCheck(vector<string> foundIdents, string current);
//goes through each item to check for the instances to see what is where and such
vector<string> getUniqueKeywords(vector<string> uniqueKeywords, vector<string> foundKeys);
vector<string> getUniqueDelimiters(vector<string> uniqueDelimiters, vector<string> foundDelims);
vector<string> getUniqueOperators(vector<string> uniqueOperators, vector<string> foundOps);
vector<string> getUniqueConstants(vector<string> uniqueConstants, vector<string> foundCons);
vector<string> getUniqueIdentifiers(vector<string> uniqueIdentifiers, vector<string> foundIdents);
vector<string> getUniqueErrors(vector<string> uniqueErrors, vector<string> foundErrs);
//this section checks the depth
int getDepth(vector<string> foundKeys); 
   
//locates any additional errors
string findErrors(vector<string> foundKeys, vector<string> foundErrs);
//this section prints the statements   
void printingPortion(vector<string> input);     


int main() {

    //**IDENTIFIER SECTION**

    //Keywords
    vector<string> keywordReference;
    keywordReference.push_back("BEGIN");
    keywordReference.push_back("FOR");
    keywordReference.push_back("END");

    //Operators
    vector<string> operatorReference;
    operatorReference.push_back("++");
    operatorReference.push_back("+");
    operatorReference.push_back("-");
    operatorReference.push_back("*");
    operatorReference.push_back("/");
    operatorReference.push_back("=");

    //Delimiters
    vector<string> delimiterReference;
    delimiterReference.push_back(",");
    delimiterReference.push_back(";");

    //Ways to keep track of items in queue
    vector<string> foundKeys;
    vector<string> foundIdents;
    vector<string> foundCons;
    vector<string> foundOps;
    vector<string> foundDelims;
    vector<string> foundErrs;

    //Unique queues
    vector<string> uniqueKeywords;
    vector<string> uniqueIdentifiers;
    vector<string> uniqueConstants;
    vector<string> uniqueOperators;
    vector<string> uniqueDelimiters;
    vector<string> uniqueErrors;


   

    //this section intakes the file name
    //checks to see if the file is found in directory/usable
    //analyzes the file and prints the results
    string fileName;

    cout << "Please enter the name of the input file." << endl;
    cin >> fileName;

    ifstream readIn(fileName.c_str()); 

    //this is used when there is an error with reading the file
    if (!readIn.is_open()) {
        cout << "Not able to read/take in file " << fileName << ",  please try again." << endl;
        system("pause");
        return 1;
    }

 

    deque<string> lineQue = saveLines(readIn);

    //Get tokenized lines
    long length = lineQue.size(); 
    //while the input is still being ran...  

    while (!lineQue.empty()) { 
        //   string input = "";
        //  deque<string> newLineQue;
        string current = lineQue.front();
        foundKeys = keywordCheckPoint(keywordReference, foundKeys, current);
        foundOps = operatorCheckPoint(operatorReference, foundOps, current);
        foundDelims = delimiterCheckPoint(delimiterReference, foundDelims, current);
        //CONSTANTS
        foundCons = constantCheckPoint(foundCons, current);
        //INDENTIFIERS
        foundIdents = identifierCheck(foundIdents, current);

        //ERRORS
        foundErrs = errorCheckPoint(foundErrs, keywordReference, current);
        lineQue.pop_front();
    }




    //creates a unique item list
    uniqueKeywords = getUniqueKeywords(uniqueKeywords, foundKeys);
    uniqueConstants = getUniqueConstants(uniqueConstants, foundCons);
    uniqueDelimiters = getUniqueDelimiters(uniqueDelimiters, foundDelims);
    uniqueOperators = getUniqueOperators(uniqueOperators, foundOps);
    uniqueIdentifiers = getUniqueIdentifiers(uniqueIdentifiers, foundIdents);
    uniqueErrors = getUniqueErrors(uniqueErrors, foundErrs);


     //PRINTING SECTION
    string errors = findErrors(foundKeys, foundErrs);
    int depth = getDepth(foundKeys);
    if(depth ==2)
	{ 
		depth = 2;
	}	
    else
	{
		depth=depth-1;
	}

    //Print everything
    cout << "The depth of nested loop(s) is ";
    cout << depth << endl;
    cout << "Keywords: ";
    printingPortion(uniqueKeywords);
    cout << "Identifiers: ";
    printingPortion(uniqueIdentifiers);
    cout << "Constants: ";
    printingPortion(uniqueConstants);
    cout << "Operators: ";
    printingPortion(uniqueOperators);
    cout << "Delimiters: ";
    printingPortion(uniqueDelimiters);
    cout << "Syntax Error(s): ";
    printingPortion(uniqueErrors);
    cout << errors << endl;



    return(0);

}


//**METHODS SECTION**

//loops through each line and prints the vector
void printingPortion(vector<string> input) {
    long size = input.size();
    for (int i = 0; i < size; i++) {
        cout << input.at((unsigned long) i) << " ";
    }
    cout << endl;
}

//saves the lines into the vector
deque<string> saveLines(ifstream &readIn) {
    string line;
    deque<string> pushLines;

    if (readIn.is_open()) {
        while (getline(readIn, line)) {
            pushLines.push_back(line);
        }
    } else {
        cout << "Unable to read file. Please try again." << endl;
        exit(1);
    }

    return pushLines;
}
 //CHECKS FOR KEYWORDS
vector<string> keywordCheckPoint(vector<string> keywordReference, vector<string> foundKeys, string current) {
   
    for (int i = 0; i < keywordReference.size(); i++) {
        string keyword = keywordReference.at(i);
        //If keyword is token
        while (current.find(keyword) != string::npos) {
	    //ADD TO THE LIST, take away from the string
            foundKeys.push_back(keyword);
            current.replace(current.find(keyword), keyword.length(), " ");

        }

    }
    return foundKeys;
}
//CHECKS FOR DELIMETERS
vector<string> delimiterCheckPoint(vector<string> delimiterReference, vector<string> foundDelims, string current) {
   
    for (int i = 0; i < delimiterReference.size(); i++) {
        string delimiter = delimiterReference.at(i);
        //If delimiter is found
        while (current.find(delimiter) != string::npos) {
            //ADD TO THE LIST, take away from the string
            foundDelims.push_back(delimiter);
            current.replace(current.find(delimiter), delimiter.length(), " ");

        }
    }

    return foundDelims;
}
//CHECKS FOR OPERATORS
vector<string> operatorCheckPoint(vector<string> operatorReference, vector<string> foundOps, string current) {
    
    for (int i = 0; i < operatorReference.size(); i++) {
        string operators = operatorReference.at(i);
        //If operator is found
        while (current.find(operators) != string::npos) {
	    //ADD TO THE LIST, take away from the string
            foundOps.push_back(operators);
            current.replace(current.find(operators), operators.length(), " ");

        }
    }

    return foundOps;
}
//CHECKS FOR ERRORS
vector<string> errorCheckPoint(vector<string> foundErrs, vector<string> keywordReference, string current) {
    current.erase(remove(current.begin(), current.end(), ' '), current.end());
    string error = "";

    for (int i = 0; i < keywordReference.size(); i++) {
        string keyword = keywordReference.at(i);
        //If keyword is token
        while (current.find(keyword) != string::npos) {
            current.replace(current.find(keyword), keyword.length(), " ");
        }
    }

    for (int i = 0; i < current.length(); i++) {
        if (error == "") {
            if (isupper(current.at(i))) {
                error = current.at(i);
                current.replace(current.find(current.at(i)), 1, " ");
            }
        } else {
            if (isupper(current.at(i))) {
                error += current.at(i);
                current.replace(current.find(current.at(i)), 1, " ");
            } else {
                foundErrs.push_back(error);
                error = "";
            }
        }
    }
    return foundErrs;
}
//CHECKS FOR CONSTANTS
vector<string> constantCheckPoint(vector<string> foundCons, string current) {

    string constant = "";
    int constCount = 0;
    
    for (int i = 0; i < current.length(); i++) {
        if (constant == "") {
            if (isdigit(current.at(i))) {
                constant = current.at(i);
                constCount++;
            }
        } else {
            if (isdigit(current.at(i))) {
                constant += current.at(i);
            } else {
                foundCons.push_back(constant);
                current.replace(current.find(constant), constant.length(), " ");
                constant = "";
            }
        }
    }
    return foundCons;
}
//CHECKS FOR IDENTIFIERS
vector<string> identifierCheck(vector<string> foundIdents, string current) {
    string identifier = "";
    int constCount = 0;
 
    for (int i = 0; i < current.length(); i++) {
        if (identifier == "") {
            if (islower(current.at(i))) {
                identifier = current.at(i);
                current.replace(current.find(current.at(i)), 1, " ");
                constCount++;
            }
        } else {
            if (islower(current.at(i))) {
                identifier += current.at(i);
                current.replace(current.find(current.at(i)), 1, " ");
            } else {
                foundIdents.push_back(identifier);
                identifier = "";
            }

        }
    }
    return foundIdents;
}


//CHECKS FOR UNIQUE KEYWORDS
vector<string> getUniqueKeywords(vector<string> uniqueKeywords, vector<string> foundKeys) {
    string allKeywords = " ";
    uniqueKeywords.push_back(foundKeys.at(0));
    allKeywords += foundKeys.at(0) + " ";
    int size = (int) foundKeys.size();
    for (int i = 0; i < size; i++) {
        string current = foundKeys.front();
        if (allKeywords.find(current) == string::npos) {
            allKeywords += current + " ";
            uniqueKeywords.push_back(current);
        }
        foundKeys.erase(foundKeys.begin());
    }
    return uniqueKeywords;
}
//CHECKS FOR UNIQUE OPERATORS
vector<string> getUniqueOperators(vector<string> uniqueOperators, vector<string> foundOps) {
    string allOperators = " ";
    uniqueOperators.push_back(foundOps.at(0));
    allOperators += foundOps.at(0) + " ";
    int plusCounter = 0;
    int size = (int) foundOps.size();
    for (int i = 0; i < size; i++) {
        string current = foundOps.front();
        if (current.find(" + ") && current != "++") {
            if (plusCounter == 0) {
                uniqueOperators.push_back(current);
                allOperators += current + " ";
                plusCounter ++;
            }
        }
        if (allOperators.find(current) == string::npos) {
            allOperators += current + " ";
            uniqueOperators.push_back(current);
        }
        foundOps.erase(foundOps.begin());
    }
    return uniqueOperators;
}
//CHECKS FOR UNIQUE IDENTIFIERS
vector<string> getUniqueIdentifiers(vector<string> uniqueIdentifiers, vector<string> foundIdents) {
    string allIdentifiers = " ";
    uniqueIdentifiers.push_back(foundIdents.at(0));
    allIdentifiers += foundIdents.at(0) + " ";
    int size = (int) foundIdents.size();
    for (int i = 0; i < size; i++) {
        string current = foundIdents.front();
        if (allIdentifiers.find(current) == string::npos) {
            allIdentifiers += current + " ";
            uniqueIdentifiers.push_back(current);
        }
        foundIdents.erase(foundIdents.begin());
    }
    return uniqueIdentifiers;
}
//CHECKS FOR UNIQUE CONSTANTS
vector<string> getUniqueConstants(vector<string> uniqueConstants, vector<string> foundCons) {
    string allConstants = " ";
    uniqueConstants.push_back(foundCons.at(0));
    allConstants += foundCons.at(0) + " ";
    int size = (int) foundCons.size();
    for (int i = 0; i < size; i++) {
        string current = foundCons.front();
        if (allConstants.find(current) == string::npos) {
            allConstants += current + " ";
            uniqueConstants.push_back(current);
        }
        foundCons.erase(foundCons.begin());
    }
    return uniqueConstants;
}
//CHECKS FOR UNIQUE DELIMITERS
vector<string> getUniqueDelimiters(vector<string> uniqueDelimiters, vector<string> foundDelims) {
    string allDelimiters = " ";
    uniqueDelimiters.push_back(foundDelims.at(0));
    allDelimiters += foundDelims.at(0) + " ";
    int size = (int) foundDelims.size();
    for (int i = 0; i < size; i++) {
        string current = foundDelims.front();
        if (allDelimiters.find(current) == string::npos) {
            allDelimiters += current + " ";
            uniqueDelimiters.push_back(current);
        }
        foundDelims.erase(foundDelims.begin());
    }
    return uniqueDelimiters;
}

//CHECKS FOR UNIQUE ERRORS
vector<string> getUniqueErrors(vector<string> uniqueErrors, vector<string> foundErrs) {
    if (!foundErrs.empty()) {
        string allErrors = " ";
        uniqueErrors.push_back(foundErrs.at(0));
        allErrors += foundErrs.at(0) + " ";
        for (int i = 0; i < foundErrs.size() + 1; i++) {
            string current = foundErrs.front();
            if (allErrors.find(current) == string::npos) {
                allErrors += current + " ";
                uniqueErrors.push_back(current);
            }
            foundErrs.erase(foundErrs.begin());
        }
    }
    return uniqueErrors;
}
//FINDS THE DEPTH OF THE LOOP
int getDepth(vector<string> foundKeys) {
    int depth = 0;
    int maxDepth = 0;
    int size = (int) foundKeys.size();
    int forCounter = 0;
    int endCounter = 0;
    for (int i = 0; i < size; i++) {
        if (forCounter >= endCounter) {
            string current = foundKeys.front();
            if (current == "FOR") {
                depth++;
                forCounter++;
            } else if (current == "END") {
                depth--;
                endCounter++;
            }
            if (depth > maxDepth) {
                maxDepth = depth;
            }

        } else {
            if (endCounter > maxDepth) {
                maxDepth = endCounter;
            }
        }
        foundKeys.erase(foundKeys.begin());
    }
    return maxDepth;
}
//CHECKS FOR ADDITIONAL ERRORS
string findErrors(vector<string> foundKeys, vector<string> foundErrs) {
    int forCounter = 0;
    int startCounter = 0;
    int endCounter = 0;
    string error = " ";
    while (!foundKeys.empty()) {
        string current = foundKeys.front();
        if (current == "FOR") {
            forCounter++;
        } else if (current == "BEGIN") {
            startCounter++;
        } else if (current == "END") {
            endCounter++;
        }
        foundKeys.erase(foundKeys.begin());
    }

    if (foundErrs.empty()) {
        if (forCounter != startCounter != endCounter) {
            if (forCounter < startCounter) {
                error += "FOR ";
            } else if (forCounter < endCounter) {
                error += "FOR ";
            }
            if (startCounter < endCounter) {
                error += "BEGIN ";
            } else if (startCounter < forCounter) {
                error += "BEGIN ";
            }
            if (endCounter < startCounter) {
                error += "END ";
            } else if (endCounter < forCounter) {
                error += "END ";
            }

        }
    }
    return error;
}
