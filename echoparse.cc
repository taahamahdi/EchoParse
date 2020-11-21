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
#include "libs/duckx/include/duckx.hpp"

using namespace std;

int main() {
    const string OUTPUT_FILE_NAME = "output/output.xlsx";
    const string INPUT_DOCS_PATH = "input_docs";

    xlsxiowriter handle;
    if ((handle = xlsxiowrite_open(OUTPUT_FILE_NAME.c_str(), "MySheet")) == NULL) {
        cerr << "Error creating .xlsx file" << endl;
        return 1;
    }

    xlsxiowrite_add_column(handle, "MRN", 7);
    for (const auto &columnName: keyVals) {
        xlsxiowrite_add_column(handle, columnName.c_str(), columnName.length());
    }

    xlsxiowrite_next_row(handle);

    for (const auto &file : filesystem::directory_iterator(INPUT_DOCS_PATH)) {
        cout << endl << file.path() << endl;

        double values[KEY_VAL_SIZE] = {};
        fill_n(values, KEY_VAL_SIZE, INT_MIN);

        duckx::Document doc(file.path());
        doc.open();

        duckx::Paragraph mrnParagraph;

        bool pastMeasurements = false;
        for (auto p : doc.paragraphs()) {
            for (auto r : p.runs()) {
                if (r.get_text().find("MRN") != string::npos) {
                    mrnParagraph = p;
                }
                if (r.get_text() == "Measurements") {
                    pastMeasurements = true;
                }
            }
            string line = "";
            for (auto r : p.runs()) {
                line.append(r.get_text());
            }
            if (pastMeasurements) {
                for (int i = 0; i < KEY_VAL_SIZE; i++) {
                    if (line.find(keyVals[i]) != string::npos) {
                        // Store the line excluding the keyVal name
                        string entry = line.substr(keyVals[i].length());
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
        }

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
        assert(mrn.length() == 7);

        int id = atoi(mrn.c_str());
        xlsxiowrite_add_cell_float(handle, id);

        for (const auto &val: values) {
            if (val != INT_MIN) {
                cout << val << endl;
                xlsxiowrite_add_cell_float(handle, val);
            }
            else {
                xlsxiowrite_add_cell_string(handle, NULL);
            }
        }
        xlsxiowrite_next_row(handle);
    }
    xlsxiowrite_close(handle);
}
