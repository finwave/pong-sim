/**
 * CResources.h header file
 */

#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <iostream>
#include <filesystem>
#include <fstream>
#include <zip.h>

namespace fs = std::filesystem;

class CResources
{
public:

	CResources(void);
	~CResources(void);

	void Initialize(void);
	void SetResourceFilePath(const char* childPath);

	inline const char* GetResourceFilePath() { return this->m_sFullResourceFilePath.c_str(); }

private:

	void create_folder(const fs::path dirPath);
	void extract_file(struct zip_file* file, struct zip_stat fileInfo);

	const char* PASSWORD = "mf8p_LWs*j";

	std::string m_sDataPath;
	std::string m_sFullResourceFilePath;
};

#endif //__RESOURCES_H__
