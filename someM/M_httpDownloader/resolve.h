#ifndef RESOLVE_H_INCLUDED
#define RESOLVE_H_INCLUDED

int str_resolve(char *url_name,char *host_name,char *path_filename,ushort *port);
int port_resolve(char *start,char *strend,ushort *port);

#endif // RESOLVE_H_INCLUDED
