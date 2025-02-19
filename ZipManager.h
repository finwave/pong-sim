/**
 * ZipManager.h header file
 */

#ifndef __ZIPMANAGER_H__
#define __ZIPMANAGER_H__

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <zip.h>

namespace fs = std::filesystem;

class CZipManager
{
public:

	CZipManager(void);
	~CZipManager(void);

	void Initialize(void);
	void UnpackZipFile(const char* zipName);
	void SetResourceFilePath(const char* childPath);

	inline const char* GetResourceFilePath() { return this->m_sFullResourceFilePath.c_str(); }

private:

	std::string GetWorkingDir();
	void CreateFolder(const fs::path dirPath);
	void ExtractFile(struct zip_file* file, struct zip_stat fileInfo);

	const char* PASSWORD = "mf8p_LWs*j";

	std::string m_sApplicationPath;
	std::string m_sDataPath;
	std::string m_sFullResourceFilePath;
};

#endif //__ZIPMANAGER_H__
