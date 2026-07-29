#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono>
using namespace std;

const int SIZE = 100;
const int MAX_HISTORY = 50;

struct Record {
    int id;
    int value;
};

// keeps track of every sort/search we run
// for a sort, sortedValues has the output
// for a search, searchedNumber has the number we looked for
struct LogEntry {
    int uniqueIdentifier;
    string type;
    string algorithmName;
    int sortedValues[SIZE];
    int searchedNumber;
    long long executionTime;
};

LogEntry history[MAX_HISTORY];
int historyCount = 0;

void fillRecords(Record list[], int n) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        list[i].id = i + 1;
        list[i].value = rand() % 1000 + 1;
    }
}

void showRecords(Record list[], int n, string lastAlgorithm) {
    cout << "\nCurrent order: " << lastAlgorithm << "\n";
    cout << "RECORD ID\tVALUE\n";
    for (int i = 0; i < n; i++) {
        cout << list[i].id << "\t\t" << list[i].value << endl;
    }
}

void showHistory(int n) {
    cout << "\n--- Activity Log ---\n";
    if (historyCount == 0) {
        cout << "No records found.\n";
        return;
    }
    for (int i = 0; i < historyCount; i++) {
        cout << "UniqueIdentifier: " << history[i].uniqueIdentifier
             << " | Type: " << history[i].type
             << " | Algorithm: " << history[i].algorithmName << "\n";

        if (history[i].type == "Sort") {
            cout << "Output: [";
            for (int v = 0; v < n; v++) {
                cout << history[i].sortedValues[v];
                if (v < n - 1) cout << ", ";
            }
            cout << "]\n";
        } else {
            cout << "Number Searched: " << history[i].searchedNumber << "\n";
        }
    }
}

void addSortHistory(Record list[], int n, string algorithmName, long long execTime) {
    if (historyCount >= MAX_HISTORY) return;

    history[historyCount].uniqueIdentifier = historyCount + 1;
    history[historyCount].type = "Sort";
    history[historyCount].algorithmName = algorithmName;
    history[historyCount].executionTime = execTime;
    for (int i = 0; i < n; i++) {
        history[historyCount].sortedValues[i] = list[i].value;
    }
    historyCount++;
}

void addSearchHistory(string algorithmName, int number) {
    if (historyCount >= MAX_HISTORY) return;

    history[historyCount].uniqueIdentifier = historyCount + 1;
    history[historyCount].type = "Search";
    history[historyCount].algorithmName = algorithmName;
    history[historyCount].searchedNumber = number;
    historyCount++;
}

void swapRecords(Record &a, Record &b) {
    Record temp = a;
    a = b;
    b = temp;
}

void selectionSort(Record list[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int smallest = i;
        for (int j = i + 1; j < n; j++) {
            if (list[j].value < list[smallest].value) {
                smallest = j;
            }
        }
        swapRecords(list[i], list[smallest]);
    }
}

int partition(Record list[], int start, int end) {
    int pivot = list[end].value;
    int i = start - 1;

    for (int j = start; j < end; j++) {
        if (list[j].value < pivot) {
            i++;
            swapRecords(list[i], list[j]);
        }
    }
    swapRecords(list[i + 1], list[end]);
    return i + 1;
}

void quickSort(Record list[], int start, int end) {
    if (start < end) {
        int p = partition(list, start, end);
        quickSort(list, start, p - 1);
        quickSort(list, p + 1, end);
    }
}

void merge(Record list[], int start, int mid, int end) {
    Record temp[SIZE];
    int i = start, j = mid + 1, k = start;

    while (i <= mid && j <= end) {
        if (list[i].value <= list[j].value) {
            temp[k] = list[i];
            i++;
        } else {
            temp[k] = list[j];
            j++;
        }
        k++;
    }

    while (i <= mid) {
        temp[k] = list[i];
        i++;
        k++;
    }

    while (j <= end) {
        temp[k] = list[j];
        j++;
        k++;
    }

    for (int x = start; x <= end; x++) {
        list[x] = temp[x];
    }
}

void mergeSort(Record list[], int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        mergeSort(list, start, mid);
        mergeSort(list, mid + 1, end);
        merge(list, start, mid, end);
    }
}

int binarySearch(Record list[], int n, int target) {
    int low = 0, high = n - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (list[mid].value == target) {
            return mid;
        } else if (list[mid].value < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int interpolationSearch(Record list[], int n, int target) {
    int low = 0, high = n - 1;

    while (low <= high && target >= list[low].value && target <= list[high].value) {
        if (list[high].value == list[low].value) {
            if (list[low].value == target) return low;
            return -1;
        }

        int pos = low + ((target - list[low].value) * (high - low)) / (list[high].value - list[low].value);

        if (list[pos].value == target) {
            return pos;
        } else if (list[pos].value < target) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    return -1;
}

// reads a number safely, if user types letters it just asks again
// instead of getting stuck
int readIntChoice(string prompt) {
    int value;
    cout << prompt;
    cin >> value;

    while (cin.fail()) {
        if (cin.eof()) {
            cout << "\nNo more input available, closing program.\n";
            exit(0);
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a number: ";
        cin >> value;
    }

    cin.ignore(1000, '\n');
    return value;
}

void sortMenu(Record list[], int n, bool &sorted, string &lastAlgorithm) {
    cout << "\n1. Selection Sort\n2. Quick Sort\n3. Merge Sort\n0. Go Back\n";
    int choice = readIntChoice("Choose: ");

    if (choice == 0) {
        return;
    }
    
    auto startTime = chrono::high_resolution_clock::now();
    
    if (choice == 1) {
        selectionSort(list, n);
        lastAlgorithm = "Selection Sort";
    } else if (choice == 2) {
        quickSort(list, 0, n - 1);
        lastAlgorithm = "Quick Sort";
    } else if (choice == 3) {
        mergeSort(list, 0, n - 1);
        lastAlgorithm = "Merge Sort";
    } else {
        cout << "Invalid choice.\n";
        return;
    }

    auto endTime = chrono::high_resolution_clock::now();
    long long duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

    sorted = true;
    cout << "Done with " << lastAlgorithm << " in " << duration << " ms.\n";
    addSortHistory(list, n, lastAlgorithm, duration);
    showRecords(list, n, lastAlgorithm);
}

void searchMenu(Record list[], int n, bool sorted, string lastAlgorithm) {
    if (!sorted) {
        cout << "\nPlease sort the records first (option B).\n";
        return;
    }

    cout << "\n1. Binary Search\n2. Interpolation Search\n0. Go Back\n";
    int choice = readIntChoice("Choose: ");

    if (choice == 0) return;
    if (choice != 1 && choice != 2) {
        cout << "Invalid choice.\n";
        return;
    }

    int target = readIntChoice("Enter number to search: ");

    int pos = -1;
    string algorithmName;
    if (choice == 1) {
        pos = binarySearch(list, n, target);
        algorithmName = "Binary Search";
    } else {
        pos = interpolationSearch(list, n, target);
        algorithmName = "Interpolation Search";
    }

    if (pos == -1) {
        cout << "Value " << target << " not found (records currently sorted by " << lastAlgorithm << ").\n";
    } else {
        cout << "Found! Record ID " << list[pos].id << " holds value " << target
             << " (records currently sorted by " << lastAlgorithm << ").\n";
    }

    addSearchHistory(algorithmName, target);
}

void showExecutionTimeSummary() {
    cout << "\n\n========== EXECUTION TIME SUMMARY ==========\n";
    if (historyCount == 0) {
        cout << "No algorithms were executed.\n";
        return;
    }
    
    cout << "\nAlgorithm Execution Times:\n";
    cout << "--------------------------------------------\n";
    for (int i = 0; i < historyCount; i++) {
        if (history[i].type == "Sort") {
            cout << history[i].algorithmName << ": " << history[i].executionTime << " ms\n";
        }
    }
    cout << "--------------------------------------------\n";
}

int main() {
    Record records[SIZE];
    fillRecords(records, SIZE);
    bool sorted = false;
    string lastAlgorithm = "Unsorted (original order)";
    char choice;

    do {
        cout << "\n===== Records Management System =====\n";
        cout << "A. Display All Records\n";
        cout << "B. Sort Records\n";
        cout << "C. Search for Number\n";
        cout << "D. Exit Application\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input, please enter a letter A-D.\n";
            continue;
        }

        cin.ignore(1000, '\n');
        choice = toupper(choice);

        if (choice == 'A') {
            showRecords(records, SIZE, lastAlgorithm);
            showHistory(SIZE);
        } else if (choice == 'B') {
            sortMenu(records, SIZE, sorted, lastAlgorithm);
        } else if (choice == 'C') {
            searchMenu(records, SIZE, sorted, lastAlgorithm);
        } else if (choice == 'D') {
            showExecutionTimeSummary();
            cout << "\nThanks for using the program.\n";
        } else {
            cout << "Invalid choice, try again.\n";
        }

    } while (choice != 'D');

    return 0;
}