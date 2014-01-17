//
// Generated file, do not edit! Created by opp_msgc 4.3 from PriorityEtherFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "PriorityEtherFrame_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(PriorityEtherFrame);

PriorityEtherFrame::PriorityEtherFrame(const char *name, int kind) : EtherFrame(name,kind)
{
    this->priority_var = 0;
}

PriorityEtherFrame::PriorityEtherFrame(const PriorityEtherFrame& other) : EtherFrame(other)
{
    copy(other);
}

PriorityEtherFrame::~PriorityEtherFrame()
{
}

PriorityEtherFrame& PriorityEtherFrame::operator=(const PriorityEtherFrame& other)
{
    if (this==&other) return *this;
    EtherFrame::operator=(other);
    copy(other);
    return *this;
}

void PriorityEtherFrame::copy(const PriorityEtherFrame& other)
{
    this->priority_var = other.priority_var;
}

void PriorityEtherFrame::parsimPack(cCommBuffer *b)
{
    EtherFrame::parsimPack(b);
    doPacking(b,this->priority_var);
}

void PriorityEtherFrame::parsimUnpack(cCommBuffer *b)
{
    EtherFrame::parsimUnpack(b);
    doUnpacking(b,this->priority_var);
}

int PriorityEtherFrame::getPriority() const
{
    return priority_var;
}

void PriorityEtherFrame::setPriority(int priority)
{
    this->priority_var = priority;
}

class PriorityEtherFrameDescriptor : public cClassDescriptor
{
  public:
    PriorityEtherFrameDescriptor();
    virtual ~PriorityEtherFrameDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(PriorityEtherFrameDescriptor);

PriorityEtherFrameDescriptor::PriorityEtherFrameDescriptor() : cClassDescriptor("PriorityEtherFrame", "EtherFrame")
{
}

PriorityEtherFrameDescriptor::~PriorityEtherFrameDescriptor()
{
}

bool PriorityEtherFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PriorityEtherFrame *>(obj)!=NULL;
}

const char *PriorityEtherFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PriorityEtherFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int PriorityEtherFrameDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *PriorityEtherFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "priority",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int PriorityEtherFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "priority")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PriorityEtherFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *PriorityEtherFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int PriorityEtherFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PriorityEtherFrame *pp = (PriorityEtherFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PriorityEtherFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PriorityEtherFrame *pp = (PriorityEtherFrame *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPriority());
        default: return "";
    }
}

bool PriorityEtherFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PriorityEtherFrame *pp = (PriorityEtherFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setPriority(string2long(value)); return true;
        default: return false;
    }
}

const char *PriorityEtherFrameDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *PriorityEtherFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PriorityEtherFrame *pp = (PriorityEtherFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


