/* Copyright (c) 2010, huxingyi@msn.com
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

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

/* When matched <tagname/> the name's value is empty. */
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
