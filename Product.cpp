#include "Product.h"

Product::Product()
{

}

Product::Product(string json_string)
{
  // TODO: Verify in longer JSON whether any potential useful optional fields are missing here.
  // If any fields are added, add them to to_string as well.
  json full_json = json::parse(json_string);
  json product = full_json["products"][0];

  string isbn_string = product["barcode_number"].get<string>();
  isbn = stod(isbn_string);


  title = product["title"].get<string>();
  category = product["category"].get<string>();
  description = product["description"].get<string>();

  size = product["size"].get<string>();
  if (product["images"].size() > 0) {
    image_urls = product["images"][0].get<string>();
  }

  series_name = "";
  if (to_string(product["series_name"]) != "null") {
    series_name = product["series_name"].get<string>();
  }

  volume_number = -1;
  if (to_string(product["volume_number"]) != "null" && stoi(product["volume_number"].get<string>()) > -1) {
    volume_number = stoi(product["volume_number"].get<string>());
  }

  format = "";
  if (to_string(product["format"]) != "null") {
    format = product["format"].get<string>();
  }
}

double Product::getIsbn() {
  return isbn;
}

string Product::getTitle() {
  return title;
}

string Product::getSeriesTitle() {
  return series_name;
}

string Product::doubleToString(double double_to_convert) {
  string doubleAsString = to_string(double_to_convert);
  return doubleAsString.substr(0, doubleAsString.find('.'));
}

string Product::sanitizeString(string string_to_sanitize) {
  return "\"" + regex_replace(string_to_sanitize, std::regex("\""), "\"\"") + "\"";
}

string Product::getCsv() {
  string volume_number_string = "";

  if (volume_number > -1) {
    volume_number_string = to_string(volume_number);
  }

  string csv = sanitizeString(title) + ",";
  csv = csv + doubleToString(isbn) + ",";
  csv = csv + sanitizeString(series_name) + ",";
  csv = csv + sanitizeString(volume_number_string) + ",";
  csv = csv + sanitizeString(format) + ",";
  csv = csv + sanitizeString(image_urls) + ",";
  csv = csv + sanitizeString(description) + ",";
  csv = csv + sanitizeString(size) + ",";
  csv = csv + sanitizeString(category) + "\n";
  return csv;
}

// TODO: Maybe add color to this text
string Product::toString() {
  string object_contents;
  string volume_number_string = "";

  if (volume_number > -1) {
    volume_number_string = to_string(volume_number);
  }

  object_contents = "\nisbn: " + doubleToString(isbn) + "\n";
  object_contents = object_contents + "title: " + title + "\n";
  object_contents = object_contents + "category: " + category + "\n";
  object_contents = object_contents + "series_name: " + series_name + "\n";
  object_contents = object_contents + "volume_number: " + volume_number_string + "\n";
  object_contents = object_contents + "format: " + format + "\n";
  object_contents = object_contents + "description: " + description + "\n";
  object_contents = object_contents + "image_urls: " + image_urls + "\n";
  object_contents = object_contents + "size: " + size + "\n";

  return object_contents;
}

bool Product::isStringInTitleOrDesc(string search_string) {
  string search_string_t = search_string;
  string title_t = title;
  string description_t = description;
  transform(search_string_t.begin(), search_string_t.end(), search_string_t.begin(), ::toupper);
  transform(title_t.begin(), title_t.end(), title_t.begin(), ::toupper);
  transform(description_t.begin(), description_t.end(), description_t.begin(), ::toupper);
  bool isStringInTitle = title_t.find(search_string_t) != string::npos;
  bool isStringInDescription = description_t.find(search_string_t) != string::npos;
  return isStringInTitle || isStringInDescription;
}
