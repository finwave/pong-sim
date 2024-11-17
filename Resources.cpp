/**
 * Resources.cpp header file
 */

#include "Resources.h"

CResources::CResources(void)
{
}

CResources::~CResources(void)
{
}

void CResources::Initialize(void)
{
    struct zip* archive;
    struct zip_file* file;
    struct zip_stat fileInfo;

    int i, len;

    // Step 1: Initialize the zip archive

    if ((archive = zip_open("data", 0, NULL)) == NULL) {
        std::cerr << "Failed to open the zip file." << std::endl;
        return;
    }

    zip_set_default_password(archive, PASSWORD);

    // Step 2: Create data folders

    fs::current_path(fs::temp_directory_path());
    create_folder("PongSim");

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

                create_folder(dirPath);
            }
            else
            {
                file = zip_fopen_index(archive, i, 0);

                if (file)
                {
                    extract_file(file, fileInfo);
                    zip_fclose(file);
                }
            }
        }
    }

    // Step 4: Close the zip archive

    zip_close(archive);
}

void CResources::create_folder(const fs::path dirPath)
{
    fs::create_directory(dirPath);
}

void CResources::extract_file(struct zip_file* file, struct zip_stat fileInfo)
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

void CResources::SetResourceFilePath(const char* childPath)
{
    std::string stringChildPath = childPath;
    m_sFullResourceFilePath = m_sDataPath + stringChildPath;
}