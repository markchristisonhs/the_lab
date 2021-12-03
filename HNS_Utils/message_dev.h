#ifndef MESSAGE_DEV_H
#define MESSAGE_DEV_H

#include "message.h"
#include <vector>

namespace message_dev
{

class HNS_Message_Page;
class HNS_Message_Justified_Element;
class HNS_Message_Element;

class HNS_Message
{
public:
    HNS_Message();
private:
    std::vector<HNS_Message_Page> f_pages;
};

class HNS_Message_Page
{
public:
    HNS_Message_Page();
private:
    std::vector<HNS_Message_Justified_Element> f_elements;
};

class HNS_Message_Justified_Element
{
public:
    HNS_Message_Justified_Element();
private:
    std::vector<HNS_Message_Element> f_elements;
};

class HNS_Message_Element
{
public:
    HNS_Message_Element();
private:
};

}

#endif // MESSAGE_DEV_H
