#pragma once
#include<cstdint>
#include <string.h>
class recv_table;

class RecvProp
{
public:
    char* m_pVarName;
    void* m_RecvType;
    int                     m_Flags;
    int                     m_StringBufferSize;
    int                     m_bInsideArray;
    const void* m_pExtraData;
    RecvProp* m_pArrayProp;
    void* m_ArrayLengthProxy;
    void* m_ProxyFn;
    void* m_DataTableProxyFn;
    recv_table* m_pDataTable;
    int                     m_Offset;
    int                     m_ElementStride;
    int                     m_nElements;
    const char* m_pParentArrayPropName;
};

class recv_table
{
public:

    RecvProp* m_pProps;
    int            m_nProps;
    void* m_pDecoder;
    char* m_pNetTableName;
    bool        m_bInitialized;
    bool        m_bInMainList;
};

class ClientClass
{
public:
    void* m_pCreateFn;
    void* m_pCreateEventFn;
    char* m_pNetworkName;
    recv_table* m_pRecvTable;
    ClientClass* m_pNext;
    int                m_ClassID;
public:
    virtual ClientClass* GetAllClasses(void);
};

intptr_t get_offset(recv_table* pTable, const char* table_name, const char* netvar_name) {
    for (int i = 0; i < pTable->m_nProps; i++)
    {
        RecvProp prop = pTable->m_pProps[i];
        if (!_stricmp(prop.m_pVarName, netvar_name)) { return prop.m_Offset; }
        if (prop.m_pDataTable)
        {
            intptr_t offset = get_offset(prop.m_pDataTable, table_name, netvar_name);
            if (offset) { return offset + prop.m_Offset; }
        }
    }
    return 0;
}
intptr_t get_net_var(const char* table_name, const char* netvar_name, ClientClass* client_class) {
    ClientClass* current_node = client_class;
    for (auto current_node = client_class; current_node; current_node = current_node->m_pNext)
    {
        if (!_stricmp(table_name, current_node->m_pRecvTable->m_pNetTableName))
        {
            return get_offset(current_node->m_pRecvTable, table_name, netvar_name);
        }
    }
    return 0;
}
