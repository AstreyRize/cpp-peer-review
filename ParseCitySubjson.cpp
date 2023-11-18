#include <vector>
#include <string>

using namespace std;

struct Language {
    // Определения Language...
};

struct City {
    string name, iso_code, full_phone_code, country_name, country_iso_code, country_time_zone;
    vector<Language> languages;
};

struct Country {
    string name, iso_code, phone_code, time_zone;
    vector<Language> languages;
};

string GetStringFromJson(const Json& json, const string& key) {
    return json.AsObject().at(key).AsString();
}

void ParseCitySubjson(vector<City>& cities, const Json& city_json, const Country& country) {
    for (const auto& city_obj : city_json.AsList()) {
        cities.push_back({ GetStringFromJson(city_obj, "name"),
                          GetStringFromJson(city_obj, "iso_code"),
                          country.phone_code + GetStringFromJson(city_obj, "phone_code"),
                          country.name, country.iso_code, country.time_zone, country.languages });
    }
}

void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        Country country{
            GetStringFromJson(country_json, "name"),
            GetStringFromJson(country_json, "iso_code"),
            GetStringFromJson(country_json, "phone_code"),
            GetStringFromJson(country_json, "time_zone"),
        };

        for (const auto& lang_obj : country_json.AsObject()["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }

        countries.push_back(country);
        ParseCitySubjson(cities, country_json.AsObject()["cities"s], countries.back());
    }
}
