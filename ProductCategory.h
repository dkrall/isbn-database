#ifndef PRODUCTCATEGORY_H
#define PRODUCTCATEGORY_H

#include <string>
#include <list>
#include <iterator>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <filesystem>
#include <ctime>
#include "Product.h"

using namespace std;

class ProductCategory
{
  private:
    list<Product> product_list;
    list<string> list_of_json_files;
    list<string> list_of_series_titles;
    void populateListOfJsonFiles();
    void populateProducts(string folderPath, bool isVerbose);
    string folderPath;
    string category_name;
    double max_manual_isbn;
    static bool isTitleFirstInAlphabet(Product& first, Product& second);
    static bool isYN(string prompt);
    string sanitizeString(string string_to_sanitize);
    bool seriesTitleExists(string series_title);
    string getSeriesTitleFromUser();
    void printAllSeries();

  public:
    ProductCategory();
    ProductCategory(string folderName, bool isVerbose);
    Product getProductByIsbn(double isbn);
    void writeToCsv();
    void addEntry();
    void printSearchResults(list<Product> search_results, char mode);
    void searchProducts(string search_string);
    static bool isStringFirstInAlphabet(string& first, string& second);
};

#endif
