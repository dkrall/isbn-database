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
    string series_name;
    int volume_number;
    string format;
    // TODO: Series should prompt when
    // adding a volume to a new series so duplicate series are not created ("This series does not exist. Are you sure you want to create a new one?")

    string sanitizeString(string string_to_sanitize);

  public:
    Product();
    Product(string json_string);

    void print_full_json();
    double getIsbn();
    string getTitle();
    string getSeriesTitle();
    string getCsv();
    string toString();
    bool isStringInTitleOrDesc(string search_string);
    string doubleToString(double double_to_convert);
};

#endif
