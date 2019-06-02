#include <cpprest/json.h>
#include <sstream>
#include <nlohmann/json.hpp>

using namespace std;
typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;
typedef std::wstring String;
typedef std::wstringstream StringStream;

String JsonValueTypeToString(const JsonValueType& type)
{
  switch (type)
    {
        case JsonValueType::Array: return L"Array";
        case JsonValueType::Boolean: return L"Boolean";
        case JsonValueType::Null: return L"Null";
        case JsonValueType::Number: return L"Number";
        case JsonValueType::Object: return L"Object";
        case JsonValueType::String: return L"String";
    }
}

int main()
{
    nlohmann::json j;
    struct Product
    {
        std::string name;
        int price;
        std::vector<std::string> sizes;
    };
    Product product= Product();
    product.name = "hola";
    product.price = 12;
    product.sizes = std::vector<std::string>();
    product.sizes.push_back("Ala");
    product.sizes.push_back("bala");
    j = {
        { "name", product.name },   // explicit relationship between "name" and "name"
        { "price", product.price }, // explicit relationship between "price" and "price"
        { "sizes", product.sizes }  // explicit relationship between "sizes" and "sizes"
    };
    cout << j;

    Product nproduct = Product();
    nproduct.name = j["name"];
    cout << product.name;


    return 0;
}
