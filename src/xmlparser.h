#ifndef XML_H
#define XML_H

typedef enum _tagXMLERR
{
    XMLERR_OK = 0,            
    XMLERR_PARAM,            
    XMLERR_OPENTAG,
    XMLERR_CLOSETAG,
    XMLERR_ATTRIBUTE,
    XMLERR_UNMATCH,
} XMLERR;

#define XML_OPENTAG_NAME        10
#define XML_CLOSETAG_NAME        20
#define XML_ATTRIBUTE_PRE        30
#define XML_ATTRIBUTE_NAME        40
#define XML_ATTRIBUTE_VALUE        50
#define XML_ATTRIBUTE_EQUAL        60
#define XML_TEXT            70
#define XML_CDATA            80
#define XML_COMMENT            90

#define XML_COMMENT_OPEN_TAG        "<!--"
#define XML_COMMENT_CLOSE_TAG    "-->"
#define XML_HEAD_OPEN_TAG        "<?"
#define XML_CDATA_OPEN_TAG        "<![CDATA["
#define XML_CDATA_CLOSE_TAG        "]]>"
#define XML_TEXT_CLOSE_TAG        "</"

#define xml_isname(_c) ( (_c) > 32 && (_c) < 122 && isalnum(_c) )

/* Callback returns 0 indicates continue to explain, returns others indicates quit. */
typedef int (*xmlonopentag)(void* param, 
    int depth, 
    char* name);
    
/* When matched <tagname/>��the name's value is empty. */
typedef int (*xmlonclosetag)(void* param, 
    int depth, 
    char* name);
    
typedef int (*xmlonattribute)(void* param, 
    int depth, 
    char* name, 
    char* value);

typedef int (*xmlonendattribute)(void* param, 
    int depth);
    
typedef int (*xmlontext)(void* param, 
    int depth, 
    char* text);

int xml_parse(char* xml,                
    void* param,                 
    xmlonopentag on_opentag,         
    xmlonclosetag on_closetag,        
    xmlonattribute on_attribute,     
    xmlonendattribute on_endattribute, 
    xmlontext on_text);            

#endif



