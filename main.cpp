#include <iostream>
#include <fstream>
#include <iterator>
#include <list>
#include <string>
#include <filesystem>
#include <dirent.h>
#include <sys/types.h>
#include "json.hpp"
#include "Product.h"
using namespace std;
using json = nlohmann::json;

// Given a list of products and an isbn number, return the product from that list.
Product getProductByIsbn(list<Product> product_list, double isbn){
  list<Product>::iterator it;
  Product product_with_correct_isbn;

  for(it = product_list.begin(); it != product_list.end(); it++){
    cout << "current: " << (*it).getIsbn() << endl;
    bool is_isbn = (*it).getIsbn() == isbn;
    if(is_isbn) {
      product_with_correct_isbn = (*it);
    }
  }

  return product_with_correct_isbn;
}

// Get the list of json files in the jsons directory and return a list of strings containing those filenames.
list<string> getListOfJsonFiles(){
  list<string>list_of_json_files;
  DIR *dir;
  struct dirent *file;
  dir = opendir("./jsons");
  if (dir) {
    while ((file = readdir(dir)) != NULL) {
      string file_string(file->d_name);

      if ((file_string).find(".json") != string::npos) {
        list_of_json_files.push_back(file_string);
      }
    }
    closedir(dir);
  }

  return list_of_json_files;
}

// Display the main menu options by printing them line-by-line to the screen.
void showMainMenu() {
  cout << endl << "Welcome to the ISBN database. JSONs have been loaded from the jsons directory." << endl;
  cout << "Please select one of the following options by typing the corresponding character:" << endl;
  cout << "v: view details for an entry in the terminal" << endl;
  cout << "e: exit program" << endl;
  cout << "Please enter selection: ";
}


void viewEntry(list<Product> product_list) {
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
      product = getProductByIsbn(product_list, isbn);
      cout << product.toString();
      break;
    case 'e':
      cout << "Returning to main menu." << endl;
      break;
    default:
      cout << "Unable to interpret. Returning to main menu." << endl;
  }
}


int main() {
  char choice;
  list<Product>product_list;
  list<string>::iterator string_iterator;
  list<string> list_of_json_files = getListOfJsonFiles();

  for(string_iterator = list_of_json_files.begin(); string_iterator != list_of_json_files.end(); string_iterator++){
    string line;
    ifstream file("jsons/" + (*string_iterator));
    if(file.is_open()) {
      // Expect only one line to be returned.
      getline(file, line);
      Product product_from_json = Product(line);
      product_list.push_back(product_from_json);
      cout << "Loaded " << (*string_iterator) << " successfully." << endl;
      file.close();
    } else {
      cout << "Failed to open file. " << *string_iterator << endl;
    }
  }

  // TODO: This menu loop will eventually have more options. The text-based menu may eventually be replaced with a GUI.
  while (choice != 'e') {
    showMainMenu();
    cin >> choice;
    switch(choice) {
      case 'v':
        viewEntry(product_list);
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
