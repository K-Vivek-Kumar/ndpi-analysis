#include <bits/stdc++.h>
#include <filesystem>  // C++17 required
namespace fs = std::filesystem;
using namespace std;

map<string, map<double, int>> fun_occurances_percentage_wise; // function -> { threshold -> count }
map<string, int> fun_occurances; // function -> count

void processFile(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    map<string, double> functionMap;
    string line;
    
    while (getline(inputFile, line)) {
        istringstream stream(line);
        double overhead;
        string var1, var2, var3, func_Name;

        stream >> overhead >> var1 >> var2 >> var3 >> func_Name;

        functionMap[func_Name] = overhead;
        fun_occurances[func_Name]++;
    }

    inputFile.close();

    for (auto it : functionMap) {
        double overhead = it.second;
        string func_Name = it.first;

        if (overhead > 5) fun_occurances_percentage_wise[func_Name][5]++;
        if (overhead > 10) fun_occurances_percentage_wise[func_Name][10]++;
        if (overhead > 15) fun_occurances_percentage_wise[func_Name][15]++;
        if (overhead > 20) fun_occurances_percentage_wise[func_Name][20]++;
    }
}

int main() {
    string outputDir = "./output";

    // Traverse the output directory
    for (const auto& entry : fs::directory_iterator(outputDir)) {
        if (fs::is_directory(entry)) {
            string filePath = entry.path().string() + "/top10_funcs.txt";
            if (fs::exists(filePath)) {
                cout << "Processing: " << filePath << endl;
                processFile(filePath);
            }
        }
    }

    // Open files for writing output
    ofstream freqFile("functions_freq.txt");
    ofstream overheadFile("function_overheads.txt");

    if (!freqFile || !overheadFile) {
        cerr << "Error creating output files!" << endl;
        return 1;
    }

    // sort function occurrences based on frequency
    vector<pair<string, int>> sortedFunOccurrences(fun_occurances.begin(), fun_occurances.end());
    sort(sortedFunOccurrences.begin(), sortedFunOccurrences.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    // Write function occurrence frequencies
    freqFile << "Function Name\t\tOccurrences\n";
    freqFile << "--------------------------------------\n";
    for (auto& it : sortedFunOccurrences) {
        freqFile << setw(40) << left << it.first << "\t" << it.second << endl;
    }
    // Write function overhead percentage wise
    overheadFile << "Function Name\t\tOverhead Counts (>5%, >10%, >15%, >20%)\n";
    overheadFile << "------------------------------------------------------------\n";
    for (auto& it : fun_occurances_percentage_wise) {
        overheadFile << setw(40) << left << it.first;
        for (auto& threshold : {5, 10, 15, 20}) {
            overheadFile << "\t" << it.second[threshold];
        }
        overheadFile << endl;
    }

    cout << "\nResults written to 'functions_freq.txt' and 'function_overheads.txt' successfully.\n";

    freqFile.close();
    overheadFile.close();

    return 0;
}
