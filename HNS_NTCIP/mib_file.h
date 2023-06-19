#ifndef MIB_FILE_H
#define MIB_FILE_H

#include <string>
#include <vector>
#include "NTCIP.h"

typedef enum
{
    MIB_PARSE_ERR_NOERR = 0,
    MIB_PARSE_ERR_NOSTART
} type_mib_parse_error;

class MIB_Imports
{
private:
    MIB_Imports();

    void fSetObjName(const std::string &obj_name) {f_obj_name = obj_name;}
    std::string fGetObjName() const {return f_obj_name;}

    void fSetModuleName(const std::string &module_name) {f_module_name = module_name;}
    std::string fGetModuleName() const {return f_module_name;}
public:
    std::string f_obj_name;
    std::string f_module_name;
};

class MIB_Tree
{
public:
    MIB_Tree();

    void fSetName(const std::string &name) {f_name = name;}
    std::string fGetName() const {return f_name;}
private:
    NTCIP_Node f_ntcip;
    std::string f_name;
};

class MIB_File
{
public:
    MIB_File(const std::string &filename, const std::string &directory);

    type_mib_parse_error fBuildNTCIP();

private:
    std::string f_filename;
    std::string f_directory;

    std::vector<MIB_Imports> f_imports;
    //each file SHOULD and typically do contain only one tree, but there is nothing to say that a single file can't contain multiple trees either.
    std::vector<MIB_Tree> f_trees;

    std::string fIsComment(const std::string &input) const;
};

class MIB_File_List
{
public:
    MIB_File_List(const std::vector<std::string> &filenames, const std::string &directory);

    NTCIP_Node fParseFiles();
private:

    std::vector<MIB_File> f_files;
};

#endif // MIB_FILE_H
