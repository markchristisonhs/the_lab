#ifndef NTCIP_H
#define NTCIP_H

#include <vector>
#include <string>
#include <functional>
#include "SNMP.h"
#include "logaline.h"

typedef enum
{
    HNS_NTCIP_READ = 0,
    HNS_NTCIP_READWRITE
} type_ntcip_data_access;

typedef enum
{
    HNS_NTCIP_INTEGER = 0,
    HNS_NTCIP_OCTETSTRING,
    HNS_NTCIP_COUNTER,
    HNS_NTCIP_TIMETICKS
} type_ntcip_data_type;

typedef enum
{
    HNS_NTCIP_NOERROR = 0x0,
    HNS_NTCIP_CHILDNOTFOUND = 0x1
} type_ntcip_error;

void SetLogALine_NTCIP_Node(HNS_LogALine2 *logaline);

//This is utterly bizarre and deserves some explanation.  I know HNS_NTCIP_XML is in a different library.  However, doing this will
//allow HNS_NTCIP_XML to be a friend of NTCIP_Node which it needs.  I just hope this works at link time.
//Holy shit, this actually worked and links fine.
class HNS_NTCIP_XML;

class NTCIP_Node
{
    friend class HNS_NTCIP_XML;
public:
    NTCIP_Node(NTCIP_Node *parent = nullptr);
    NTCIP_Node(const int &oid_number, NTCIP_Node *parent = nullptr);
    //MSC20211020 This doesn't have a copy constructor.  Why doesn't it have a copy constructor?  oops.
    NTCIP_Node(const NTCIP_Node &copy);

    NTCIP_Node &operator=(const NTCIP_Node &rhs);

    ~NTCIP_Node();

    void fClone(const NTCIP_Node &clone_source, NTCIP_Node *parent = nullptr);

    void fSetOidNumber(const int &oid_number);
    int fGetOidNumber();

    std::string fGetWholeOID(const std::string &child_oid = "");

    void fAddChild(const std::string &oid, const std::string &name = "", NTCIP_Node *parent = nullptr);

    void fSetData(const std::string &oid, const std::string &data, const type_ntcip_data_access &access, const type_ntcip_data_type &data_type, const bool &binary_mode = false);
    void fSetData(const std::string &oid, const int &data, const type_ntcip_data_access &access, const type_ntcip_data_type &data_type, const bool &binary_mode = false);
    void fSetInt64Data(const std::string &oid, const int64_t &data, const type_ntcip_data_access &access, const type_ntcip_data_type &data_type, const bool &binary_mode = false);
    void fSetData(const std::string &oid, const std::vector<unsigned char> &data, const type_ntcip_data_access &access, const type_ntcip_data_type &data_type, const bool &binary_mode = false);
    //This assumes that the data is writable and is of type integer.
    void fIncrementData(const std::string &oid);

    SNMP_PDU_Data fNetworkSetData(const std::string &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode = false);
    SNMP_PDU_Data fNetworkSetData(const int &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode = false);
    SNMP_PDU_Data fNetworkSetData(const std::vector<unsigned char> &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode = false);
    SNMP_PDU_Data fNetworkSetData(const SNMP_Data &data, const bool &verify);

    SNMP_PDU_Data fNetworkSetData(const std::string &oid, const std::string &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode = false, type_ntcip_error *error = nullptr);
    SNMP_PDU_Data fNetworkSetData(const std::string &oid, const int &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode = false, type_ntcip_error *error = nullptr);
    SNMP_PDU_Data fNetworkSetData(const std::string &oid, const std::vector<unsigned char> &data, const type_snmp_data_types &data_type, const bool &verify, const bool &binary_mode = false, type_ntcip_error *error = nullptr);
    SNMP_PDU_Data fNetworkSetData(const std::string &oid, const SNMP_Data &data, const bool &verify, type_ntcip_error *error = nullptr);

    bool fNetworkGetData(std::vector<unsigned char> &data, const type_ntcip_data_type &data_type);

    //gets the data, if it exists, at this current node
    std::vector<unsigned char> fGetData() const;
    std::string fGetStringData() const;
    int fGetIntData() const;
    int64_t fGetInt64Data() const;
    std::string fGetRawDataAsString() const;
    size_t fGetDataSize() const;
    type_ntcip_data_type fGetDataType() const;

    //gets data from a child node
    std::vector<unsigned char> fGetData(const std::string &oid, const std::vector<unsigned char> &default_value = std::vector<unsigned char> (), type_ntcip_error *error = nullptr);
    std::string fGetStringData(const std::string &oid, const std::string &default_value = "", type_ntcip_error *error = nullptr);
    int fGetIntData(const std::string &oid, const int &default_value = 0, type_ntcip_error *error = nullptr);
    int64_t fGetInt64Data(const std::string &oid, const int64_t &default_value = 0L, type_ntcip_error *error = nullptr);
    std::string fGetRawDataAsString(const std::string &oid, const std::string &default_value = "", type_ntcip_error *error = nullptr);
    size_t fGetDataSize(const std::string &oid, type_ntcip_error *error = nullptr);
    type_ntcip_data_type fGetDataType(const std::string &oid, type_ntcip_error *error = nullptr);

    std::string fGetName() const;
    size_t fGetNumChildren() const;
    size_t fGetNumChildren(const std::string &oid, type_ntcip_error *error = nullptr);
    bool fDoesChildExist(const std::string &oid);

    type_ntcip_data_access fGetDataAccess() const;

    bool fGetBinaryMode() const;
    void fClearChildren();

    void fSetSetter(std::function<SNMP_PDU_Data(void *data, const int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> setter);
    void fSetGetter(std::function<SNMP_PDU_Data(void *data, int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> getter);

    void fSetSetter(const std::string &oid, std::function<SNMP_PDU_Data(void *data, const int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> setter, type_ntcip_error *error = nullptr);
    void fSetGetter(const std::string &oid, std::function<SNMP_PDU_Data(void *data, int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> getter, type_ntcip_error *error = nullptr);
private:
    bool fValidateSNMP_Type(const type_snmp_data_types &data_type);

    //This is a HUGE issue.  The pointer to the child should NEVER be exposed.  Doing so means checking the resulting pointer for null EVERY time!!!
    //Never trust yourself to be responsible and not be lazy and do the right thing each and every time.
    //Instead all functions above should use the OID to perform their actions
    NTCIP_Node *fGetChild( const size_t &index);
    NTCIP_Node *fGetChild( const std::string &oid, std::string *last_valid_oid = nullptr);
    NTCIP_Node *fGetChildByName( const std::string &name);
    int f_oid_number;
    std::vector<NTCIP_Node *> f_children;
    NTCIP_Node * f_parent;

    type_ntcip_data_type f_data_type;
    std::vector<unsigned char> f_data;
    type_ntcip_data_access f_access;
    bool f_binary_mode;

    std::string f_name;

    std::function<SNMP_PDU_Data(void *data, int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> f_getter;
    std::function<SNMP_PDU_Data(void *data, const int &data_size, const type_snmp_data_types &data_type, NTCIP_Node *sender, const bool &verify)> f_setter;
};

#endif // NTCIP_H
