#include "ProductCategory.h"
#include <iostream>
#include <cctype>
using namespace std;

ProductCategory::ProductCategory()
{

}

ProductCategory::ProductCategory(string folderName, bool isVerbose)
{
  folderPath = "./jsons/" + folderName;
  category_name = folderName;
  populateListOfJsonFiles();
  populateProducts(folderPath + "/", isVerbose);
}

// Get the list of json files in the jsons directory and return a list of strings containing those filenames.
void ProductCategory::populateListOfJsonFiles()
{
  list<string>json_filenames;
  DIR *dir;
  struct dirent *file;

  dir = opendir(folderPath.c_str());
  if (dir) {
    while ((file = readdir(dir)) != NULL) {
      string file_string(file->d_name);

      if ((file_string).find(".json") != string::npos) {
        json_filenames.push_back(file_string);
      }
    }
    closedir(dir);
  }

  list_of_json_files = json_filenames;
}

void ProductCategory::populateProducts(string folderPath, bool isVerbose)
{
  list<string>::iterator string_iterator;
  max_manual_isbn = 0;
  double isbn;
  string series_title;

  for(string_iterator = list_of_json_files.begin(); string_iterator != list_of_json_files.end(); string_iterator++){
    string line;
    ifstream file(folderPath + (*string_iterator));

    if (isVerbose) {
      cout << folderPath + (*string_iterator) << endl;
    }

    if(file.is_open()) {
      // Expect only one line to be returned.
      getline(file, line);
      Product product_from_json = Product(line);
      product_list.push_back(product_from_json);

      isbn = product_from_json.getIsbn();
      series_title = product_from_json.getSeriesTitle();

      if (!seriesTitleExists(series_title)) {
        list_of_series_titles.push_back(series_title);
      }

      if (isbn < 10000 && isbn > max_manual_isbn) {
        max_manual_isbn = isbn;
      }

      if (isVerbose) {
        cout << "Loaded " << folderPath + (*string_iterator) << " successfully." << endl;
      }

      file.close();
    } else {
      cout << "Failed to open file. " << folderPath + (*string_iterator) << endl;
    }
  }

  product_list.sort(&ProductCategory::isTitleFirstInAlphabet);
  list_of_series_titles.sort(&ProductCategory::isStringFirstInAlphabet);
}

bool ProductCategory::seriesTitleExists(string series_title) {
  return(find(list_of_series_titles.begin(), list_of_series_titles.end(), series_title) != list_of_series_titles.end());
}

// Given an isbn number, return the product from that list.
Product ProductCategory::getProductByIsbn(double isbn){
  list<Product>::iterator it;
  Product product_with_correct_isbn;

  for(it = product_list.begin(); it != product_list.end(); it++){
    bool is_isbn = (*it).getIsbn() == isbn;
    if(is_isbn) {
      product_with_correct_isbn = (*it);
    }
  }

  return product_with_correct_isbn;
}

// Helper function passed to sort list of products.
bool ProductCategory::isTitleFirstInAlphabet(Product& first, Product& second) {
  return first.getTitle().compare(second.getTitle()) < 0;
}

// Helper function passed to sort list of arbitrary strings.
bool ProductCategory::isStringFirstInAlphabet(string& first, string& second) {
  return first.compare(second) < 0;
}

// Helper function to interpret y/n inputs.
bool ProductCategory::isYN(string prompt) {
  char yn = ' ';

  while(yn != 'y' && yn != 'n') {
    cout << prompt;
    cin >> yn;
  }

  return(yn == 'y');
}

// TODO: Add menu with options, including options to print out manga in the order they appear on my shelves or in alphabetical order.
void ProductCategory::writeToCsv() {
  bool is_use_default_filename;
  int day;
  int month;
  int year;
  ofstream output_file;
  string csvHeaders = "Title,ISBN,Series,Volume Number,Format,Image URLs,Description,Size,Category\n";
  string filename;
  time_t now;
  tm *ltm;

  is_use_default_filename = ProductCategory::isYN("Use default filename (y/n)? ");

  if (is_use_default_filename) {
    now = time(0);
    ltm = localtime(&now);
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    day = ltm->tm_mday;

    // Base filename is category name
    filename = category_name + "_";

    // Add month to filename
    if (month < 10) {
      filename = filename + "0";
    }
    filename = filename + to_string(month) + "_";

    // Add day, year, and extension to filename
    if (day < 10) {
      filename = filename + "0";
    }
    filename = filename + to_string(day) + "_" + to_string(year) + ".csv";

    cout << "Using default filename " + filename + "." << endl;
  } else {
    cout << "Please enter filename: ";
    cin >> filename;
  }
  filename = "output/" + filename;
  output_file.open(filename);
  output_file << csvHeaders;

  list<Product>::iterator it;

  for(it = product_list.begin(); it != product_list.end(); it++){
    output_file << (*it).getCsv();
  }

  output_file.close();
  cout << "Successfully created " << filename << endl;
}

void ProductCategory::addEntry() {
  Product product_from_json;
  string barcode_number;
  double barcode_number_converted;
  string barcode_formats;
  string title;
  string manufacturer;
  string image;
  string volume_number;
  string description;
  char choice;
  list<Product> product_list_temp = product_list;
  Product product_for_isbn;
  bool is_auto_generate_isbn;
  string json_string = "{\"products\":[{\"barcode_number\":\"";
  getline(cin, barcode_number);// Get current line so it does not interrupt later operations. barcode_number will be overwritten.

  cout << "Input ISBN Number: ";
  getline(cin, barcode_number);

  if (barcode_number.empty()) {
    is_auto_generate_isbn = ProductCategory::isYN("Use autogenerated ISBN (y/n)? ");
  }

  if (is_auto_generate_isbn) {
    max_manual_isbn = max_manual_isbn + 1;
    barcode_number = to_string(int(max_manual_isbn));
    while (barcode_number.length() < 12) {
      barcode_number = "0" + barcode_number;
    }
    cout << "Created automatic ISBN " + barcode_number << endl;
    getline(cin, barcode_formats); // Keep script from skipping ahead.
  } else {
    barcode_number_converted = stod(barcode_number);
    product_for_isbn = ProductCategory::getProductByIsbn(barcode_number_converted);
    while (product_for_isbn.getIsbn() == barcode_number_converted) {
      cout << "This ISBN already exists! Please try another: ";
      getline(cin, barcode_number);
      barcode_number_converted = stod(barcode_number);
      product_for_isbn = ProductCategory::getProductByIsbn(barcode_number_converted);
    }
  }

  json_string = json_string + barcode_number + "\",\"barcode_formats\":\"";

  cout << "Input Barcode Formats: ";
  getline(cin, barcode_formats);
  json_string = json_string + barcode_formats + "\",\"mpn\":\"\",\"model\":\"\",\"asin\":\"\",\"title\":\"";

  cout << "Input Title: ";
  getline(cin, title);
  json_string = json_string + title + "\",\"category\":\"\",\"manufacturer\":\"";

  cout << "Input Manufacturer: ";
  getline(cin, manufacturer);

  if (category_name == "animeDiscs") {
    json_string = json_string + manufacturer + "\",\"format\":\"";
    cout << "Input Format (d: DVD, b: Blu-Ray, anything else: neither):";
    cin >> choice;
    switch(choice) {
      case 'd':
        cout << "Setting format as 'dvd'." << endl;
        json_string = json_string + "dvd";
        break;
      case 'b':
        cout << "Setting format as 'bluray'." << endl;
        json_string = json_string + "bluray";
        break;
      default:
        cout << "Setting format as 'Neither'." << endl;
    }

    getline(cin, volume_number); // Keep script from skipping ahead.
  }

  json_string = json_string + "\",\"series_name\":\"";
  json_string = json_string + getSeriesTitleFromUser() + "\",\"volume_number\":\"";

  cout << "Input Volume Number: ";
  getline(cin, volume_number);

  if (volume_number.length() < 1) {
    volume_number = "-1";
  }

  json_string = json_string + volume_number + "\",\"brand\":\"\",\"contributors\":[],\"age_group\":\"\",\"ingredients\":\"\"";
  json_string = json_string + ",\"nutrition_facts\":\"\",\"energy_efficiency_class\":\"\",\"color\":\"\",\"gender\":\"\"";
  json_string = json_string + ",\"material\":\"\",\"pattern\":\"\",\"multipack\":\"\",\"size\":\"\",\"length\":\"\"";
  json_string = json_string + ",\"width\":\"\",\"height\":\"\",\"weight\":\"\",\"release_date\":\"\",\"features\":[],\"images\":[\"";

  cout << "Input Image URL: ";
  getline(cin, image);
  json_string = json_string + image + "\"],\"description\":\"";

  cout << "Input Description: ";
  getline(cin, description);
  json_string = json_string + sanitizeString(description) + "\", \"last_update\":\"\",\"stores\":[]}]}";

  // Add product to list
  product_from_json = Product(json_string);
  product_list_temp.push_back(product_from_json);
  product_list = product_list_temp;

  // Create file
  ofstream file;
  file.open(folderPath + "/" + category_name + "_" + barcode_number + ".json");
  file << json_string;
  file.close();
}

string ProductCategory::getSeriesTitleFromUser() {
  string series;
  string temp;
  string json_string = "";
  char choice = ' ';
  bool is_title_confirmed = false;
  cout << "Input Series: ";
  getline(cin, series);

  if (!seriesTitleExists(series)) {
    while (!is_title_confirmed) {
      cout << "This series name does not exist yet. Do you want to see all existing series (e), enter a different series name (n), or keep the series name you entered (k)? ";
      cin >> choice;
      getline(cin, temp); // Prevent program from skipping inputs.

      switch(choice) {
        case 'e':
          printAllSeries();
          break;
        case 'n':
          cout << "Input Series: ";
          getline(cin, series);
          is_title_confirmed = seriesTitleExists(series);
          break;
        case 'k':
          is_title_confirmed = true;
          list_of_series_titles.push_back(series);
          break;
        default:
          cout << "Entry not recognized. Please try again." << endl;
      }
    }
  }

  for (int i = 0; i < series.length(); i++) {
    json_string = json_string + string(1, tolower(series[i]));
  }
  return(json_string);
}

string ProductCategory::sanitizeString(string string_to_sanitize) {
  string return_string = regex_replace(string_to_sanitize, regex("\""), "\"\"");
  return regex_replace(return_string, regex(":"), ";");
}

void ProductCategory::printAllSeries() {
  list<string>::iterator it;

  for(it = list_of_series_titles.begin(); it != list_of_series_titles.end(); it++) {
    cout << (*it) << endl;
  }
  cout << endl;
}

void ProductCategory::printSearchResults(list<Product> search_results, char mode) {
  list<Product>::iterator it;
  int counter = 1;
  cout << "------------------------------------" << endl;
  cout << "---------- Search results ----------" << endl;
  cout << "------------------------------------" << endl;

  for(it = search_results.begin(); it != search_results.end(); it++){
    switch(mode) {
      case 'i':
        cout << (*it).doubleToString((*it).getIsbn()) << endl;
        break;
      case 't':
        cout << (*it).getTitle() << endl;
        break;
      case 'a':
        cout << "----- Entry " << counter << " -----" << endl;
        cout << (*it).toString() << endl;
        break;
      default:
        cout << "Unable to interpret. No results can be displayed" << endl;
    }
    counter++;
  }

  cout << "------------------------------------" << endl;
  cout << "----------                ----------" << endl;
  cout << "------------------------------------" << endl;
}

void ProductCategory::searchProducts(string search_string) {
  list<Product> search_results;
  list<Product>::iterator it;
  char choice;
  string valid_choices = "ita";

  for(it = product_list.begin(); it != product_list.end(); it++){
    bool is_string_present_in_title_or_description = (*it).isStringInTitleOrDesc(search_string);
    if(is_string_present_in_title_or_description) {
      search_results.push_back(*it);
    }
  }

  cout << endl << "Found " << search_results.size() << " results." << endl;
  if (search_results.size() > 0) {
    cout << "How would you like to view these results?" << endl;
    cout << "i: View ISBN list only." << endl;
    cout << "t: View title only." << endl;
    cout << "a: View all data for each entry." << endl;
    cout << "e: exit submenu and return to main menu." << endl;
    cout << "Please enter selection: ";

    cin >> choice;
    if (valid_choices.find(choice) != string::npos) {
      printSearchResults(search_results, choice);
    }
  }
  cout << "Returning to main menu." << endl;
}
