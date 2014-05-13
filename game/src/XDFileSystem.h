/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _XD_FILESYSTEM_H_
#define _XD_FILESYSTEM_H_

#include <iostream>
#include <array>

class FileSystem {
  public:
    static std::string RWFolder;
    static std::string ROFolder;
    static unsigned char* GetByteArrayFromFile(std::string filename,unsigned int& length);
    static void Get_RWFolder(std::string& fullpath,const std::string& filename);
    static void Get_ROFolder(std::string& fullpath,const std::string& filename);
    static bool FileExist(const std::string& fullpath);
};

class XDFileSystem {
  public:
    unsigned char* GetByteArrayFromFile(std::string filename,unsigned int& length);
    void ResolveToRWFolder(std::string& fullpath,const std::string& filename);
    void ResolveToROFolder(std::string& fullpath,const std::string& filename);
    bool FileExist(const std::string& fullpath);
    void SetRWFolder(const std::string& rwfolder);
    void SetROFolder(const std::string& rofolder);
  private:
    std::string m_RWFolder;
    std::string m_ROFolder;
};

#endif // _XD_FILESYSTEM_H_