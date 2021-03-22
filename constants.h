#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const std::string OUTPUT_FILE_NAME = "output/output.xlsx";
const std::string INPUT_DOCS_PATH = "input_docs";

const int KEY_VAL_SIZE = 55;
const std::string KEY_VALS[KEY_VAL_SIZE] = {
    "IVSd",
    "LVPWd",
    "LVIDd",
    "LVIDs)",
    "LVIDd index",
    "LV FS",
    "LV EF",
    "LV Mass",
    "LV Mass indexed",
    "LV RWT",
    "LV EDV 2D",
    "LV ESV 2D",
    "LV EDV index",
    "LV ESV index",
    "SV LVOT",
    "Aortic Sinuses",
    "Aortic Sinuses index",
    "Prox Asc Aorta", // Hard-coded "ProxAsc Aorta" to this, appears as such in some files
    // TODO(taahamahdi): Find generalized solution
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
    "LA Vol A4C A/L",
    "LA Vol A2C A/L",
    "LA Vol BP A/L",
    "LA VOL A/L index",
    "PV S",
    "PV D",
    "PV A",
    "MV Max Vel",
    "MV Pk Grad",
    "MV Mean Grad",
    "MV Area, PHT",
    "AoV Vmax",
    "LVOT Vmax",
    "AoV Area, Vmax",
    "AoV Peak PG",
    "LVOT VTI",
    "AoV VTI",
    "AoV Area, VTI",
    "AoV Mean PG",
    "LVOT Diameter",
    "Dimensionless index VTI",
    "Dimensionless index Vmax",
    "TR Max Velocity",
};

const int KEY_TEXT_SIZE = 12;
const std::string KEY_TEXT[KEY_TEXT_SIZE] = {
    "Left Ventricle",
    "Right Ventricle",
    "Aortic Valve",
    "Mitral Valve",
    "Tricuspid Valve",
    "Pulmonic Valve",
    "Left Atrium",
    "Right Atrium",
    "Pericardium",
    "Aorta",
    "Venous",
    "Shunts",
};

#endif // CONSTANTS_H
