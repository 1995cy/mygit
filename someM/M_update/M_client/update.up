#include <mxml.h>
#include <stdio.h>
#include <stdlib.h>
#include  "xmlget.h"

int getxmlval(struct xmlVal *valget){
    FILE *fp = NULL;
    mxml_node_t * xml = NULL;
    mxml_node_t * node = NULL;
    int er = 0;

    fp = fopen("update.xml", "r");
    if(fp < 0)
    {
        perror("升级配置文件已丢失");
        return -1;
    }
    xml = mxmlLoadFile(NULL,fp, MXML_TEXT_CALLBACK);
    close(fp);

   er = find_node(xml,"version",valget->version);
   if(er < 0){
        mxmlDelete(xml);
        return -1;
   }
    er = find_node(xml,"time",valget->time);
    if(er < 0){
        mxmlDelete(xml);
        return -1;
   }
   er = find_node(xml,"hash",valget->hash_md5);
   if(er < 0){
        mxmlDelete(xml);
        return -1;
   }
   er = find_node(xml,"name",valget->name);
   if(er < 0){
        mxmlDelete(xml);
        return -1;
   }

    mxmlDelete(xml);
    return 0;
}

int find_node(mxml_node_t *xml ,char *name,char *node_val){
    mxml_node_t *node = NULL;

     node = mxmlFindElement(xml,xml,name,NULL,NULL,MXML_DESCEND);
     if(node == NULL){
        perror("升级配置文件已被破坏");
        return -1;
     }
    strcpy(node_val,node->child->value.text.string);

    return 0;
}
