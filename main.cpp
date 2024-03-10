// Mustafa Batin EFE
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

/* Begin: code taken and updated from MatrixDemo2.cpp */
void Print(const vector<vector<char>> & temp){
    for (int j = 0; j < temp.size(); j++){
        for (int k = 0; k < temp[0].size(); k++){
            cout << temp[j][k];
        }
        cout << endl;
    }
}
/* Finish: code taken and updated from MatrixDemo2.cpp */

/* Following function will create query vector from input string */
void makeQueryVector(vector<vector<char>> & queryVector, vector<string> & seperateRows){
    for(int w = 0; w < seperateRows.size(); w++){
        vector<char> temp; // create temp vector
        temp.clear();
        /* Iterate through each string like [3s3b4s] by 3-s-3-b-4-s */
        for (int q = 0; q < seperateRows[w].size(); q++) {
            string str2(1, seperateRows[w][q]); // Get 1 char of it and transfer to string
            if (str2 == "s") {
                //Add q-1 times star to the vector
                char starChar = '*';
                string str3(1,seperateRows[w][q-1]); // Get 1 char of it and transfer to string
                for (int y = 0; y < stoi(str3); y++) {
                    temp.push_back(starChar);
                }
            } else if (str2 == "b") {
                //Add q-1 times blank to vector
                char blankChar = ' ';
                string str4(1,seperateRows[w][q-1]); // Get 1 char of it and transfer to string
                for (int z = 0; z <stoi(str4); z++) {
                    temp.push_back(blankChar);
                }
            }
        }
        queryVector.push_back(temp);
    }
}

/* Following function will travel through each char of query Vector and push them in to 1D vector */
void setQueryCoordinate(vector<vector<char>> & queryVector, vector<int> & queryChange, int & starCount){
    for(int rowQ = 0; rowQ < queryVector.size(); rowQ++) {
        for (int colQ = 0; colQ < queryVector[rowQ].size(); colQ++) {
            if(queryVector[rowQ][colQ] == '*'){
                queryChange.push_back(rowQ);
                queryChange.push_back(colQ);
                starCount += 1;
            }

        }
    }

}

/* Following function will find maximum size of columns in queryVector*/
void findMaxQueryCol(int & maxSizeCol, vector<vector<char>> & queryVector){
    for(int l = 0; l < queryVector.size(); l++){
        if(queryVector[l].size() > maxSizeCol){
            maxSizeCol = queryVector[l].size();
        }
    }
}

/* Following function will check every probability of queryVector(stars) in fileVector */
void findPlaceVector(vector<vector<char>> & fileVector, vector<vector<char>> & queryVector, int & maxSizeCol, vector<int> & queryChange, int & addTryRow, int & addTryCol,
                     vector<string> & coordinates, int & count, int & starCount, vector<string> & placements){
    // (Matrix Row Size - Query String Row Size + 1) = Number of probability that can be checked in each row
    for (int q = 0; q < (fileVector.size() - queryVector.size() + 1); q++) {
        // (Matrix Col Size - Query String MAX Col Size + 1) = Number of probability that can be checked in each col
        for (int z = 0; z < (fileVector[q].size() - maxSizeCol + 1); z++) {
            // { 0, 0, 1, 0,} = (0,0) - (1,0); it will take the coordinate of star occurrences
            for (int y = 0; y < queryChange.size(); y = y + 2) {
                /* It will travel through probability by increasing 1 step*/

                // queryChange sample = // {1,2,3,4,5,6,7,8} = (1,2) - (3,4) - (5,6) - (7,8)
                int queRowCord = queryChange[y];
                int queColCord = queryChange[y + 1];
                // Store the placements in temps vector
                if (fileVector[queRowCord + addTryRow][queColCord + addTryCol] == '-') {
                    string temps =
                            "(" + to_string(queRowCord + addTryRow) + "," + to_string(queColCord + addTryCol) +
                            ")";
                    coordinates.push_back(temps);
                    count++; // Count to check later if we find same number of occurrences stars in query
                }
            }
            // increase col number to step 1 to the right
            addTryCol++;
            string placed;
            // Now, algorithm will check if it is the same amount of star
            // If yes, it will push to the placements vector
            if (starCount == count && count != 0) {
                for (int zq = 0; zq < count; zq++) {
                    placed += coordinates[zq];
                    if (zq != (count - 1)) {
                        placed += " ";
                    }
                }
                placements.push_back(placed);
            }
            // clear coordinates and reset count for the next occurrences
            count = 0;
            coordinates.clear();
        }
        // increase row number to step 1 to the down
        addTryRow++;
        addTryCol = 0;
    }
}

int main() {
    /* Getting file name input and opening it*/
    string fileName, fileNameSecond;
    cout << "Please enter the file name:" << endl;
    cin >> fileName;
    ifstream inputFile;
    inputFile.open(fileName);

    /* Quick check if the file is opened, otherwise it will keep asking */
    while(inputFile.fail()) {
        cout<< "Could not open the file. Please enter a valid file name: " << endl;
        cin.clear();
        cin >> fileNameSecond;
        inputFile.open(fileNameSecond);
    }
    /* File successfully opened */
    int lineCount = 0, charCount = 0;
    vector<int> tempColNum;
    string line;
    while(!inputFile.eof()){
        getline(inputFile, line);
        stringstream ss(line);
        tempColNum.push_back(line.length());
        lineCount++;
    }
    /* Checking if number of columns in each row are the same*/
    int firstColNum = tempColNum[0];
    for(int z = 0; z < tempColNum.size(); z++){
        if(firstColNum != tempColNum[z]){
            cout << "Erroneous file content. Program terminates." << endl;
            return 0;
        }
    }
    /* Creating a matrix vector and adding the chars from file*/
    vector<vector<char>> fileVector(lineCount,vector<char>(firstColNum));

    string line2;
    int lineCount2 = 0;
    /* Reset the file to start reading again */
    inputFile.clear();
    inputFile.seekg(0);
    /*----------------------------------------*/
    /* Read the file line by line then char by char*/
    while(!inputFile.eof()){
        getline(inputFile, line2);
        stringstream ss2(line2);
        for(int x = 0; x < line2.length(); x++){
            // Check if a char in file is not "-" or "#"
            string str(1,line2[x]); // Get 1 char of it and transfer to string
            if(str != "-" && str != "#"){
                cout << "Erroneous file content. Program terminates." << endl;
                return 0;
            }
            // Going to update chars into matrix vector
            fileVector[lineCount2][x] = line2[x];
        }
        lineCount2 ++;
    }
    cout << "The matrix file contains:" << endl;
    Print(fileVector);

    /* It is time to get query and put it on vector */
    string queryInput = "";
    /* If input is Quit, terminate the program*/
    while(queryInput != "Quit"){
        cout << "Enter the string query for a shape, or \"Quit\" to terminate the program:" << endl;
        cin >> queryInput;
        /* If input is Quit, terminate the program*/
        if(queryInput == "Quit"){
            return 0;
        }
        /* Program continues */
        /*Split the string and store*/
        stringstream qq(queryInput);
        string eachRow;
        vector<string> seperateRows;
        while(getline(qq, eachRow, '/')){
            seperateRows.push_back(eachRow);
        }

        int starCount =0; // to get number of star in query
        vector<vector<char>> queryVector;
        /* Following function will create a matrix vector for query string */
        makeQueryVector(queryVector, seperateRows);

        vector<int> queryChange;
        /* Following function will store all the coordinates of the stars in query matrix vector */
        setQueryCoordinate(queryVector,queryChange,starCount);

        /* We got the query and stored, now it is time to find coordinates*/
        vector<string> coordinates;
        vector<string> placements;
        int count = 0;

        /* We need learn max size of queryVector's col to
 * break the program if it does not fit, to save processing time*/
        int maxSizeCol =0;
        findMaxQueryCol(maxSizeCol, queryVector);

        /* There will not be any place to put if query vector is bigger than our file matrix */
        if(maxSizeCol > fileVector[0].size() || queryVector.size() > fileVector.size()){
            cout << "No placements found." << endl;
        } else {

            /* Now we will look for the coordinates */
            int addTryRow = 0;
            int addTryCol = 0;

            findPlaceVector(fileVector,queryVector,maxSizeCol,queryChange, addTryRow, addTryCol,
                            coordinates,count,starCount,  placements);
            // If there are occurrences,print; else print that there are no placements
            if(placements.empty()) {
                cout << "No placements found." << endl;
            } else{
                for (int a = 0; a < placements.size(); a++) {
                    cout << "Placement number " << a + 1 << ":" << endl << placements[a] << endl;
                }
            }
            // Clear the input
            cin.clear();
            cin.ignore();
        }
    }
    inputFile.close();
    return 0;
}

