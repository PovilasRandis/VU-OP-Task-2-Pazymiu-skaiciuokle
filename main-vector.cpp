#include <algorithm>  //std::sort
#include <iomanip>    // std::setw
#include <iostream>
#include <random>  // std::rand, std::srand
#include <string>
#include <vector>

#define GRADE_MIN 1
#define GRADE_MAX 10

using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::left;
using std::numeric_limits;
using std::setprecision;
using std::setw;
using std::sort;
using std::streamsize;
using std::string;
using std::vector;

struct Student {
  string firstName;
  string lastName;
  vector<int> grades;
  int examGrade;
  double meanGrade;
  double finalGrade;
  double medianGrade;
};

void clearLine() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool confirm(const string &message, char yes = 'y', char no = 'n') {
  // https://stackoverflow.com/a/9158263
  string colorYellow = "\033[33m";
  string colorReset = "\033[0m";

  while (true) {
    cout << colorYellow << "-> " << message << " (" << yes << "/" << no << "): " << colorReset;

    char response;
    cin >> response;
    clearLine();
    if (response == yes) {
      return true;
    } else if (response == no) {
      return false;
    } else {
      cout << "Unknown character. ";
    }
  }
}

double findMean(vector<int> &array) {
  const int arraySize = array.size();
  int sum = 0;
  for (int i = 0; i < arraySize; i++) {
    sum += array[i];
  }

  return (double)sum / arraySize;
}

double findMedian(vector<int> &array) {
  sort(array.begin(), array.end());

  const int arraySize = array.size();
  const bool isOddNumber = arraySize % 2 != 0;

  if (isOddNumber) {
    return (double)array[arraySize / 2];
  }

  return (double)(array[(arraySize - 1) / 2] + array[arraySize / 2]) / 2.0;
}

int getNumberOfGrades() {
  cout << "Enter number of grades: ";
  int numGrades;
  cin >> numGrades;
  clearLine();

  while (numGrades < 0) {
    cout << "Value cannot be negative. Please enter new value: ";
    cin >> numGrades;
    clearLine();
  }

  return numGrades;
}

int getRandomIntegerInRange(int min, int max) {
  static bool first = true;
  if (first) {
    srand(time(NULL));  //seeding for the first time only!
    first = false;
  }
  return min + rand() % ((max + 1) - min);
}

bool isValidGrade(int grade) {
  return grade >= GRADE_MIN && grade <= GRADE_MAX;
}

void printResult(Student *student, bool showMeanGrade = true) {
  cout << left
       << setw(10) << student->firstName
       << setw(15) << student->lastName
       << setw(12) << fixed << setprecision(2)
       << (showMeanGrade ? student->finalGrade : student->medianGrade)
       << endl;
}

void printResults(vector<Student> &students, bool showMeanGrade = true) {
  cout << left
       << setw(10) << "Vardas"
       << setw(16) << "Pavardė"
       << "Galutinis " << (showMeanGrade ? "Vid." : "Med.") << endl;
  cout << "-----------------------------------------------------------" << endl;

  for (int i = 0; i < students.size(); i++) {
    printResult(&students[i], showMeanGrade);
  }
}

void processStudent(Student *student, bool shouldCalculateMean = true) {
  student->finalGrade = 0;
  student->meanGrade = 0;
  student->medianGrade = 0;
  if (student->grades.size() > 0) {
    if (shouldCalculateMean) {
      student->meanGrade = findMean(student->grades);
      student->finalGrade = 0.4 * student->meanGrade + 0.6 * student->examGrade;
    } else {
      student->medianGrade = findMedian(student->grades);
    }
  }
}

void printRandomGrades(Student *student) {
  cout << "Generated " << student->grades.size() << " random grades: ";
  for (int i = 0; i < student->grades.size(); i++) {
    cout << student->grades[i] << " ";
  }
  cout << endl;
  cout << "Generated random exam grade: " << student->examGrade << endl;
}

int main() {
  vector<Student> students;

  while (true) {
    Student student;

    cout << "Please enter first name: ";
    getline(cin, student.firstName);

    cout << "Please enter last name: ";
    getline(cin, student.lastName);

    const bool numberOfGradesIsKnown = confirm("Do you know the number of grades?");
    const int numGrades = numberOfGradesIsKnown ? getNumberOfGrades() : 0;

    bool shouldGenerateRandomGrades = confirm("Generate RANDOM grades (otherwise, enter grades MANUALLY)?");
    if (shouldGenerateRandomGrades) {
      if (numberOfGradesIsKnown) {
        for (int i = 0; i < numGrades; i++) {
          int grade = getRandomIntegerInRange(GRADE_MIN, GRADE_MAX);
          student.grades.push_back(grade);
        }
      } else {
        while (true) {
          int grade = getRandomIntegerInRange(0, GRADE_MAX);
          if (isValidGrade(grade)) {
            student.grades.push_back(grade);
          } else {
            break;
          }
        }
      }
      student.examGrade = getRandomIntegerInRange(GRADE_MIN, GRADE_MAX);
      printRandomGrades(&student);

    } else {
      if (numberOfGradesIsKnown) {
        if (numGrades > 0) {
          cout << "Enter grades: ";
          while (student.grades.size() != numGrades) {
            int grade;
            cin >> grade;
            if (!isValidGrade(grade)) {
              cout << "Grade " << grade << " at index " << student.grades.size() << " is out of range ("
                   << GRADE_MIN << "-" << GRADE_MAX << "). Fix it and continue entering." << endl;
              clearLine();
            } else {
              student.grades.push_back(grade);
            }
          }

          clearLine();
        }
      } else {
        while (true) {
          cout << "Enter grade [" << student.grades.size() << "] (type -1 to quit): ";

          int grade;
          cin >> grade;
          clearLine();

          if (grade == -1) {
            break;
          } else {
            if (!isValidGrade(grade)) {
              cout << "Grade " << grade << " is out of range ("
                   << GRADE_MIN << "-" << GRADE_MAX << ")." << endl;
            } else {
              student.grades.push_back(grade);
            }
          }
        }
      }

      cout << "Enter exam grade: ";
      cin >> student.examGrade;
      while (!isValidGrade(student.examGrade)) {
        cout << "Grade " << student.examGrade << " is out of range ("
             << GRADE_MIN << "-" << GRADE_MAX << "). Fix it and continue entering." << endl;
        clearLine();
        cin >> student.examGrade;
      }

      clearLine();
    }

    students.push_back(student);
    if (!confirm("Add another student?")) {
      break;
    }
  }

  bool shouldCalculateMean = confirm("Calculate MEAN (otherwise, calculate MEDIAN)?");
  for (int i = 0; i < students.size(); i++) {
    processStudent(&students[i], shouldCalculateMean);
  }

  cout << endl;
  printResults(students, shouldCalculateMean);
  cout << endl;

  return 0;
}