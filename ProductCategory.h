#ifndef PRODUCTCATEGORY_H
#define PRODUCTCATEGORY_H

#include <string>
#include <list>
#include <iterator>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <filesystem>
#include "Product.h"

using namespace std;

class ProductCategory
{
  private:
    list<Product> product_list;
    list<string> list_of_json_files;
    void populateListOfJsonFiles();
    void populateProducts(string folderPath, bool isVerbose);
    string folderPath;
    string category_name;

  public:
    ProductCategory();
    ProductCategory(string folderName, bool isVerbose);
    Product getProductByIsbn(double isbn);
    void writeToCsv();
    void addEntry();
    void printSearchResults(list<Product> search_results, char mode);
    void searchProducts(string search_string);
};

#endif
