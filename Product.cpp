#include "Product.h"

Product::Product()
{

}

Product::Product(string json_string)
{
  // TODO: Parse the JSON string and assign to fields
  json json_object = json::parse(json_string);
  full_json = json_object;

  string isbn_string = full_json["products"][0]["barcode_number"].get<std::string>();
  isbn = std::stod(isbn_string);
}

double Product::getIsbn() {
  return isbn;
}

string Product::doubleToString(double double_to_convert) {
  string doubleAsString = to_string(double_to_convert);
  return doubleAsString.substr(0, doubleAsString.find('.'));
}

string Product::toString() {
  string object_contents;

  object_contents = "isbn: " + doubleToString(isbn) + "\n";

  return object_contents;
}
