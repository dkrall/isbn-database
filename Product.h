#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Product
{
  private:
    double isbn;
    json full_json;

    string doubleToString(double double_to_convert);

  public:
    Product();
    Product(string json_string);

    void print_full_json();
    double getIsbn();
    string toString();
};

#endif
