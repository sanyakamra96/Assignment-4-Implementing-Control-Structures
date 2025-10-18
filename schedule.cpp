#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// ------------------ Constants ------------------
const vector<string> DAYS = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                             "Friday", "Saturday", "Sunday"};
const vector<string> SHIFTS = {"Morning", "Afternoon", "Evening"};
const int MAX_DAYS_PER_EMPLOYEE = 5;
const int MIN_EMPLOYEES_PER_SHIFT = 2;

// ------------------ Employee Structure ------------------
struct Employee {
  string name;
  unordered_map<string, string> preferredShifts;  // Day -> Preferred shift
  int daysWorked = 0;
};

// ------------------ Helper Function ------------------
string getRandomEmployee(vector<Employee*>& available) {
  if (available.empty()) return "None";
  int index = rand() % available.size();
  available[index]->daysWorked++;
  return available[index]->name;
}

// ------------------ Main Program ------------------
int main() {
  srand(time(0));

  int numEmployees;
  cout << "Enter number of employees: ";
  cin >> numEmployees;
  cin.ignore();  // clear newline

  vector<Employee> employees(numEmployees);

  // ----------- INPUT SECTION -----------
  for (int i = 0; i < numEmployees; ++i) {
    cout << "\nEnter employee name: ";
    getline(cin, employees[i].name);

    cout << "Enter preferred shift (morning/afternoon/evening) for each day:\n";
    for (auto& day : DAYS) {
      string shift;
      cout << "  " << day << ": ";
      getline(cin, shift);
      employees[i].preferredShifts[day] = shift;
    }
  }

  // schedule[day][shift] = list of employees
  unordered_map<string, unordered_map<string, vector<string>>> schedule;

  // ----------- SCHEDULING LOGIC -----------
  for (auto& day : DAYS) {
    for (auto& shift : SHIFTS) {
      schedule[day][shift] = {};
    }

    for (auto& emp : employees) {
      if (emp.daysWorked >= MAX_DAYS_PER_EMPLOYEE) continue;

      string prefShift = emp.preferredShifts[day];

      // Assign to preferred shift if space
      if (schedule[day][prefShift].size() < 3) {
        schedule[day][prefShift].push_back(emp.name);
        emp.daysWorked++;
      } else {
        // ---------- CONFLICT RESOLUTION ----------
        bool assigned = false;

        // Try another shift same day
        for (auto& alt : SHIFTS) {
          if (schedule[day][alt].size() < 3) {
            schedule[day][alt].push_back(emp.name);
            emp.daysWorked++;
            assigned = true;
            break;
          }
        }

        // Try next day if still unassigned
        if (!assigned) {
          for (auto& nextDay : DAYS) {
            if (nextDay == day) continue;
            for (auto& alt : SHIFTS) {
              if (schedule[nextDay][alt].size() < 3 &&
                  emp.daysWorked < MAX_DAYS_PER_EMPLOYEE) {
                schedule[nextDay][alt].push_back(emp.name);
                emp.daysWorked++;
                assigned = true;
                break;
              }
            }
            if (assigned) break;
          }
        }
      }
    }
  }

  // ----------- ENSURE MIN 2 EMPLOYEES PER SHIFT -----------
  for (auto& day : DAYS) {
    for (auto& shift : SHIFTS) {
      while (schedule[day][shift].size() < MIN_EMPLOYEES_PER_SHIFT) {
        vector<Employee*> available;
        for (auto& emp : employees) {
          if (emp.daysWorked < MAX_DAYS_PER_EMPLOYEE &&
              find(schedule[day][shift].begin(), schedule[day][shift].end(),
                   emp.name) == schedule[day][shift].end()) {
            available.push_back(&emp);
          }
        }
        string chosen = getRandomEmployee(available);
        if (chosen != "None")
          schedule[day][shift].push_back(chosen);
        else
          break;
      }
    }
  }

  // ----------- OUTPUT FINAL SCHEDULE -----------
  cout << "\n========== FINAL WEEKLY SCHEDULE ==========\n";
  for (auto& day : DAYS) {
    cout << "\n" << day << ":\n";
    for (auto& shift : SHIFTS) {
      cout << "  " << shift << ": ";
      for (auto& emp : schedule[day][shift]) {
        cout << emp << " ";
      }
      cout << endl;
    }
  }

  cout << "\nScheduling completed successfully!\n";
  return 0;
}
