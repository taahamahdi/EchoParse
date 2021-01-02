# EchoParse
Parses and extracts data from echocardiography reports (in *docx format) to an Excel spreadsheet.

## Build instructions
* `git submodule update --init --recursive`
* `make all`

## Running EchoParse
* Copy your reports to EchoParse/input_docs
* Set which values you'd like extracted in `constants.h`
* `./EchoParse`
* Open the generated `output.xlsx` file in `output/`

## Expected format of echocardiography reports

...

KEY_TEXT[any index]: [text that will be extracted]

KEY_TEXT[any index]: [text that will be extracted]

KEY_TEXT[any index]: [text that will be extracted]

...

Measurements [or] MEASUREMENTS AND CALCULATIONS:

...

KEY_VAL[any index]: [value that will be extracted]

KEY_VAL[any index]: [value that will be extracted]

KEY_VAL[any index]: [value that will be extracted]

...

The line containing the MRN can appear anywhere.

## Output format of Excel file

| "MRN" | KEY_VAL[0]'s name | KEY_VAL[1]'s name | ... | KEY_TEXT[0]'s name | KEY_TEXT[1]'s name | ... |
| ----- | ----------------- | ---------------- | --- | ------------------ | ------------------ | --- |
| [First patient's MRN] | KEY_VAL[0] | KEY_VAL[1] | ... | KEY_TEXT[0] | KEY_TEXT[1] | ... |
| [Second patient's MRN] | KEY_VAL[0] | KEY_VAL[1] | ... | KEY_TEXT[0] | KEY_TEXT[1] | ... |
| ... | ... | ... | ... | ... | ... | ... |
