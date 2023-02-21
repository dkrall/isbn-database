#ifndef PRODUCT_H
#define PRODUCT_H

#include <algorithm>
#include <string>
#include <iostream>
#include <list>
#include <regex>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Product
{
  private:
    double isbn;
    string title;
    string category;
    string description;
    string image_urls;
    string size;

    string sanitizeString(string string_to_sanitize);

  public:
    Product();
    Product(string json_string);

    void print_full_json();
    double getIsbn();
    string getTitle();
    string getCsv();
    string toString();
    bool isStringInTitleOrDesc(string search_string);
    string doubleToString(double double_to_convert);
};

#endif
