#ifndef ECHO_H
#define ECHO_H

#include <string>
#include "libs/duckx/include/duckx.hpp"
#include "libs/xlsxio/include/xlsxio_write.h"
#include "constants.h"

void insertText(std::string *text, const xlsxiowriter *handle);
void extractText(std::string *text, const std::string &line);
void insertVals(double *values, const xlsxiowriter *handle);
void extractVals(double *values, const std::string &line);
void getMRN(duckx::Paragraph mrnParagraph, const xlsxiowriter *handle);

#endif // ECHO_H
