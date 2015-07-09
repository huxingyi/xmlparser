Overview
=========
a tiny xml parser

usage:

```c
#include "xmlparser.h"
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

License(MIT)
=============
Copyright (c) 2010, huxingyi@msn.com

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
