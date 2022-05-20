/*
 Author:
    Huseyin Karakaya
*/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int probing_counter = 0;

struct Key {
    bool team; // true if home, false if away
    int shirt_no;
};

struct Value {
    int goals;
    int assists;
    int score;
};

struct Pair {
    Key key;
    Value value;
};

Pair * table = new Pair[28];

int hash_f(bool team, int shirt_number) {
    if(team) {
        return (0+ shirt_number) % 28;
    }else {
        return (99+ shirt_number) % 28;
    } 
}

int probing(bool team, int shirt_number) {
    probing_counter += 1;
    if(team) {
        return (0 + shirt_number + (probing_counter*probing_counter)) % 28;
    }else {
        return (99 + shirt_number + (probing_counter*probing_counter)) % 28;
    } 
}

void set_item(bool team, int shirt_number, int goal, int assist, int score) {
    int index = hash_f(team, shirt_number);

    Key key;
    key.team = team;
    key.shirt_no = shirt_number;
    
    Value value;
    value.goals = goal;
    value.assists = assist;
    value.score = score;
    
    Pair pair;
    pair.key = key;
    pair.value = value;

    //Checking if data exists..
    //Assumed shirt number cannot be zero
    if(table[index].key.shirt_no == 0) {
        table[index] = pair;
    }else {
        //Checking if there is a collision..
        if(table[index].key.shirt_no == key.shirt_no) {
            table[index].value.assists += assist;
            table[index].value.goals += goal;
            table[index].value.score += score;
        }else {
            int new_index = probing(team,shirt_number);
            table[new_index] = pair;
        }
    }
}

bool is_home_away(string home_or_away) {
    if(home_or_away == "H") {
        return true;
    }
    return false;
}


int main(int argc, char** argv) {

    string path = argv[1];

    int min = atoi(argv[2]);

    ifstream inputFile;
    inputFile.open(path);

    if(inputFile.fail()) {
        cerr << "Unable to open the file: " << path << endl;
        return 1;
    }

    /// Use getline() to read the first line, then begin reading the rest of the stream.
    string line = "";
    getline(inputFile, line);
    line = "";

    while(getline(inputFile, line)) {
        if(line == "") {
            break;
        }
        string tempString = "";
        int minute;
        string home_or_away;
        bool is_home;
        int shirt_no;
        string event_code;

        stringstream inputString(line);

        getline(inputString, tempString, ',');
        minute = atoi(tempString.c_str());

        getline(inputString, home_or_away, ',');

        tempString = "";
        getline(inputString, tempString, ',');
        shirt_no = atoi(tempString.c_str());

        getline(inputString, event_code, ',');

        is_home = is_home_away(home_or_away);

        if(minute<=min) {
            int contribution = 0;
            int goals = 0;
            int assists = 0;

            if(event_code=="S") {
                contribution += 20;
                goals += 1;
            }else if(event_code=="A") {
                contribution += 10;
                assists += 1;
            }else{
                if(event_code == "T") {
                    contribution += 1;
                }else if(event_code == "P") {
                    contribution += 1;
                }else if(event_code == "SH") {
                    contribution += 2;
                }else if(event_code == "SG") {
                    contribution += 3;
                }else if(event_code == "IP") {
                    contribution -= 1;
                }else if(event_code == "YC") {
                    contribution -= 10;
                }else if(event_code == "RC") {
                    contribution -= 20;
                }else if(event_code == "L") {
                    contribution -= 2;
                }else if(event_code == "CG") {
                    contribution -= 5;
                }
            }
            set_item(is_home, shirt_no, goals, assists, contribution);
        }
        line = "";
    }

    inputFile.close();

    printf("Index\tTeam\tNo\tGoals\tAssists\tScore\n");
    for (int i = 0; i < 28; i++) {
        Pair pair = table[i];
        if(pair.key.team == false) {
            if(pair.key.shirt_no == 0) {
                printf("%d\t \t \t \t \t \n",i);
                continue;
            }else {
                printf("%d\tA\t%d\t%d\t%d\t%d\n",
                i,
                pair.key.shirt_no,
                pair.value.goals,
                pair.value.assists,
                pair.value.score);
            }
        }else {
            printf("%d\tH\t%d\t%d\t%d\t%d\n",
            i,
            pair.key.shirt_no,
            pair.value.goals,
            pair.value.assists,
            pair.value.score);
        }
    }
    printf("\n");
    return 0;
}
