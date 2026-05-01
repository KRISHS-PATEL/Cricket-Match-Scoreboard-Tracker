#include <iostream>
#include <fstream>
using namespace std;

// ---------------- PLAYER ----------------
class Player {
public:
    string name;
    int runs = 0;
    int balls = 0;
};

// ---------------- TEAM ----------------
class Team {
public:
    string name;
    Player players[11];
};

// ---------------- MATCH ----------------
class Match {
public:
    Team t1, t2;

    int totalRuns = 0;
    int wickets = 0;
    int overs = 0;
    int balls = 0;

    Player* striker;
    Player* nonStriker;

    // -------- INIT TEAM --------
    void initTeam(Team &t) {
        cout << "Enter Team Name: ";
        cin >> t.name;
        cin.ignore();
        for(int i = 0; i < 11; i++) {
            cout << "Player " << i+1 << ": ";
            cin >> t.players[i].name;
        }
    }

    // -------- START MATCH --------
    void startMatch() {
        cout << "\n--- Team 1 ---\n";
        initTeam(t1);

        cout << "\n--- Team 2 ---\n";
        initTeam(t2);

        striker = &t1.players[0];
        nonStriker = &t1.players[1];
    }

    // -------- STRIKE RATE --------
    float getStrikeRate(Player *p) {
        if(p->balls == 0) return 0;
        return (p->runs * 100.0) / p->balls;
    }

    // -------- OVERS --------
    float getOversFloat() {
        return overs + (balls / 6.0);
    }

    // -------- CURRENT RUN RATE --------
    float getCRR() {
        float o = getOversFloat();
        if(o == 0) return 0;
        return totalRuns / o;
    }

    // -------- REQUIRED RUN RATE --------
    float getRRR(int runsNeeded, int ballsLeft) {
        float oversLeft = ballsLeft / 6.0;
        if(oversLeft == 0) return 0;
        return runsNeeded / oversLeft;
    }

    // -------- PLAY BALL --------
    void playBall() {
        int choice;
        cout << "\n1.Run  2.Wicket  3.Wide  4.No Ball\n";
        cout << "Enter: ";
        cin >> choice;

        if(choice == 1) {
            int run;
            cout << "Runs: ";
            cin >> run;

            striker->runs += run;
            striker->balls++;
            totalRuns += run;
            balls++;

            if(run % 2 == 1) {
                swap(striker, nonStriker);
            }
        }
        else if(choice == 2) {
            cout << "WICKET!\n";
            wickets++;
            balls++;

            if(wickets < 10) {
                int idx;
                cout << "New batsman index (0-10): ";
                cin >> idx;

                striker = &t1.players[idx];
                striker->runs = 0;
                striker->balls = 0;
            }
        }
        else if(choice == 3) {
            cout << "Wide!\n";
            totalRuns += 1;
        }
        else if(choice == 4) {
            cout << "No Ball!\n";
            totalRuns += 1;
        }

        // Over complete
        if(balls == 6) {
            overs++;
            balls = 0;
            swap(striker, nonStriker);
        }
    }


    void display(int target, int totalBalls, bool isSecondInnings) {
        cout << "\n============================\n";
        cout << "Score: " << totalRuns << "/" << wickets;
        cout << " (" << overs << "." << balls << ")\n";

        cout << "\nBatsmen:\n";
        cout << striker->name << ": "
             << striker->runs << "(" << striker->balls << ") "
             << "SR: " << getStrikeRate(striker) << endl;

        cout << nonStriker->name << ": "
             << nonStriker->runs << "(" << nonStriker->balls << ") "
             << "SR: " << getStrikeRate(nonStriker) << endl;

        cout << "\nCRR: " << getCRR() << endl;
      if(isSecondInnings){
        int ballsPlayed = overs * 6 + balls;
        int ballsLeft = totalBalls - ballsPlayed;
        int runsNeeded = target - totalRuns;

        cout << "Runs Needed: " << runsNeeded << endl;
        cout << "Balls Left: " << ballsLeft << endl;

        if(runsNeeded > 0 && ballsLeft > 0) {
            cout << "Required RR: "
                 << getRRR(runsNeeded, ballsLeft) << endl;
        }
      }

        cout << "============================\n";
    }


    void save() {
        ofstream file("match.txt");

        file << "Score: " << totalRuns << "/" << wickets << endl;
        file << "Overs: " << overs << "." << balls << endl;

        file.close();

        cout << "Match Saved!\n";
    }
};


int main() {
    Match m;

    m.startMatch();

    int totalBalls;
    cout << "\nEnter total balls: ";
    cin >> totalBalls;

    // -------- FIRST INNINGS --------
    for(int i = 0; i < totalBalls; i++) {
        m.playBall();
        m.display(0, totalBalls, false);

        if(m.wickets == 10) break;
    }

    int target = m.totalRuns + 1;

    cout << "\nTarget: " << target << endl;

    // Reset for 2nd innings
    m.totalRuns = 0;
    m.wickets = 0;
    m.overs = 0;
    m.balls = 0;

    m.striker = &m.t2.players[0];
    m.nonStriker = &m.t2.players[1];

    // -------- SECOND INNINGS --------
    for(int i = 0; i < totalBalls; i++) {
        m.playBall();
        m.display(target, totalBalls, true);

        if(m.totalRuns >= target) {
            cout << "\nTeam 2 Wins!\n";
            break;
        }

        if(m.wickets == 10) break;
    }

    if(m.totalRuns < target) {
        cout << "\nTeam 1 Wins!\n";
    }

    m.save();

    return 0;
}
