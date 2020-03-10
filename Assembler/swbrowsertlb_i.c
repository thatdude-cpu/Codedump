/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Aug 02 17:54:08 2001
 */
/* Compiler settings for P:\am\thirdptysrc\toolbox\solidworks\swbrowser\API\typelib\swbrowsertlb.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_ToolboxBrowser = {0xED783340,0xD5DB,0x11d4,{0xBD,0x5A,0x00,0xC0,0x4F,0x01,0x98,0x09}};


const IID IID_IApplication = {0xF2A5D740,0x01F6,0x4f20,{0xA1,0x61,0x8D,0x78,0xD6,0x5F,0x62,0x07}};


const IID IID_IPDMDocManager = {0x859EFC6D,0x62A4,0x4872,{0xA2,0x04,0xDF,0xDD,0xC5,0x50,0x79,0x9D}};


const IID DIID_DApplicationEvents = {0xF4E1A719,0x13DD,0x4e9f,{0xBB,0x73,0x65,0x9E,0xDE,0x0C,0xD0,0x92}};


const CLSID CLSID_Application = {0x280FD973,0xC9AF,0x4825,{0x9C,0x7D,0x40,0x8A,0xC4,0xCE,0xA1,0xA3}};


const IID DIID_DPDMDocManagerEvents = {0x5E6EC4A7,0x98E1,0x429b,{0xA8,0x4A,0xEF,0xBD,0x62,0x44,0x77,0x40}};


const CLSID CLSID_PDMDocManager = {0x463BE734,0x9E5D,0x4d80,{0xA9,0x20,0x1D,0x86,0x1D,0xC5,0x8D,0xF5}};


#ifdef __cplusplus
}
#endif

