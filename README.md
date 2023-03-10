# isbn-database
A C++ program to interact with json files generated by https://github.com/dkrall/manga-from-isbn. This should allow for viewing entries, creation of new entries, editing entries, and exporting entries.

## File Structure

### completed_isbns
Contains a .txt file for each dataset listing the isbns that have been processed. Every ISBN in one of these files has either been successfully used to generate a JSON which is now present in the jsons folder or has failed to process and is listed in the errored_isbns folder.

### errored_isbns
Contains a .txt file for each dataset listing the isbns that were not successfully processed. For each isbn listed, there is a JSON file that failed to generate.

### jsons
Contains one subfolder for each dataset. Within that subfolder, one .json file exists per ISBN in the database. These are read in at the start of the program's execution.

### output
When exporting to CSV, the output CSV files are written here.

### Sample Folder structure:
- completed_isbns
  - animeDiscs.txt
  - gamesSwitch.txt
  - manga.txt
- errored_isbns
  - animeDiscs.txt
  - gamesSwitch.txt
  - manga.txt
- jsons
  - animeDiscs
    - animeDiscs_000000000001.json
    - animeDiscs_000000000002.json
  - gamesSwitch
    - gamesSwitch_000000000001.json
    - gamesSwitch_000000000002.json
  - manga
    - manga_000000000001.json
    - manga_000000000002.json
- output
- json.hpp (downloaded from https://json.nlohmann.me/releases)

## Usage

### Makefile
To compile this project, execute the following instruction from the command line:
`make`

### Execution
After the program has been successfully compiled, execute the following to run the program:
`./main`

### Flags
This program supports a "-v" or "-verbose" flag for use in debugging. When running in verbose mode, execute using the following command:
`./main -v`
or
`./main -verbose`
