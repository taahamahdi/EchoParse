#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <assert.h>

#include "echoparse.h"

#include "libs/xlsxio/include/xlsxio_write.h"
#include "libs/duckx.hpp"

using namespace std;

int main() {
    const string OUTPUT_FILE_NAME = "output/output.xlsx";
    const string INPUT_DOCS_PATH = "input_docs";

    xlsxiowriter *handle = new xlsxiowriter;
    if ((*handle = xlsxiowrite_open(OUTPUT_FILE_NAME.c_str(), "MySheet")) == NULL) {
        cerr << "Error creating .xlsx file" << endl;
        return 1;
    }

    xlsxiowrite_add_column(*handle, "MRN", 7);
    for (const auto &columnName: KEY_VALS) {
        xlsxiowrite_add_column(*handle, columnName.c_str(), columnName.length());
    }
    for (const auto &columnName: KEY_TEXT) {
        xlsxiowrite_add_column(*handle, columnName.c_str(), columnName.length());
    }

    xlsxiowrite_next_row(*handle);

    cout << "Reading from:" << endl;
    for (const auto &file : filesystem::directory_iterator(INPUT_DOCS_PATH)) {
        cout << file.path();

        double *values = new double[KEY_VAL_SIZE];
        fill_n(values, KEY_VAL_SIZE, INT_MIN);

        string *text = new string[KEY_TEXT_SIZE];

        duckx::Document doc(file.path());
        doc.open();

        duckx::Paragraph mrnParagraph;

        bool pastMeasurements = false;
        string line = "";
        for (auto p : doc.paragraphs()) {
            for (auto r : p.runs()) {
                if (r.get_text().find("MRN") != string::npos) {
                    mrnParagraph = p;
                }
                if (r.get_text() == "Measurements" || r.get_text() == "MEASUREMENTS AND CALCULATIONS:") {
                    pastMeasurements = true;
                }
            }
            if (pastMeasurements) {
                line = "";
            } else if (p.runs().get_text().length() > 0) {
                const char startLineChar = p.runs().get_text().at(0);
                // Assumption: A new line begins when the starting character of that line is uppercase.
                // Reasoning: Some lines in the docx file have unexpected paragraph breaks in random parts
                // of a KEY_TEXT entry.
                if (startLineChar >= 'A' && startLineChar <= 'Z') {
                    line = "";
                }
            }
            for (auto r : p.runs()) {
                line.append(r.get_text());
            }
            if (!pastMeasurements) {
                extractText(text, line);
            } else {
                extractVals(values, line);
            }
        }

        getMRN(mrnParagraph, handle);

        insertVals(values, handle);
        insertText(text, handle);
        delete[] values;
        xlsxiowrite_next_row(*handle);
    }
    xlsxiowrite_close(*handle);
}

void insertText(string *text, const xlsxiowriter *handle) {
    for (int i = 0; i < KEY_TEXT_SIZE; i++) {
        xlsxiowrite_add_cell_string(*handle, text[i].c_str());
    }
}

void extractText(string *text, const string &line) {
    for (int i = 0; i < KEY_TEXT_SIZE; i++) {
        // Look only if KEY_TEXT is at start of line
        if (line.find(KEY_TEXT[i]) == 0) {
            // Store the line excluding the keyText name
            // + 2 (colon and space that follow)
            string entry = line.substr(KEY_TEXT[i].length() + 2);
            text[i] = entry;
        }
    }
}

void insertVals(double *values, const xlsxiowriter *handle) {
    for (int i = 0; i < KEY_VAL_SIZE; i++) {
        if (values[i] != INT_MIN) {
            xlsxiowrite_add_cell_float(*handle, values[i]);
        } else {
            xlsxiowrite_add_cell_string(*handle, NULL);
        }
    }
}

void extractVals(double *values, const string &line) {
    for (int i = 0; i < KEY_VAL_SIZE; i++) {
        if (line.find(KEY_VALS[i]) != string::npos) {
            // Store the line excluding the keyVal name
            string entry = line.substr(KEY_VALS[i].length());
            // Remove beginning whitespace
            entry.erase(0, 1);

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

            values[i] = value;
            break;
        }
    }
}

void getMRN(duckx::Paragraph mrnParagraph, const xlsxiowriter *handle) {
    string mrn = "";
    for (auto r : mrnParagraph.runs()) {
        mrn.append(r.get_text());
    }

    // Remove any garbage before MRN (such as name)
    size_t i = 0;
    for (; i < mrn.length(); i++ ) {
        if (isdigit(mrn.at(i))) {
            break;
        }
    }
    // Find first digit, remove everything before
    mrn = mrn.substr(i, mrn.length() - i);
    cout << "\tMRN: " << mrn << endl;
    assert(mrn.length() == 7);

    int id = atoi(mrn.c_str());
    xlsxiowrite_add_cell_float(*handle, id);
}
