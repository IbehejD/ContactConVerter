#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> getNextLineAndSplitIntoTokens(ifstream &str)
{
    vector<string> result;
    string line;
    getline(str, line);

    stringstream lineStream(line);
    string cell;

    while (getline(lineStream, cell, ','))
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Open the file dialog and get the selected file
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,             // Parent widget (use nullptr for no parent)
        "Open File",         // Title of the dialog
        "",                  // Initial directory (empty means home directory)
        "Text Files (*.csv)" // File filters
    );

    // Check if the user selected a file (not canceled)
    if (!fileName.isEmpty())
    {
        QString extension = fileName.mid(fileName.size() - 4, 4);
        QString resultFile = fileName.mid(0, fileName.size() - 4) + ".vcf";

        if (extension != ".csv")
        {
            QMessageBox::information(nullptr, "Wrong File", "You must select a csv file.");

            return app.exec();
        }
        else
        {
            ifstream csvfile(fileName.toStdString());
            ofstream vcffile(resultFile.toStdString());

            if (csvfile.fail() || vcffile.fail())
            {
                cout << "Error opening files." << endl;
                cout << "Aborting..." << endl;
                return app.exec();
            }

            while (!csvfile.eof())
            {
                vector<string> contact_info = getNextLineAndSplitIntoTokens(csvfile);
                write_vcf(vcffile, contact_info);
            }

            csvfile.close();
            vcffile.close();

            // Display the selected file path in a message box
            QMessageBox::information(nullptr, "File converted", "Resulted file: " + resultFile);
            QCoreApplication::quit();
            return app.exec();
        }
    }
    else
    {
        // Handle case when the user cancels the dialog
        QMessageBox::information(nullptr, "No File", "No file was selected.");

        return app.exec();
    }

    return app.exec();
}
// ################## IMPROVEMENTS ###################
// https://github.com/mlabbe/nativefiledialog