/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"

#include "XDFileSystem.h"
#include "XDChecker.h"
#include <fstream>

std::string FileSystem::RWFolder = "";
std::string FileSystem::ROFolder = "";

unsigned char* FileSystem::GetByteArrayFromFile(std::string filename,unsigned int& length)  {
    unsigned char* f_bytearray = NULL;
    // open the file
    std::ifstream VertexFile(filename, std::ios::in | std::ios::binary | std::ios::ate);
    // if open was successful
    bool is_open_ok = VertexFile.is_open() ;
    assert( is_open_ok ) ;
    if(is_open_ok) {
        // find the length of the file
        int f_length = (int)VertexFile.tellg();
        length = f_length;
        // collect the file data
        f_bytearray = new unsigned char[f_length];
        VertexFile.seekg(0, std::ios::beg);
        VertexFile.read(reinterpret_cast<char*>(f_bytearray), f_length);
        VertexFile.close();
    }
    return f_bytearray;
}

void FileSystem::Get_RWFolder(std::string& fullpath,const std::string& filename) {
    assert( ( RWFolder.compare("") != 0 ) );
    fullpath.append(RWFolder);
    fullpath.append("\\");
    fullpath.append(filename);
}

void FileSystem::Get_ROFolder(std::string& fullpath,const std::string& filename) {
    assert( ( ROFolder.compare("") != 0 ) );
    fullpath.append(ROFolder);
    fullpath.append("\\");
    fullpath.append(filename);
}

bool FileSystem::FileExist(const std::string& fullpath) {
    // not the best way
    std::ifstream checkedFile(fullpath, std::ios::in | std::ios::binary | std::ios::ate);
    bool is_open_ok = checkedFile.is_open() ;
    checkedFile.close();
    return is_open_ok;
}

// XDFileSystem

unsigned char* XDFileSystem::GetByteArrayFromFile(std::string filename,unsigned int& length)  {
    unsigned char* f_bytearray = NULL;
    // open the file
    std::ifstream VertexFile(filename, std::ios::in | std::ios::binary | std::ios::ate);
    // if open was successful
    bool is_open_ok = VertexFile.is_open() ;
    XDChecker::Assert( is_open_ok , " can't open file " + filename );
    if(is_open_ok) {
        // find the length of the file
        int f_length = (int)VertexFile.tellg();
        length = f_length;
        // collect the file data
        f_bytearray = new unsigned char[f_length];
        VertexFile.seekg(0, std::ios::beg);
        VertexFile.read(reinterpret_cast<char*>(f_bytearray), f_length);
        VertexFile.close();
    }
    return f_bytearray;
}

void XDFileSystem::ResolveToRWFolder(std::string& fullpath,const std::string& filename) {
    XDChecker::Assert(  m_RWFolder.compare("") != 0 , " m_RWFolder is empty string, must be set first " );
    fullpath.append(m_RWFolder);
    fullpath.append("\\");
    fullpath.append(filename);
}

void XDFileSystem::ResolveToROFolder(std::string& fullpath,const std::string& filename) {
    XDChecker::Assert(  m_ROFolder.compare("") != 0 , " m_ROFolder is empty string, must be set first " );
    fullpath.append(m_ROFolder);
    fullpath.append("\\");
    fullpath.append(filename);
}

bool XDFileSystem::FileExist(const std::string& fullpath) {
    // not the best way
    std::ifstream checkedFile(fullpath, std::ios::in | std::ios::binary | std::ios::ate);
    bool is_open_ok = checkedFile.is_open() ;
    checkedFile.close();
    return is_open_ok;
}

void XDFileSystem::SetRWFolder(const std::string& rwfolder) {
    m_RWFolder = rwfolder;
}

void XDFileSystem::SetROFolder(const std::string& rofolder) {
    m_ROFolder = rofolder;
}