#ifndef ECHO_H
#define ECHO_H

#include <string>
#include "libs/duckx/include/duckx.hpp"
#include "libs/xlsxio/include/xlsxio_write.h"
#include "constants.h"

void insertVals(double *values, const xlsxiowriter *handle);
void extractVals(double *values, std::string line);
void getMRN(duckx::Paragraph mrnParagraph, const xlsxiowriter *handle);

#endif // ECHO_H
