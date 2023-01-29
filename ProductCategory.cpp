#include "ProductCategory.h"

ProductCategory::ProductCategory()
{

}

ProductCategory::ProductCategory(string folderName)
{
  string folderPath = "./jsons/" + folderName;
  populateListOfJsonFiles(folderPath);
  populateProducts(folderPath + "/");
}

// Get the list of json files in the jsons directory and return a list of strings containing those filenames.
void ProductCategory::populateListOfJsonFiles(string folderPath)
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

void ProductCategory::populateProducts(string folderPath)
{
  list<Product> product_list_from_files;
  list<string>::iterator string_iterator;

  for(string_iterator = list_of_json_files.begin(); string_iterator != list_of_json_files.end(); string_iterator++){
    string line;
    ifstream file(folderPath + (*string_iterator));
    cout << folderPath + (*string_iterator) << endl;//TODO: Remove
    if(file.is_open()) {
      // Expect only one line to be returned.
      getline(file, line);
      Product product_from_json = Product(line);
      product_list_from_files.push_back(product_from_json);
      cout << "Loaded " << folderPath + (*string_iterator) << " successfully." << endl;
      file.close();
    } else {
      cout << "Failed to open file. " << folderPath + (*string_iterator) << endl;
    }
  }
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

// TODO: Add menu with options, including options to print out manga in the order they appear on my shelves or in alphabetical order.
void ProductCategory::writeToCsv() {
  ofstream output_file;
  string filename;
  string csvHeaders = "Title,ISBN,Description,Image URLs,Size,Category\n";

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
