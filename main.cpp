#include <iostream>
#include <fstream>
#include <iterator>
#include <list>
#include <string>
#include <filesystem>
#include <dirent.h>
#include <sys/types.h>
#include <map>
#include "json.hpp"
#include "Product.h"
#include "ProductCategory.h"
using namespace std;
using json = nlohmann::json;

// Display the main menu options by printing them line-by-line to the screen.
void showMainMenu() {
  cout << endl << "Welcome to the ISBN database. JSONs have been loaded from the jsons directory." << endl;
  cout << "Please select one of the following options by typing the corresponding character:" << endl;
  cout << "v: view details for an entry in the terminal" << endl;
  cout << "c: write all data to csv" << endl;
  cout << "a: add a new entry" << endl;
  cout << "d: select a different dataset" << endl;
  cout << "e: exit program" << endl;
  cout << "Please enter selection: ";
}


void viewEntry(ProductCategory product_list) {
  string isbn_string;
  double isbn;
  Product product;
  char choice;

  cout << "How do you want to search for a product?" << endl;
  cout << "i: enter full isbn." << endl;
  cout << "e: exit submenu and return to main menu." << endl;
  cout << "Please enter selection: ";

  cin >> choice;
  switch(choice) {
    case 'i':
      cout << "Please enter full isbn: ";
      cin >> isbn_string;
      isbn = stod(isbn_string);
      product = product_list.getProductByIsbn(isbn);
      cout << product.toString();
      break;
    case 'e':
      cout << "Returning to main menu." << endl;
      break;
    default:
      cout << "Unable to interpret. Returning to main menu." << endl;
  }
}

void showCategoryMenu(list<string> json_folder_names) {
  list<string>::iterator it;
  cout << "The following datasets have been loaded into the ISBN Database:" << endl;

  for(it = json_folder_names.begin(); it != json_folder_names.end(); it++){
    cout << (*it) << endl;
  }

  cout << "Type the name of the dataset you'd like to access: ";
}

ProductCategory getProductCategoryByName(list<string> json_folder_names, map<string, ProductCategory> categoryMap) {
  bool categoryFound;
  string selectedCategoryName;
  ProductCategory selectedDataset;

  showCategoryMenu(json_folder_names);
  cin >> selectedCategoryName;
  categoryFound = find(json_folder_names.begin(), json_folder_names.end(), selectedCategoryName) != json_folder_names.end();

  while (!categoryFound) {
    cout << "Dataset not found. Please reenter: ";
    cin >> selectedCategoryName;
    categoryFound = find(json_folder_names.begin(), json_folder_names.end(), selectedCategoryName) != json_folder_names.end();
  }

  selectedDataset = categoryMap[selectedCategoryName];
  cout << "Loaded " << selectedCategoryName << " dataset." << endl;
  return(selectedDataset);
}

bool isFlagInArgs(int argc, const char* argv[], string flag) {
  for (int x = 0; x < argc; x++) {
    if (string(argv[x]) == flag) {
      return true;
    }
  }
  return false;
}

int main(int argc, const char* argv[]) {
  char choice;
  list<string> json_folder_names;
  list<ProductCategory> productCategoryList;
  const char* json_parent_dir = "./jsons/";
  DIR *dir = opendir(json_parent_dir);
  map<string, ProductCategory> categoryMap;
  ProductCategory selectedDataset;
  bool categoryFound;
  bool isVerbose = isFlagInArgs(argc, argv, "-v") || isFlagInArgs(argc, argv, "-verbose");

  struct dirent *entry = readdir(dir);

  while (entry != NULL) {
    if (entry->d_type == DT_DIR && (entry->d_name)[0] != '.') {
      string folder_string(entry->d_name);
      json_folder_names.push_back(folder_string);
    }

    entry = readdir(dir);
  }

  closedir(dir);

  list<string>::iterator it;

  for(it = json_folder_names.begin(); it != json_folder_names.end(); it++){
    categoryMap[*it] = ProductCategory(*it, isVerbose);
  }

  selectedDataset = getProductCategoryByName(json_folder_names, categoryMap);

  // TODO: This menu loop will eventually have more options. The text-based menu may eventually be replaced with a GUI.
  while (choice != 'e') {
    showMainMenu();
    cin >> choice;
    switch(choice) {
      case 'v':
        viewEntry(selectedDataset);
        break;
      case 'c':
        selectedDataset.writeToCsv();
        break;
      case 'a':
        selectedDataset.addEntry();
        break;
      case 'd':
        selectedDataset = getProductCategoryByName(json_folder_names, categoryMap);
        break;
      case 'e':
        break;
      default:
        cout << "Unable to interpret. Please try again." << endl;
    }
  }
  cout << "Exiting program." << endl;
  return 0;
}
