#include "xmlparser.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#pragma warning(disable:4013)
#endif

int xml_parse(char* xml,                
    void* param,                 
    xmlonopentag on_opentag,         
    xmlonclosetag on_closetag,        
    xmlonattribute on_attribute,     
    xmlonendattribute on_endattribute, 
    xmlontext on_text)            
{
    int status = XML_TEXT;
    char tmp = 0;
    char tmp2 = 0;
    int callback_return = 0;
    char* attr_name_start = 0;
    char* attr_name_stop = 0;
    char* attr_value_start = 0;
    char* attr_value_stop = 0;
    char* opentag_start = 0;
    char* opentag_stop = 0;
    char* closetag_start = 0;
    char* closetag_stop = 0;
    char* text_start = 0;
    char* text_stop = 0;
    int depth = 0;
    char* p = xml;
    int moved = 0;
    
    //fapdebug("xml_parse.\n");
    
    if (0 == p)
    {
        return XMLERR_PARAM;
    }
    
    while (0 != p && 
        '\0' != (*p))
    {
        //printf("%c ", *p);

        moved = 0;

        switch (*p)
        {
        case '<': // <? <foo </foo <!-- <![CDATA[
            {
                if (XML_TEXT == status)
                {
                    if (0 == strncmp(p, 
                        XML_CDATA_OPEN_TAG, 
                        strlen(XML_CDATA_OPEN_TAG))) // <![CDATA[
                    {
                        status = XML_CDATA;
                        p += strlen(XML_CDATA_OPEN_TAG);
                        moved = 1;
                        text_start = p;
                    }
                    else if (0 == strncmp(p, 
                        XML_COMMENT_OPEN_TAG, 
                        strlen(XML_COMMENT_OPEN_TAG))) // <!--
                    {
                        status = XML_COMMENT;
                        p += strlen(XML_COMMENT_OPEN_TAG);
                        moved = 1;
                    }
                    else if (0 == strncmp(p, 
                        XML_TEXT_CLOSE_TAG, 
                        strlen(XML_TEXT_CLOSE_TAG))) // </foo
                    {
                        status = XML_CLOSETAG_NAME;
                        text_stop = p;
                        if (0 != text_start && 0 != text_stop)
                        {
                            tmp = *text_stop;
                            *text_stop = '\0';
                            callback_return = on_text(param, 
                                depth, 
                                text_start);
                            *text_stop = tmp;
                            if (0 != callback_return)
                            {
                                return callback_return;
                            }
                            text_start = 0;
                        }
                        text_stop = 0;
                        p += strlen(XML_TEXT_CLOSE_TAG);
                        moved = 1;
                        closetag_start = p;
                    }
                    else if (0 == strncmp(p, 
                        XML_HEAD_OPEN_TAG, 
                        strlen(XML_HEAD_OPEN_TAG))) // <?
                    {
                        status = XML_OPENTAG_NAME;
                        p += strlen(XML_HEAD_OPEN_TAG);
                        moved = 1;
                        opentag_start = p;
                    }
                    else // <foo
                    {
                        status = XML_OPENTAG_NAME;
                        opentag_start = p + 1;
                    }
                }
            }
            break;
        case '?': // ?>
        case '/': // />
            {
                if (XML_ATTRIBUTE_PRE == status)
                {
                    if (0 != (p+1) && '>' == (*(p+1)))
                    {
                        callback_return = on_endattribute(param, 
                            depth);
                        if (0 != callback_return)
                        {
                            return callback_return;
                        }
                        status = XML_TEXT;
                        p += 2;
                        moved = 1;
                        callback_return = on_closetag(param, 
                            depth, 
                            "");
                        if (0 != callback_return)
                        {
                            return callback_return;
                        }
                        --depth;
                    }
                }
                else if (XML_OPENTAG_NAME == status)
                {
                    if (0 != (p+1) && '>' == (*(p+1)))
                    {
                        status = XML_TEXT;
                        opentag_stop = p;
                        p += 2;
                        moved = 1;
                        if (0 == opentag_start || 
                            0 == opentag_stop)
                        {
                            return XMLERR_OPENTAG;
                        }
                        ++depth;
                        tmp = *opentag_stop;
                        *opentag_stop = '\0';
                        callback_return = on_opentag(param, 
                            depth, 
                            opentag_start);
                        *opentag_stop = tmp;
                        if (0 != callback_return)
                        {
                            return callback_return;
                        }
                        opentag_start = 0;
                        opentag_stop = 0;
                        callback_return = on_endattribute(param, 
                            depth);
                        if (0 != callback_return)
                        {
                            return callback_return;
                        }
                        callback_return = on_closetag(param, 
                            depth, 
                            "");
                        if (0 != callback_return)
                        {
                            return callback_return;
                        }
                        --depth;
                    }
                    else
                    {
                        return XMLERR_OPENTAG;
                    }
                }
            }
            break;
        case '>':
            {
                if (XML_ATTRIBUTE_PRE == status)
                {
                    callback_return = on_endattribute(param, 
                        depth);
                    if (0 != callback_return)
                    {
                        return callback_return;
                    }
                    status = XML_TEXT;
                    text_start = p + 1;
                }
                else if (XML_CLOSETAG_NAME == status)
                {
                    status = XML_TEXT;
                    closetag_stop = p;
                    if (0 == closetag_start || 
                        0 == closetag_stop)
                    {
                        return XMLERR_CLOSETAG;
                    }
                    tmp = *closetag_stop;
                    *closetag_stop = '\0';
                    callback_return = on_closetag(param, 
                        depth, 
                        closetag_start);
                    *closetag_stop = tmp;
                    if (0 != callback_return)
                    {
                        return callback_return;
                    }
                    --depth;
                    closetag_start = 0;
                    closetag_stop = 0;
                }
                else if (XML_OPENTAG_NAME == status)
                {
                    status = XML_TEXT;
                    text_start = p + 1;
                    opentag_stop = p;
                    if (0 == opentag_start || 
                        0 == opentag_stop)
                    {
                        return XMLERR_OPENTAG;
                    }
                    ++depth;
                    tmp = *opentag_stop;
                    *opentag_stop = '\0';
                    callback_return = on_opentag(param, 
                        depth, 
                        opentag_start);
                    *opentag_stop = tmp;
                    if (0 != callback_return)
                    {
                        return callback_return;
                    }
                    opentag_start = 0;
                    opentag_stop = 0;
                    callback_return = on_endattribute(param, 
                        depth);
                    if (0 != callback_return)
                    {
                        return callback_return;
                    }
                }
            }
            break;
        case '"':
            {
                if (XML_ATTRIBUTE_EQUAL == status)
                {
                    status = XML_ATTRIBUTE_VALUE;
                    attr_value_start = p + 1;
                }
                else if (XML_ATTRIBUTE_VALUE == status)
                {
                    status = XML_ATTRIBUTE_PRE;
                    attr_value_stop = p;
                    if (0 == attr_name_start
                        || 0 == attr_name_stop
                        || 0 == attr_value_start
                        || 0 == attr_value_stop)
                    {
                        return XMLERR_ATTRIBUTE;
                    }
                    tmp = *attr_name_stop;
                    tmp2 = *attr_value_stop;
                    *attr_name_stop = '\0';
                    *attr_value_stop = '\0';
                    callback_return = on_attribute(param, 
                        depth, 
                        attr_name_start, 
                        attr_value_start);
                    *attr_name_stop = tmp;
                    *attr_value_stop = tmp2;
                    if (0 != callback_return)
                    {
                        return callback_return;
                    }
                    attr_name_start = 0;
                    attr_name_stop = 0;
                    attr_value_start = 0;
                    attr_value_stop = 0;
                }
            }
            break;
        case '-': // --> 
            {
                if (XML_COMMENT == status)
                {
                    if (0 == strncmp(p, 
                        XML_COMMENT_CLOSE_TAG, 
                        strlen(XML_COMMENT_CLOSE_TAG)))
                    {
                        status = XML_TEXT;
                        p += strlen(XML_COMMENT_CLOSE_TAG);
                        moved = 1;
                    }
                }
            }
            break;
        case ']': // ]]>
            {
                if (XML_CDATA == status)
                {
                    if (0 == strncmp(p, 
                        XML_CDATA_CLOSE_TAG, 
                        strlen(XML_CDATA_CLOSE_TAG)))
                    {
                        status = XML_TEXT;
                        text_stop = p;
                        if (0 != text_start && 
                            0 != text_stop)
                        {
                            tmp = *text_stop;
                            *text_stop = '\0';
                            callback_return = on_text(param, 
                                depth, 
                                text_start);
                            *text_stop = tmp;
                            if (0 != callback_return)
                            {
                                return callback_return;
                            }
                            text_start = 0;
                        }
                        text_stop = 0;
                        p += strlen(XML_CDATA_CLOSE_TAG);
                        moved = 1;
                    }
                }
            }
            break;
        case '=':
            {
                if (XML_ATTRIBUTE_NAME == status)
                {
                    status = XML_ATTRIBUTE_EQUAL;
                    if (0 == attr_name_stop)
                    {
                        attr_name_stop = p;
                    }
                }
            }
            break;
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            {
                if (XML_ATTRIBUTE_NAME == status)
                {
                    if (0 == attr_name_stop)
                    {
                        attr_name_stop = p;
                    }
                }
                else if (XML_OPENTAG_NAME == status)
                {
                    status = XML_ATTRIBUTE_PRE;
                    opentag_stop = p;
                    if (0 == opentag_start || 
                        0 == opentag_stop)
                    {
                        return XMLERR_OPENTAG;
                    }
                    ++depth;
                    tmp = *opentag_stop;
                    *opentag_stop = '\0';
                    callback_return = on_opentag(param, 
                        depth, 
                        opentag_start);
                    *opentag_stop = tmp;
                    if (0 != callback_return)
                    {
                        return callback_return;
                    }
                    opentag_start = 0;
                    opentag_stop = 0;

                }
            }
            break;
        default:
            {
                
                if (XML_ATTRIBUTE_PRE == status)
                {
                    if (xml_isname(*p))
                    {
                        status = XML_ATTRIBUTE_NAME;
                        attr_name_start = p;
                    }
                }
            }
        }
        if (!moved)
        {
            ++p;
        }
    }

    if (0 != depth)
    {
        return XMLERR_UNMATCH;
    }

    return XMLERR_OK;
}
