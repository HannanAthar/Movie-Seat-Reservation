#include<iostream>

#include<string>

#include<conio.h>

#include<json/value.h>

#include<json/json.h>

#include<fstream>

using namespace std;

void displayMessage() {
    cout << "\n\t*****************************";
    cout << "\n\t*                           *";
    cout << "\n\t*                           *";
    cout << "\n\t*  Movie Seat Reservation   *";
    cout << "\n\t*          system           *";
    cout << "\n\t*                           *";
    cout << "\n\t*                           *";
    cout << "\n\t*****************************";
    cout << "\n\n\t Welcome To Universal Cinema";
    cout << "\n\n\n\n\n\t Press Any Key To Continue: ";
    _getch();
    cout << "\n\n";
}

Json::Value readFile() {
    ifstream file("movie.json"); //opening file using fstream
    Json::Value jsonData; //Store JSON data and make it accessible
    Json::Reader jsonReader; //for reading the data
    jsonReader.parse(file, jsonData); //getting data from the JSON file.
    return jsonData;
}

int selectDay(Json::Value data) {
    int day;
    for (int i = 0; i <= 6; i++) { //Show Days form file
        cout << i << ": " << data[i]["day"] << endl;
    }
    cout << "\n" << "Select Day Number = ";
    cin >> day; //Asking User to Enter Day Number
    return day;
}

int displayMovies(int day, Json::Value data) {
    int movieNumber;
    for (int i = 0; i <= 4; i++) { //Show Movies Name,Available Seates and Time from file
        cout << "\n" << i << ": Movie Name: " << data[day]["movies"][i]["name"] << endl;
        cout << "Available Seats: " << data[day]["movies"][i]["seats"] << endl;
        cout << "Time: " << data[day]["movies"][i]["time"] << endl;
    }
    cout << "\n" << "Select a Movie Number = ";
    cin >> movieNumber; //Asking User to Enter Movie Number
    return movieNumber;
}

int getSeats() {
    int seats;
    cout << "Enter Number of Seats = ";
    cin >> seats; //Asking User to Enter Seates he want to reserve
    return seats;
}

void writeData(int day, int movieNumber, int seats, string name, string number, Json::Value data) {
    int remainingsSeats = data[day]["movies"][movieNumber]["seats"].asInt() - seats;
    Json::Value details = data[day]["movies"][movieNumber]["details"];
    Json::Value arr;
    Json::Value peopleDetails;
    peopleDetails["name"] = name;
    peopleDetails["number"] = number;
    for (int i = 0; i <= 6; i++) {
        if (i == day) {
            Json::Value arr2;
            for (int j = 0; j <= 4; j++) {
                if (movieNumber == j) {
                    Json::Value obj;
                    Json::Value detailsArr;
                    for (int k = 0; k <= data[i]["movies"][j]["details"].size(); k++) {
                        if (k == data[i]["movies"][j]["details"].size()) {
                            detailsArr[k] = peopleDetails;
                        }
                        else {
                            detailsArr[k] = data[i]["movies"][j]["details"][k];
                        }
                    }
                    obj["name"] = data[i]["movies"][j]["name"];
                    obj["seats"] = remainingsSeats;
                    obj["details"] = detailsArr;
                    obj["time"] = data[i]["movies"][j]["time"];
                    arr2[j] = Json::Value(obj);
                }
                else {
                    arr2[j] = Json::Value(data[i]["movies"][j]);
                }
            }
            Json::Value obj;
            obj["day"] = data[i]["day"];
            obj["movies"] = arr2;
            arr[i] = Json::Value(obj);
        }
        else {
            arr[i] = Json::Value(data[i]);
        }
    }
    ofstream file_id; //writing data from ofstream
    file_id.open("movie.json");
    Json::StyledWriter styledWriter;
    file_id << styledWriter.write(arr);
    file_id.close();
}
int main() {
    char choice;
    do {
        displayMessage();
        Json::Value jsonFileData = readFile();
        int day;
        while (true) {
            day = selectDay(jsonFileData);
            if (day > 6 || day < 0) {
                cout << "\n" << "Select a Valid Day" << endl;
            }
            else {
                break;
            }
        }
        int movieNumber;
        while (true) {
            movieNumber = displayMovies(day, jsonFileData);
            if (movieNumber > 4 || movieNumber < 0) {
                cout << "\n" << "Select a Valid Movie Number" << endl;
            }
            else {
                break;
            }
        }
        int numberOfSeats;
        while (true) {
            numberOfSeats = getSeats();
            int availableSeats = jsonFileData[day]["movies"][movieNumber]["seats"].asInt();
            if (numberOfSeats > availableSeats || numberOfSeats == 0) {
                cout << "Seats are Not Available, Select Available Seats" << endl;
            }
            else {
                break;
            }
        }
        string name, number;;
        cout << "Enter Name = ";
        
        getline(cin >> ws, name);
        cout << "Enter Number = ";
        cin >> number;
        writeData(day, movieNumber, numberOfSeats, name, number, jsonFileData);
        void* x;
        x = &writeData;
        cout << "\n\n\n\t Press Y to continue. Press any Key To Exit = ";
        cin >> choice;
    } while (choice == 'Y' || choice == 'y');
    return 0;
}