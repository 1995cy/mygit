#include <mxml.h>
#include "httprev.h"

#ifndef XMLGET_H_INCLUDED
#define XMLGET_H_INCLUDED

int getxmlval(struct xmlVal *valget);

int find_node(mxml_node_t *xml ,char *name,char *node_val);

void change_upxml(struct xmlVal *val);

#endif // XMLGET_H_INCLUDED
