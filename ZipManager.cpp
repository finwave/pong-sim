/**
 * ZipManager.cpp source file
 */

#include "ZipManager.h"

CZipManager::CZipManager(void)
{
}

CZipManager::~CZipManager(void)
{
}

void CZipManager::Initialize()
{
    m_sApplicationPath = GetWorkingDir();
}

void CZipManager::UnpackZipFile(const char* zipName)
{
    struct zip* archive;
    struct zip_file* file;
    struct zip_stat fileInfo;

    int i, len;

    // Step 1: Initialize the zip archive

    const fs::path applicationPath = fs::path(m_sApplicationPath, std::locale(""));
    fs::current_path(applicationPath);

    if ((archive = zip_open(zipName, 0, NULL)) == NULL) {
        std::cerr << "Failed to open the zip file." << std::endl;
        return;
    }

    zip_set_default_password(archive, PASSWORD);

    // Step 2: Create data folders

    fs::current_path(fs::temp_directory_path());
    CreateFolder("PongSim");

    m_sDataPath = fs::temp_directory_path().generic_string();
    m_sDataPath += "PongSim/";

    const fs::path dataPath = fs::path(m_sDataPath, std::locale(""));
    fs::current_path(dataPath);

    // Step 3: Extract all the packed zip files

    for (i = 0; i < zip_get_num_entries(archive, 0); i++)
    {
        if (zip_stat_index(archive, i, 0, &fileInfo) == 0)
        {
            len = strlen(fileInfo.name);

            if (fileInfo.name[len - 1] == '/')
            {
                std::string dir = m_sDataPath + fileInfo.name;
                const fs::path dirPath = fs::path(dir, std::locale(""));

                CreateFolder(dirPath);
            }
            else
            {
                file = zip_fopen_index(archive, i, 0);

                if (file)
                {
                    ExtractFile(file, fileInfo);
                    zip_fclose(file);
                }
            }
        }
    }

    // Step 4: Close the zip archive

    zip_close(archive);
}

std::string CZipManager::GetWorkingDir()
{
    char buf[256];
    GetCurrentDirectoryA(256, buf);
    return std::string(buf) + '\\';
}

void CZipManager::CreateFolder(const fs::path dirPath)
{
    fs::create_directory(dirPath);
}

void CZipManager::ExtractFile(struct zip_file* file, struct zip_stat fileInfo)
{
    std::string filePath = m_sDataPath + fileInfo.name;
    std::ofstream outfile(filePath, std::ofstream::binary);

    if (!outfile)
    {
        return;
    }

    char buf[100];
    int len;
    long long sum = 0;

    while (sum != fileInfo.size)
    {
        len = zip_fread(file, buf, 100);

        if (len < 0)
        {
            break;
        }

        outfile.write(buf, len);
        sum += len;
    }

    //std::cout << "File created: " << filePath << '\n';
    outfile.close();
}

void CZipManager::SetResourceFilePath(const char* childPath)
{
    std::string stringChildPath = childPath;
    m_sFullResourceFilePath = m_sDataPath + stringChildPath;
}