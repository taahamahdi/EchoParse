#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const int KEY_VAL_SIZE = 42;
const std::string KEY_VALS[KEY_VAL_SIZE] = {
    "IVSd",
    "LVPWd",
    "LVIDd",
    "LVIDs)",
    "LV FS",
    "LV EF",
    "LV Mass",
    "LV RWT",
    "LV EDV 2D",
    "LV ESV 2D",
    "LV EDV index",
    "LV ESV index",
    "SV LVOT",

    "Prox Asc Aorta",
    "Left Atrium (2D)",

    "RA Area A4C",

    "RVd A4C",
    "TV S'",
    "TAPSE",
    "MV Peak E",
    "MV Peak A",
    "E/A Ratio",
    "LA Pressure",
    "e' lat MV Ann",
    "E/e' Lat Ratio",
    "e' med MV Ann",
    "E/e' Med Ratio",
    "E/e' Averaged",
    "Decel Time",
    "LA Vol A4C A/L",
    "LA Vol A2C A/L",
    "LA Vol BP A/L",
    "LA VOL A/L index",
    "PV S",
    "PV D",
    "MV Max Vel",
    "MV Pk Grad",
    "MV Mean Grad",
    "LVOT Diameter",
    "Dimensionless index VTI",
    "Dimensionless index Vmax",
    "TR Max Velocity",
};

const int QUAL_VAL_SIZE = 13;
const std::string QUALITATIVE_VALS[QUAL_VAL_SIZE] = {
    "Technical Comments",
    "Left Ventricle",
    "Right Ventricle",
    "Aortic Valve",
    "Mitral Valve",
    "Tricuspid Vlave",
    "Pulmonic valve",
    "Left Atrium",
    "Right Atrium",
    "Pericardium",
    "Aorta",
    "Venous",
    "Shunts",
};

#endif // CONSTANTS_H
