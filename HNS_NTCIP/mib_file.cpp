#include "mib_file.h"
#include <fstream>
#include <sstream>

using namespace std;

MIB_File::MIB_File(const string &filename, const string &directory)
    : f_filename(filename)
    , f_directory(directory)
{

}

typedef enum
{
    MIB_STATE_SEARCHING_FOR_START_NAME = 0,
    MIB_STATE_SEARCHING_FOR_START_DEFINITION,
    MIB_STATE_SEARCHING_FOR_START_SYMBOL,
    MIB_STATE_SEARCHING_FOR_START_BEGIN,
    MIB_STATE_STARTED
} type_mib_search_state;

type_mib_parse_error MIB_File::fBuildNTCIP()
{
    type_mib_parse_error error = MIB_PARSE_ERR_NOERR;
    type_mib_search_state search_state = MIB_STATE_SEARCHING_FOR_START_NAME;

    string line;
    ifstream inFile(f_directory + "/" + f_filename);

    while(getline(inFile,line) && error == MIB_PARSE_ERR_NOERR)
    {
        string input,val;
        vector<string> arguments;

//        iss >> input;

        input = fIsComment(line);

        istringstream iss(input);
        if(!input.empty())
        {
            while(iss >> val )
            {
                arguments.push_back(val);
            }

            for(size_t ui=0;ui<arguments.size();ui++)
            {
                switch(search_state)
                {
                case MIB_STATE_SEARCHING_FOR_START_NAME:
                    break;
                case MIB_STATE_SEARCHING_FOR_START_BEGIN:
                    break;
                case MIB_STATE_STARTED:
                    break;
                }
            }
        }
    }

    return error;
}

string MIB_File::fIsComment(const string &input) const
{
    string result;

    size_t position = input.find("--");

    result = input.substr(0,position);

    return result;
}

MIB_File_List::MIB_File_List(const vector<string> &filenames, const string &directory)
{
    for(size_t ui=0;ui<filenames.size();ui++)
    {
        f_files.push_back(MIB_File(filenames[ui],directory));
    }
}

NTCIP_Node MIB_File_List::fParseFiles()
{
    NTCIP_Node result;

    for(size_t ui=0;ui<f_files.size();ui++)
    {
        f_files[ui].fBuildNTCIP();
    }

    return result;
}
