#include <algorithm>
#include <array>
#include <assert.h>
#include <bits/stdc++.h>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

#include "EchoParse.h"
#include "FilePair.h"

#include "libs/duckx.hpp"
#include "libs/xlsxio/include/xlsxio_write.h"

using namespace std;

int main() {
    xlsxiowriter *handle = new xlsxiowriter;
    if ((*handle = xlsxiowrite_open(OUTPUT_FILE_NAME.c_str(), "MySheet")) ==
        NULL) {
        cerr << "Error creating .xlsx file" << endl;
        return 1;
    }

    addLabels(handle);

    cout << "Reading from:" << endl;

    map<int, FilePair> files;
    generateFileMap(files);

    for (auto mapIt = files.cbegin(); mapIt != files.cend(); ++mapIt) {
        if (filesystem::exists(mapIt->second.pre)) {
            processFile(handle, mapIt->second.pre, FilePairType::PRE);
        } else {
            for (int i = 0; i < KEY_VAL_SIZE + KEY_TEXT_SIZE + 7; i++) {
                // Skip to post section
                xlsxiowrite_add_cell_string(*handle, "");
            }
        }

        if (filesystem::exists(mapIt->second.post)) {
            processFile(handle, mapIt->second.post, FilePairType::POST);
        } else {
            xlsxiowrite_next_row(*handle);
        }
    }
    xlsxiowrite_close(*handle);
}

void addLabels(const xlsxiowriter *handle) {
    xlsxiowrite_add_column(*handle, "PRE", 10);
    xlsxiowrite_add_column(*handle, "MRN", 7);
    xlsxiowrite_add_column(*handle, "Test Date", 10);
    xlsxiowrite_add_column(*handle, "Age", 3);
    xlsxiowrite_add_column(*handle, "Heart Rate", 10);
    xlsxiowrite_add_column(*handle, "Rhythm", 10);
    xlsxiowrite_add_column(*handle, "Blood Pressure", 14);
    for (const auto &columnName : KEY_VALS) {
        xlsxiowrite_add_column(*handle, columnName.c_str(),
                               columnName.length());
    }
    for (const auto &columnName : KEY_TEXT) {
        xlsxiowrite_add_column(*handle, columnName.c_str(),
                               columnName.length() * 3);
    }

    xlsxiowrite_add_column(*handle, "POST", 10);
    xlsxiowrite_add_column(*handle, "MRN", 7);
    xlsxiowrite_add_column(*handle, "Test Date", 10);
    xlsxiowrite_add_column(*handle, "Age", 3);
    xlsxiowrite_add_column(*handle, "Heart Rate", 10);
    xlsxiowrite_add_column(*handle, "Rhythm", 10);
    xlsxiowrite_add_column(*handle, "Blood Pressure", 14);
    for (const auto &columnName : KEY_VALS) {
        xlsxiowrite_add_column(*handle, columnName.c_str(),
                               columnName.length());
    }
    for (const auto &columnName : KEY_TEXT) {
        xlsxiowrite_add_column(*handle, columnName.c_str(),
                               columnName.length() * 3);
    }

    xlsxiowrite_next_row(*handle);
}

void generateFileMap(map<int, FilePair> &files) {
    for (const auto &file : filesystem::directory_iterator(INPUT_DOCS_PATH)) {
        const string filename = file.path().stem();
        const int usPos = filename.find("_");
        const int id = stoi(filename.substr(0, usPos));
        const string preOrPost =
            filename.substr(usPos + 1, filename.find(".") - usPos);
        FilePairType pairType;
        if (preOrPost == "pre") {
            pairType = FilePairType::PRE;
        } else if (preOrPost == "post") {
            pairType = FilePairType::POST;
        } else {
            cout << "Skipping: " << filename << endl;
            continue;
        }

        if (files.count(id) == 1) {
            files[id].addFile(file, pairType);
        } else {
            const filesystem::path nonExistentFile{};
            files[id] = FilePair{
                pairType == FilePairType::PRE ? file : nonExistentFile,
                pairType == FilePairType::POST ? file : nonExistentFile};
        }
    }
}

void processFile(xlsxiowriter *handle, filesystem::path file,
                 FilePairType pairType) {
    cout << file << endl;

    array<double, KEY_VAL_SIZE> values;
    fill_n(values.begin(), KEY_VAL_SIZE, INT_MIN);

    array<string, KEY_TEXT_SIZE> text;

    duckx::Document doc(file);
    doc.open();

    duckx::Paragraph mrnParagraph, testDateParagraph, ageParagraph,
        rhythmHeartRateBloodPressureParagraph;

    bool pastMeasurements = false;
    string line = "";
    string multiLine = "";
    string testDate = "";
    for (auto p : doc.paragraphs()) {
        for (auto r : p.runs()) {
            if (r.get_text() == "Measurements" ||
                r.get_text() == "MEASUREMENTS AND CALCULATIONS:") {
                pastMeasurements = true;
            } else if (r.get_text().find("MRN") != string::npos ||
                       r.get_text().find("Med Rec#") != string::npos ||
                       r.get_text().find("Medical Rec #") != string::npos) {
                mrnParagraph = p;
            } else if (r.get_text().find("Age:") != string::npos ||
                       r.get_text().find("Age :") != string::npos) {
                ageParagraph = p;
            } else if (r.get_text().find("Rhythm") != string::npos) {
                rhythmHeartRateBloodPressureParagraph = p;
            } else if (r.get_text() == "Test ") {
                testDate += "Test ";
            } else if (r.get_text() == "Date :") {
                if (testDate == "Test ") {
                    testDate += "Date :";
                }
            } else if (testDate == "Test Date :") {
                testDate = "FOUND";
            } else if (testDate == "FOUND") {
                testDateParagraph = p;
                testDate = "";
            }
        }
        if (pastMeasurements) {
            line = "";
        } else if (p.runs().get_text().length() > 0) {
            const char startLineChar = p.runs().get_text().at(0);
            // Assumption: A new line begins when the starting character of
            // that line is uppercase. Reasoning: Some lines in the docx
            // file have unexpected paragraph breaks in random parts of a
            // KEY_TEXT entry.
            if (startLineChar >= 'A' && startLineChar <= 'Z') {
                line = "";
            }
        }
        for (auto r : p.runs()) {
            line.append(r.get_text());
        }
        const string proxToFind = "ProxAsc Aorta";
        const auto proxPos = line.find(proxToFind);
        if (proxPos != string::npos) {
            line.replace(proxPos, proxToFind.length() + 1, "Prox Asc Aorta");
        }
        if (!pastMeasurements) {
            extractText(text, line);
        } else {
            // To store multiLine entries (example shown below):
            //
            // LV Mass
            // indexed
            // [value]
            //
            // We store each line, but only reset when
            // our current line is accepted by extractVals or
            // a multiLine is accepted below (in the else clause)
            multiLine += multiLine.length() == 0 ? line : " " + line;
            if (extractVals(values, line)) {
                multiLine = "";
            } else {
                // [value] is at the start of the line and multiLine isn't
                // empty
                // => use multiLine as the KEY_VAL name and store [value]
                if (multiLine.length() > 0 && line.length() > 0 &&
                    isdigit(line.at(0))) {
                    for (int i = 0; i < KEY_VAL_SIZE; i++) {
                        const auto indexFound = multiLine.find(KEY_VALS[i]);
                        if (indexFound != string::npos) {
                            // If keyValPlusOne matches in KEY_VALS, we know
                            // something longer is in KEY_VALS. Continue for
                            // full match (greedy matching)
                            const string keyValPlusOne = multiLine.substr(
                                indexFound, KEY_VALS[i].length() + 1);
                            if (any_of(begin(KEY_VALS), end(KEY_VALS),
                                       [keyValPlusOne](string val) {
                                           return val.find(keyValPlusOne) !=
                                                  string::npos;
                                       })) {
                                continue;
                            } else {
                                values[i] = cleanAndConvertVal(line);
                                multiLine = "";
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // Skip over "PRE/POST" label
    xlsxiowrite_add_cell_string(*handle, "");

    cout << "\tMRN: " << getCleanEntry(mrnParagraph) << endl;
    setMRN(mrnParagraph, handle);
    setTestDate(testDateParagraph, handle);
    setAge(ageParagraph, handle);
    setRhythmHeartRateBloodPressure(rhythmHeartRateBloodPressureParagraph,
                                    handle);

    insertVals(values, handle);
    insertText(text, handle);
    if (pairType == FilePairType::POST) {
        xlsxiowrite_next_row(*handle);
        cout << endl;
    }
}

void insertText(array<string, KEY_TEXT_SIZE> &text,
                const xlsxiowriter *handle) {
    for (int i = 0; i < KEY_TEXT_SIZE; i++) {
        if (text.at(i) != "") {
            xlsxiowrite_add_cell_string(*handle, text.at(i).c_str());
        } else {
            xlsxiowrite_add_cell_string(*handle, "[missing]");
        }
    }
}

void extractText(array<string, KEY_TEXT_SIZE> &text, const string &line) {
    for (int i = 0; i < KEY_TEXT_SIZE; i++) {
        // Look only if KEY_TEXT is at start of line
        if (line.find(KEY_TEXT[i]) == 0) {
            // Store the line excluding the keyText name
            // + 2 (colon and space that follow)
            if (line.length() < KEY_TEXT[i].length() + 2) {
                cerr << "Skipping line: " << line << endl;
                text.at(i) = "";
                return;
            }
            string entry = line.substr(KEY_TEXT[i].length() + 2);
            text.at(i) = entry;
        }
    }
}

void insertVals(array<double, KEY_VAL_SIZE> &values,
                const xlsxiowriter *handle) {
    for (int i = 0; i < KEY_VAL_SIZE; i++) {
        if (values.at(i) != INT_MIN) {
            xlsxiowrite_add_cell_float(*handle, values.at(i));
        } else {
            xlsxiowrite_add_cell_string(*handle, "[missing]");
        }
    }
}

bool extractVals(array<double, KEY_VAL_SIZE> &values, const string &line) {
    for (int i = 0; i < KEY_VAL_SIZE; i++) {
        if (line.find(KEY_VALS[i]) != string::npos) {
            // Store the line excluding the keyVal name
            string entry = line.substr(KEY_VALS[i].length());
            if (entry.length() == 0) {
                break;
            }
            if (entry.substr(0, entry.find(':')).length() > 0) {
                // If there is more beyond the keyVal we detected,
                // see if KEY_VALS[i] combined with text up to ':' is another
                // keyVal
                const string longerKeyVal =
                    KEY_VALS[i] + entry.substr(0, entry.find(':'));

                // If there is, ignore this keyVal match (we are only interested
                // in a full match)
                if (std::find(begin(KEY_VALS), end(KEY_VALS), longerKeyVal) !=
                    end(KEY_VALS)) {
                    continue;
                }
            }
            // Remove beginning whitespace
            entry.erase(0, 1);

            values.at(i) = cleanAndConvertVal(entry);
            return true;
        }
    }
    return false;
}

double cleanAndConvertVal(string entry) {
    // Remove the normal range or the following dash
    auto normalRange = entry.find(" (");
    if (normalRange != string::npos) {
        entry.erase(entry.find(" ("));
    }

    auto dash = entry.find(" -");
    if (dash != string::npos) {
        entry.erase(entry.find(" -"));
    }

    // Conversion to double
    istringstream ss{entry};
    double value;
    ss >> value;
    return value;
}

void setMRN(const duckx::Paragraph &mrnParagraph, const xlsxiowriter *handle) {
    const int mrn = convertFirstNum(getCleanEntry(mrnParagraph));
    assert(to_string(mrn).length() == 7);

    xlsxiowrite_add_cell_float(*handle, mrn);
}

void setTestDate(duckx::Paragraph testDateParagraph,
                 const xlsxiowriter *handle) {
    string val = "";
    for (const auto &r : testDateParagraph.runs()) {
        val.append(r.get_text());
    }
    const string date = getCleanEntry(testDateParagraph);
    // assert(date.length() == 10);
    xlsxiowrite_add_cell_string(*handle, date.c_str());
}

void setAge(const duckx::Paragraph &ageParagraph, const xlsxiowriter *handle) {
    const string ageStr = getCleanEntry(ageParagraph);
    int age = convertFirstNum(ageStr);
    if (ageStr == "Age:") {
        xlsxiowrite_add_cell_string(*handle, "");
        return;
    }
    assert(to_string(age).length() <= 3);
    assert(age > 0);

    xlsxiowrite_add_cell_int(*handle, age);
}

void setRhythmHeartRateBloodPressure(
    duckx::Paragraph rhythmHeartRateBloodPressureParagraph,
    const xlsxiowriter *handle) {
    string line = "";
    for (auto r : rhythmHeartRateBloodPressureParagraph.runs()) {
        line.append(r.get_text());
    }
    const string RHYTHM = "Rhythm", HEART_RATE = "Heart Rate",
                 BLOOD_PRESSURE = "Blood Pressure";

    if (line.find(RHYTHM) != string::npos) {
        line = line.substr(line.find(RHYTHM) + RHYTHM.length() + 1);
        const string rhythmStr = line.substr(0, line.find(HEART_RATE) - 1);
        xlsxiowrite_add_cell_string(*handle, rhythmStr.c_str());
    } else {
        xlsxiowrite_add_cell_string(*handle, "");
    }

    if (line.find(HEART_RATE) != string::npos) {
        const string hrStr =
            line.substr(line.find(HEART_RATE) - 1,
                        line.find(BLOOD_PRESSURE) - line.find(HEART_RATE));
        xlsxiowrite_add_cell_int(*handle,
                                 convertFirstNum(removeTextLabel(hrStr)));
    } else {
        xlsxiowrite_add_cell_string(*handle, "");
    }

    if (line.find(BLOOD_PRESSURE) != string::npos) {
        string bpStr = line.substr(line.find(BLOOD_PRESSURE));
        stringstream bpss{removeTextLabel(bpStr)};

        // Remove units
        bpss >> bpStr;
        xlsxiowrite_add_cell_string(*handle, bpStr.c_str());
    } else {
        xlsxiowrite_add_cell_string(*handle, "");
    }
}

string removeTextLabel(string s) {
    for (size_t i = 0; i < s.length(); i++) {
        if (isdigit(s.at(i))) {
            // Find first digit, remove everything before
            s = s.substr(i);
            break;
        }
    }
    return s;
}

string getCleanEntry(duckx::Paragraph paragraph) {
    string value = "";
    for (auto r : paragraph.runs()) {
        value.append(r.get_text());
    }

    return removeTextLabel(value);
}

int convertFirstNum(string s) {
    stringstream ss{s};
    int val;
    ss >> val;
    return val;
}
