#include "ProductCategory.h"

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
  list<Product> product_list_from_files;
  list<string>::iterator string_iterator;

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
      product_list_from_files.push_back(product_from_json);

      if (isVerbose) {
        cout << "Loaded " << folderPath + (*string_iterator) << " successfully." << endl;
      }

      file.close();
    } else {
      cout << "Failed to open file. " << folderPath + (*string_iterator) << endl;
    }
  }
  product_list_from_files.sort(&ProductCategory::isTitleFirstInAlphabet);

  product_list = product_list_from_files;
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

// TODO: Add menu with options, including options to print out manga in the order they appear on my shelves or in alphabetical order.
void ProductCategory::writeToCsv() {
  ofstream output_file;
  string filename;
  string csvHeaders = "Title,ISBN,Image URLs,Description,Size,Category\n";

  cout << "Please enter filename: ";
  cin >> filename;
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
  list<Product> product_list_temp = product_list;
  Product product_for_isbn;
  string json_string = "{\"products\":[{\"barcode_number\":\"";
  getline(cin, barcode_number);// Get current line so it does not interrupt later operations. barcode_number will be overwritten.

  cout << "Input ISBN Number: ";
  getline(cin, barcode_number);

  barcode_number_converted = stod(barcode_number);
  product_for_isbn = ProductCategory::getProductByIsbn(barcode_number_converted);
  while (product_for_isbn.getIsbn() == barcode_number_converted) {
    cout << "This ISBN already exists! Please try another: ";
    // TODO: Add logic to print the highest manually-added ISBN in the database. Possibly add ISBN auto-generating as a default when no ISBN is added.
    getline(cin, barcode_number);
    barcode_number_converted = stod(barcode_number);
    product_for_isbn = ProductCategory::getProductByIsbn(barcode_number_converted);
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
  json_string = json_string + manufacturer + "\",\"brand\":\"\",\"contributors\":[],\"age_group\":\"\",\"ingredients\":\"\"";
  json_string = json_string + ",\"nutrition_facts\":\"\",\"energy_efficiency_class\":\"\",\"color\":\"\",\"gender\":\"\"";
  json_string = json_string + ",\"material\":\"\",\"pattern\":\"\",\"format\":\"\",\"multipack\":\"\",\"size\":\"\",\"length\":\"\"";
  json_string = json_string + ",\"width\":\"\",\"height\":\"\",\"weight\":\"\",\"release_date\":\"\",\"description\":\"\",\"features\":[],\"images\":[\"";

  cout << "Input Image URL: ";
  getline(cin, image);
  json_string = json_string + image + "\"],\"last_update\":\"\",\"stores\":[]}]}";

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
