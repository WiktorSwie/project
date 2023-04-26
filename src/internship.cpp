#include <chrono>
#include <fstream>
#include <iostream>

#include <date/date.h>
#include <nlohmann/json.hpp>
#include "./rapidjson/document.h"

#include "internship.h"

using json = nlohmann::json;
using namespace date;
using dayPoint = std::chrono::time_point<std::chrono::system_clock, days>;
namespace internship
{
    struct OS
    {
        std::string Name;
        std::string Cycle;
        int SupportPeriod;
        std::string toString() {
            return Name + " " + Cycle + " " + std::to_string(SupportPeriod);

        }
    };

    std::chrono::_V2::system_clock::time_point TimePoint(std::string DateStr){
        std::tm tm = {};
        std::stringstream ss(DateStr);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    std::vector<OS> readfile(const std::string &jsonFileName)
    {
        std::ifstream f(jsonFileName);
        json data = json::parse(f);
        std::vector<OS> OsList;

        for (const auto &[id, OperatingSystem] : data.items())
        {
            if (OperatingSystem["os"] == true)
            {
                OS os{};
                os.Name = OperatingSystem["name"];
                os.Cycle = OperatingSystem["versions"][0]["cycle"];
                auto StartDate = TimePoint(OperatingSystem["versions"][0]["releaseDate"].get<std::string>());
                auto EndDate = TimePoint(OperatingSystem["versions"][0]["eol"].get<std::string>());
                int SupportPeriod = (std::chrono::duration_cast<std::chrono::hours>(EndDate - StartDate).count()/24)+1;
                os.SupportPeriod = SupportPeriod;
                OsList.push_back(os);
            }
        }
        std::sort(OsList.begin(), OsList.end(),[](const OS a, const OS b){
            return a.SupportPeriod > b.SupportPeriod;
        });
        return OsList;
    }

    void solution(const std::string &jsonFileName, int elementsCount)
    {
        std::vector<OS> OsList = readfile(jsonFileName);
        for(int i = 0;i < elementsCount;i++)
        {
            std::cout << OsList.at(i).toString() << "\n";
        }
        
    }
}