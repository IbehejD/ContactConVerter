#include "webview.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "nlohmann/json.hpp" // Knihovna pro práci s JSON
#include <string>
#include <vector>

using json = nlohmann::json;

using namespace std;

vector<string> getNextLineAndSplitIntoTokens(istream &str)
{

    vector<string> result;
    string line;
    getline(str, line);

    stringstream lineStream(line);
    string cell;

    while (getline(lineStream, cell, ';'))
    {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    return result;
}

void write_vcf(ofstream &file, vector<string> &contact_info)
{
    string nickname = contact_info.at(0) + " " + contact_info.at(1);
    string fatherFirst = contact_info.at(2);
    string motherFirst = contact_info.at(6);

    if (fatherFirst != "")
    {

        string fatherSecond = contact_info.at(3);
        string email = contact_info.at(4);
        string cell = contact_info.at(5);

        file << "BEGIN:VCARD" << endl
             << "VERSION:3.0" << endl
             << "N;CHARSET=utf-8:" << fatherSecond << ";" << fatherFirst << ";;;;" << endl
             << "FN;CHARSET=utf-8:" << fatherFirst << " " << fatherSecond << endl
             << "NICKNAME:" << nickname << endl
             << "ORG:" << "Skaut" << endl
             << "TEL;type=HOME;type=VOICE;type=pref:+420" << cell << endl
             << "EMAIL;type=INTERNET;type=HOME;type=pref:" << email << endl
             << "END:VCARD" << endl;
    }

    if (motherFirst != "")
    {

        string motherSecond = contact_info.at(7);
        string email = contact_info.at(8);
        string cell = contact_info.at(9);

        file << "BEGIN:VCARD" << endl
             << "VERSION:3.0" << endl
             << "N;CHARSET=utf-8:" << motherSecond << ";" << motherFirst << ";;;;" << endl
             << "FN;CHARSET=utf-8:" << motherFirst << " " << motherSecond << endl
             << "NICKNAME:" << nickname << endl
             << "ORG:" << "Skaut" << endl
             << "TEL;type=HOME;type=VOICE;type=pref:+420" << cell << endl
             << "EMAIL;type=INTERNET;type=HOME;type=pref:" << email << endl
             << "END:VCARD" << endl;
    }
}

int convert(const std::string &fileName, const std::string &content)
{
    // Přidání přípony .vcf k názvu souboru
    std::string outputFileName = fileName.substr(0, fileName.size() - 4) + ".vcf";

    // Explicitní vytvoření souboru, pokud neexistuje
    std::ofstream tempFile(outputFileName, std::ios::app);
    if (!tempFile.is_open())
    {
        std::cerr << "Error creating output file: " << outputFileName << std::endl;
        return -1;
    }
    tempFile.close();

    // Otevření souboru pro zápis
    std::ofstream vcffile(outputFileName);
    if (vcffile.fail())
    {
        std::cerr << "Error opening output file: " << outputFileName << std::endl;
        return -1;
    }

    // Zpracování obsahu CSV
    std::istringstream csvfile(content); // Použití stringstream pro zpracování obsahu
    while (!csvfile.eof())
    {
        std::vector<std::string> contact_info = getNextLineAndSplitIntoTokens(csvfile);
        if (!contact_info.empty())
        {
            write_vcf(vcffile, contact_info);
        }
    }

    vcffile.close();
    return 0;
}

void processFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Nelze otevřít soubor: " + filePath);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::cout << "Řádek: " << line << std::endl;
    }

    file.close();
}

int main()
{
    webview::webview w(true, nullptr);
    w.set_title("Convertor");
    w.set_size(400, 300, WEBVIEW_HINT_NONE);

    // Načtení HTML souboru
    std::string html;
    {
        std::ifstream file("convertor.html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        html = buffer.str();
    }

    w.bind("invoke", [&](const std::string &req) -> std::string
           {
        try {
            auto data = json::parse(req);



            if (data[0].contains("type") && data[0]["type"] == "fileContent" && data[0].contains("fileName") && data[0].contains("content")){

                std::string fileName = data[0]["fileName"];
                std::string content = data[0]["content"];
    
                if (!convert(fileName, content))
                {
                    std::cout << "Soubor byl úspěšně převeden." << std::endl;
                }
                else
                {
                    std::cout << "Chyba při převodu souboru." << std::endl;
                }
    
                return R"({"status": "success"})";
            }

            return R"({"status": "unknown request"})";
        } catch (const std::exception &e) {
            std::cerr << "Chyba: " << e.what() << std::endl;
            return R"({"status": "error"})";
        } });

    w.set_html(html);
    w.run();
    return 0;
}
