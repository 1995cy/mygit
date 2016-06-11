#include <mxml.h>

#ifndef XMLGET_H_INCLUDED
#define XMLGET_H_INCLUDED

struct xmlVal{
    char version[16];
    char time[16];
    char necessity[8];
    char hash_md5[38];
    char name[64];
};

int getxmlval(struct xmlVal *valget);

int find_node(mxml_node_t *xml ,char *name,char *node_val);

#endif // XMLGET_H_INCLUDED
