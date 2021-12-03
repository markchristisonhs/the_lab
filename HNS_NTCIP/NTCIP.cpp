#include "NTCIP.h"
#include <utilities.h>

#include <sstream>
#include <iomanip>
#include <HNS_NTCIP_Enums.h>

using namespace std;
using namespace NTCIP_SNMP;

static HNS_LogALine2 *gs_logaline = nullptr;

void SetLogALine_NTCIP_Node(HNS_LogALine2 *logaline)
{
    gs_logaline = logaline;
}

void LogALine(const string &line, const string &caller)
{
    if(gs_logaline != nullptr)
    {
        gs_logaline->fLogALine(line,caller);
    }
}

NTCIP_Node::NTCIP_Node(NTCIP_Node *parent)
{
    f_oid_number = 1;
    f_data_type = HNS_NTCIP_INTEGER;
    f_access = HNS_NTCIP_READ;
    f_binary_mode = false;
    f_parent = parent;
}

NTCIP_Node::NTCIP_Node(const int &oid_number, NTCIP_Node *parent)
{
    f_oid_number = oid_number;
    f_data_type = HNS_NTCIP_INTEGER;
    f_access = HNS_NTCIP_READ;
    f_binary_mode = false;
    f_parent = parent;
}

NTCIP_Node::NTCIP_Node(const NTCIP_Node &copy)
{
    *this = copy;
}

NTCIP_Node &NTCIP_Node::operator=(const NTCIP_Node &rhs)
{
    fClone(rhs,nullptr);
    return *this;
}

NTCIP_Node::~NTCIP_Node()
{
    for(size_t i=0;i<f_children.size();i++)
    {
        delete f_children[i];
        f_children[i] = nullptr;
    }
}

void NTCIP_Node::fClone(const NTCIP_Node &clone_source, NTCIP_Node *parent)
{
    f_oid_number = clone_source.f_oid_number;
    f_name = clone_source.f_name;
    f_binary_mode = clone_source.f_binary_mode;
    f_access = clone_source.f_access;
    f_data = clone_source.f_data;
    f_data_type = clone_source.f_data_type;
    f_parent = parent;

    fClearChildren();
    f_children.resize(clone_source.fGetNumChildren());
    for(size_t i=0;i<f_children.size();i++)
    {
        f_children[i] = new NTCIP_Node();
        f_children[i]->fClone(*clone_source.f_children[i],this);
    }
}

void NTCIP_Node::fSetOidNumber(const int &oid_number)
{
    f_oid_number = oid_number;
}

int NTCIP_Node::fGetOidNumber()
{
    return f_oid_number;
}

string NTCIP_Node::fGetWholeOID(const std::string &child_oid)
{
    string result;
    stringstream ss;

    ss << f_oid_number;
    if(!child_oid.empty())
    {
        ss << "." << child_oid;
    }
    result = ss.str();

    if(f_parent != nullptr)
    {
        result = f_parent->fGetWholeOID(result);
    }

    return result;
}

void NTCIP_Node::fAddChild(const string &oid, const string &name, NTCIP_Node *parent)
{
    istringstream input(oid);
    string temp_oid;
    int n;
    char temp;
    size_t i;

    //temp strips off '.'
    input >> n >> temp;

    //if this is not the last number, then there are further children to add
    if(input.tellg() != -1)
    {
        //Adding a child to this node
        if(n == f_oid_number)
        {
            temp_oid = string(input.str().substr(input.tellg()));
            input >> n;
            for(i=0;i<f_children.size();i++)
            {
                if(f_children[i]->fGetOidNumber() == n)
                {
                    f_children[i]->fAddChild(temp_oid,name,this);
                    break;
                }
            }

            //node not found
            if(i == f_children.size())
            {
                f_children.push_back(new NTCIP_Node(n,this));
                f_children[f_children.size()-1]->fAddChild(temp_oid,name,this);
            }
        }
    }
    //this is the last number, then there are no further children to add.  Set the name here
    else
    {
        if(n == f_oid_number)
        {
            f_name = name;
            f_parent = parent;
        }
    }
}

void NTCIP_Node::fSetData(const string &oid, const string &data, const type_ntcip_data_access &access, const type_ntcip_data_type &data_type, const bool &binary_mode)
{
    vector <unsigned char> raw_data;
    if(!binary_mode)
    {
        //this is a string;
        for(size_t i = 0;i<data.size();i++)
        {
            raw_data.push_back(data[i]);
        }
    }
    else
    {
        string tempstring = data;
        if(tempstring.substr(0,2) == "0x")
        {
            //strip off initial "0x" if present
            tempstring.erase(0,2);
        }
        raw_data = HexStringToByteArray(tempstring);
    }
    fSetData(oid,raw_data,access,data_type,binary_mode);
}

void NTCIP_Node::fSetData(const string &oid, const int &data, const type_ntcip_data_access &access, const type_ntcip_data_type &data_type, const bool &binary_mode)
{
    vector <unsigned char> raw_data = HNS_intToByteArray(data);
    fSetData(oid,raw_data,access,data_type,binary_mode);
}

void NTCIP_Node::fSetInt64Data(const std::string &oid, const int64_t &data, const type_ntcip_data_access &access, const type_ntcip_data_type &data_type, const bool &binary_mode)
{
    vector <unsigned char> raw_data = HNS_int64ToByteArray(data);
    fSetData(oid,raw_data,access,data_type,binary_mode);
}

SNMP_PDU_Data NTCIP_Node::fNetworkSetData(const std::string &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode)
{
    vector <unsigned char> raw_data;
    if(!binary_mode)
    {
        //this is a string;
        for(size_t i = 0;i<data.size();i++)
        {
            raw_data.push_back(data[i]);
        }
    }
    else
    {
        string tempstring = data;
        if(tempstring.substr(0,2) == "0x")
        {
            //strip off initial "0x" if present
            tempstring.erase(0,2);
        }
        raw_data = HexStringToByteArray(tempstring);
    }
    return fNetworkSetData(raw_data,data_type,verify,binary_mode);
}

SNMP_PDU_Data NTCIP_Node::fNetworkSetData(const int &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode)
{
    vector <unsigned char> raw_data = HNS_intToByteArray(data);
    return fNetworkSetData(raw_data,data_type,verify,binary_mode);
}

SNMP_PDU_Data NTCIP_Node::fNetworkSetData(const std::vector<unsigned char> &data, const type_snmp_data_types &data_type, const bool &verify, const bool &/*binary_mode*/)
{
    SNMP_PDU_Data result;
    SNMP_Data tempdata;

    if(f_access == HNS_NTCIP_READ)
    {
        result.fSetErrorStatus(SNMP_ERR_noAccess);
        return result;
    }
    else
    {
        if(fValidateSNMP_Type(data_type))
        {
            if(f_setter)
            {
                result = f_setter((void *)data.data(),data.size(),data_type,this,verify);
            }
            else
            {
                f_data = data;
                tempdata.fSetData(data,data_type);
                result.fAddVarBinding(SNMP_Variable_Bindings(fGetWholeOID(),tempdata));
            }
            return result;
        }
        else
        {
            LogALine("Wrong type", "NTCIP_Node::fNetworkSetData");
            result.fSetErrorStatus(SNMP_ERR_wrongType);
            return result;
        }
    }
}

SNMP_PDU_Data NTCIP_Node::fNetworkSetData(const SNMP_Data &data, const bool &verify)
{
    return fNetworkSetData(data.fGetData(),data.fGetDataType(),verify,true);
}

SNMP_PDU_Data NTCIP_Node::fNetworkSetData(const std::string &oid, const std::string &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fNetworkSetData(data,data_type,verify,binary_mode);
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return SNMP_PDU_Data();
    }
}

SNMP_PDU_Data NTCIP_Node::fNetworkSetData(const std::string &oid, const int &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fNetworkSetData(data,data_type,verify,binary_mode);
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return SNMP_PDU_Data();
    }
}

SNMP_PDU_Data NTCIP_Node::fNetworkSetData(const std::string &oid, const std::vector<unsigned char> &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fNetworkSetData(data,data_type,verify,binary_mode);
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return SNMP_PDU_Data();
    }
}

SNMP_PDU_Data NTCIP_Node::fNetworkSetData(const std::string &oid, const SNMP_Data &data, const bool &verify, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fNetworkSetData(data,verify);
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return SNMP_PDU_Data();
    }
}

bool NTCIP_Node::fNetworkGetData(std::vector<unsigned char> &/*data*/, const type_ntcip_data_type &/*data_type*/)
{
    return false;
}

vector<unsigned char> NTCIP_Node::fGetData() const
{
    return f_data;
}

string NTCIP_Node::fGetStringData() const
{
    string result;

    for(size_t i=0;i<f_data.size();i++)
    {
        result += f_data[i];
    }

    return result;
}

int NTCIP_Node::fGetIntData() const
{
    return HNS_ByteArrayToint(f_data);
}

int64_t NTCIP_Node::fGetInt64Data() const
{
    return HNS_ByteArrayToint64(f_data);
}

string NTCIP_Node::fGetRawDataAsString() const
{
    string result;
    if(f_data_type == HNS_NTCIP_INTEGER)
    {
        result = to_string(HNS_ByteArrayToint(f_data));
    }
    else
    {
        if(f_binary_mode)
        {
            stringstream ss;
            ss << "0x";
            result = ss.str();
            for(size_t i=0;i<f_data.size();i++)
            {
                //result += f_data[i];
                ss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int> (f_data[i]);
                result = ss.str();
            }
            result = ss.str();
        }
        else
        {
            for(size_t i=0;i<f_data.size();i++)
            {
                result += f_data[i];
            }
        }
    }

    return result;
}

size_t NTCIP_Node::fGetDataSize() const
{
    return f_data.size();
}

size_t NTCIP_Node::fGetNumChildren() const
{
    return f_children.size();
}

size_t NTCIP_Node::fGetNumChildren(const string &oid, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fGetNumChildren();
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return 0;
    }
}

bool NTCIP_Node::fDoesChildExist(const std::string &oid)
{
    bool result = false;

    NTCIP_Node *child = fGetChild(oid);

    if(child != nullptr)
    {
        result = true;
    }

    return result;
}

type_ntcip_data_type NTCIP_Node::fGetDataType() const
{
    return f_data_type;
}

vector<unsigned char> NTCIP_Node::fGetData(const string &oid, const vector<unsigned char> &default_value, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fGetData();
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return default_value;
    }
}

string NTCIP_Node::fGetStringData(const std::string &oid, const string &default_value, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fGetStringData();
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return default_value;
    }
}

int NTCIP_Node::fGetIntData(const std::string &oid, const int &default_value, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fGetIntData();
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return default_value;
    }
}

int64_t NTCIP_Node::fGetInt64Data(const std::string &oid, const int64_t &default_value, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fGetInt64Data();
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return default_value;
    }
}

string NTCIP_Node::fGetRawDataAsString(const std::string &oid, const string &default_value, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fGetRawDataAsString();
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return default_value;
    }
}

size_t NTCIP_Node::fGetDataSize(const std::string &oid, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fGetDataSize();
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return 0;
    }
}

type_ntcip_data_type NTCIP_Node::fGetDataType(const std::string &oid, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        return child_node->fGetDataType();
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
        return HNS_NTCIP_INTEGER;
    }
}

NTCIP_Node *NTCIP_Node::fGetChild(const size_t &index)
{
    if(index < f_children.size())
    {
        return f_children[index];
    }
    else
    {
        return nullptr;
    }
}

NTCIP_Node *NTCIP_Node::fGetChild(const string &oid, std::string *last_valid_oid)
{
    istringstream input(oid);
    string temp_oid = oid;
    int n;
    char temp;

    input >> n >> temp;

    //if this is not the last number, then there are further children to search
    if(input.tellg() != -1)
    {
        if(n == f_oid_number)
        {
            temp_oid = string(input.str().substr(input.tellg()));
            input >> n;

            for(size_t i=0;i<f_children.size();i++)
            {
                if(f_children[i]->fGetOidNumber() == n)
                {
                    return f_children[i]->fGetChild(temp_oid,last_valid_oid);
                }
            }
        }
    }
    //this is the last number, return this child
    else
    {
        if(n == f_oid_number)
        {
            return this;
        }
    }

    if(last_valid_oid != nullptr)
    {
        *last_valid_oid = temp_oid;
    }
    return nullptr;
}

NTCIP_Node *NTCIP_Node::fGetChildByName( const std::string &name)
{
    NTCIP_Node *result = nullptr;
    SNMP_Data temp_data;

    if(f_name == name)
    {
        result = this;
    }
    else
    {
        for(unsigned int ui=0;ui<f_children.size();ui++)
        {
            result = f_children[ui]->fGetChildByName(name);
            if(result != nullptr)
            {
                break;
            }
        }
    }

    return result;
}

string NTCIP_Node::fGetName() const
{
    return f_name;
}

type_ntcip_data_access NTCIP_Node::fGetDataAccess() const
{
    return f_access;
}

bool NTCIP_Node::fGetBinaryMode() const
{
    return f_binary_mode;
}

void NTCIP_Node::fClearChildren()
{
    for(size_t i=0;i<f_children.size();i++)
    {
        delete f_children[i];
        f_children[i] = nullptr;
    }
    f_children.clear();
}

void NTCIP_Node::fSetSetter(std::function<SNMP_PDU_Data(void *data, const int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> setter)
{
    f_setter = setter;
}

void NTCIP_Node::fSetGetter(std::function<SNMP_PDU_Data(void *data, int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> getter)
{
    f_getter = getter;
}

void NTCIP_Node::fSetSetter(const std::string &oid, std::function<SNMP_PDU_Data(void *data, const int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> setter, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        child_node->fSetSetter(setter);
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
    }
}

void NTCIP_Node::fSetGetter(const std::string &oid, std::function<SNMP_PDU_Data(void *data, int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> getter, type_ntcip_error *error)
{
    NTCIP_Node *child_node = fGetChild(oid);

    if(child_node != nullptr)
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_NOERROR;
        }
        child_node->fSetGetter(getter);
    }
    else
    {
        if(error != nullptr)
        {
            *error = HNS_NTCIP_CHILDNOTFOUND;
        }
    }
}

void NTCIP_Node::fSetData(const std::string &oid, const std::vector<unsigned char> &data, const type_ntcip_data_access &access, const type_ntcip_data_type &data_type, const bool &binary_mode)
{
    istringstream input(oid);
    string temp_oid;
    int n;
    char temp;

    input >> n >> temp;

    //if this is not the last number, then there are further children to search
    if(input.tellg() != -1)
    {
        if(n == f_oid_number)
        {
            temp_oid = string(input.str().substr(input.tellg()));
            input >> n;
            for(size_t i=0;i<f_children.size();i++)
            {
                if(f_children[i]->fGetOidNumber() == n)
                {
                    f_children[i]->fSetData(temp_oid,data,access,data_type,binary_mode);
                }
            }
        }
    }
    //this is the last number, set data here
    else
    {
        if(n == f_oid_number)
        {
            f_data.clear();
            for(size_t i = 0;i<data.size();i++)
            {
                f_data.push_back(data[i]);
            }
            f_access = access;
            f_data_type = data_type;
            f_binary_mode = binary_mode;
        }
    }
}

void NTCIP_Node::fIncrementData(const std::string &oid)
{
    NTCIP_Node *current_node = fGetChild(oid);
    int itemp;

    if(current_node != nullptr)
    {
        if(current_node->fGetDataType() == HNS_NTCIP_INTEGER)
        {
            itemp = current_node->fGetIntData();
            fSetData(oid,itemp+1,current_node->fGetDataAccess(),current_node->fGetDataType());
        }
    }
}

bool NTCIP_Node::fValidateSNMP_Type(const type_snmp_data_types &data_type)
{
    if((f_data_type == HNS_NTCIP_INTEGER)
    || (f_data_type == HNS_NTCIP_COUNTER)
    || (f_data_type == HNS_NTCIP_TIMETICKS))
    {
        return data_type == SNMP_INTEGER;
    }
    else if(f_data_type == HNS_NTCIP_OCTETSTRING)
    {
        return (data_type == SNMP_OCTET_STRING) || (data_type == SNMP_OID);
    }
    else
    {
        return false;
    }
}
