#ifndef _GUID_H_
#define _GUID_H_

static int g_UniqueID = 0;
static int UniqueID() { return g_UniqueID++; };

#endif //_GUID_H_