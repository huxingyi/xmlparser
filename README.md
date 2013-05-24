xmlparser
=========

a tiny xml parser

usage:

```c
#include <xmlparser.h>
#include <stdlib.h>
#include <stdio.h>

int on_opentag(void* param, 
    int depth, 
    char* name)
{
    printf("xmlonopentag>>%s\r\n",
        name);
    return 0;
}
    
int on_closetag(void* param, 
    int depth, 
    char* name)
{
    printf("xmlonclosetag>>%s\r\n",
        name);
    return 0;
}

int on_attribute(void* param, 
    int depth, 
    char* name, 
    char* value)
{
    printf("xmlonatrribute>>%s=%s\r\n",
        name,
        value);
    return 0;
}

int on_endattribute(void* param, 
    int depth)
{
    printf("xmlonendattribute>>\r\n");
    return 0;
}

int on_text(void* param, 
    int depth, 
    char* text)
{
    printf("xmlontext>>%s\r\n",
        text);
    return 0;
}

static char s_xml[] = {
    "<xml>"
    "    <tag name=\"helloworld\">"
    "        HELLO WORLD!!!"
    "    </tag>"
    "</xml>"
};

int main(int argc,
    char *argv[])
{
    int parseerr = xml_parse(s_xml,
        0,
        on_opentag, 		
        on_closetag,		
        on_attribute, 	
        on_endattribute, 
        on_text);
    return parseerr;
}
```
