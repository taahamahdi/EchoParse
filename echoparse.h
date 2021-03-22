#ifndef ECHO_H
#define ECHO_H

#include "FilePair.h"
#include "constants.h"
#include "libs/duckx.hpp"
#include "libs/xlsxio/include/xlsxio_write.h"
#include <array>
#include <filesystem>
#include <map>
#include <string>

void addLabels(const xlsxiowriter *handle);
void generateFileMap(std::map<int, FilePair> &files);
void processFile(xlsxiowriter *handle, std::filesystem::path file,
                 FilePairType pairType);

void insertText(std::array<std::string, KEY_TEXT_SIZE> &text,
                const xlsxiowriter *handle);
void extractText(std::array<std::string, KEY_TEXT_SIZE> &text,
                 const std::string &line);
void insertVals(std::array<double, KEY_VAL_SIZE> &values,
                const xlsxiowriter *handle);
bool extractVals(std::array<double, KEY_VAL_SIZE> &values,
                 const std::string &line);
double cleanAndConvertVal(std::string entry);

void setMRN(const duckx::Paragraph &mrnParagraph, const xlsxiowriter *handle);
void setTestDate(duckx::Paragraph testDateParagraph,
                 const xlsxiowriter *handle);
void setAge(const duckx::Paragraph &ageParagraph, const xlsxiowriter *handle);
void setRhythmHeartRateBloodPressure(
    duckx::Paragraph rhythmHeartRateBloodPressureParagraph,
    const xlsxiowriter *handle);
std::string removeTextLabel(std::string s);
std::string getCleanEntry(duckx::Paragraph paragraph);
int convertFirstNum(std::string s);

#endif
