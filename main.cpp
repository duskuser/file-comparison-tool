#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "FileData.h"

namespace fs = std::filesystem;

const std::string sectionBreak = "----------";
const std::string logFileName = "Changelog.txt";

std::vector<std::string> getFileNames();
std::vector<FileData> extractFileData(std::vector<std::string> &fileNames);
std::vector<std::string> compareFiles(std::vector<FileData> &fileList);
void outputToChangelog(std::vector<std::string> &fileChanges);

int main()
{
    std::vector<std::string> fileNames = getFileNames();

    if (fileNames.size() != 0)
    {
        std::cout << "Files found: "
                  << "\n";
        for (int i = 0; i < fileNames.size(); i++)
        {
            std::cout << fileNames[i] << "\n";
        }
    }

    std::vector<FileData> fileList = extractFileData(fileNames);

    std::vector<std::string> fileChanges = compareFiles(fileList);

    if (fileChanges.size() != 0)
    {
        std::cout << "Outputting to Changelog.\n";
        outputToChangelog(fileChanges);
    }
    else
    {
        std::cout << "No changes found.\n";
    }
}

std::vector<std::string> getFileNames()
{
    std::vector<std::string> fileNames;
    std::string path_name = "Files";

    for (auto &entry : std::filesystem::directory_iterator(path_name))
    {
        fileNames.push_back(entry.path().string());
    }
    return fileNames;
}

std::vector<FileData> extractFileData(std::vector<std::string> &fileNames)
{
    std::vector<FileData> fileList;
    for (int i = 0; i < fileNames.size(); i++)
    {
        FileData newFile;
        newFile.name = fileNames[i];
        fileList.push_back(newFile);
        std::ifstream inFile;

        inFile.open(fileNames[i]);

        if (inFile.is_open())
        {
            std::string line;
            while (inFile)
            {
                getline(inFile, line);
                fileList[i].data.push_back(line);
            }
        }
        else
        {
            std::cout << "Error opening file " << fileNames[i] << ".\n";
        }
        inFile.close();
    }
    return fileList;
}

std::vector<std::string> compareFiles(std::vector<FileData> &fileList)
{
    std::vector<std::string> changeLog;
    FileData currentFile = fileList[0];
    for (int i = 1; i < fileList.size(); i++)
    {
        // Compare Sizes
        std::cout << "Comparing " << currentFile.name << " & " << fileList[i].name << std::endl;

        if (currentFile.data.size() == fileList[i].data.size())
        {
            // Compare Contents
            for (int k = 0; k < currentFile.data.size(); k++)
            {
                if (currentFile.data[k] != fileList[i].data[k])
                {
                    std::string errorMessage = "Difference found between files " + currentFile.name + " and " + fileList[i].name + " on line " + std::to_string(k);
                    std::string errorMessage2 = "changed to";
                    changeLog.push_back(errorMessage);
                    changeLog.push_back(currentFile.data[k]);
                    changeLog.push_back(errorMessage2);
                    changeLog.push_back(fileList[i].data[k]);
                    changeLog.push_back(sectionBreak);
                }
            }
        }
        else
        {
            // Output if there is a size difference.
            std::string errorMessage = "File size difference found between " + currentFile.name + " and " + fileList[i].name + ".";
            changeLog.push_back(errorMessage);
            changeLog.push_back(sectionBreak);
        }

        // Update currently compared file
        currentFile = fileList[i];
    }
    return changeLog;
}

void outputToChangelog(std::vector<std::string> &fileChanges)
{
    std::ofstream outFile;
    outFile.open(logFileName);
    if (outFile.is_open())
    {
        for (int i = 0; i < fileChanges.size(); i++)
        {
            outFile << fileChanges[i] << std::endl;
        }
    }
    else
    {
        std::cout << "Error creating or opening ChangeLog file.\n";
    }
    outFile.close();
}