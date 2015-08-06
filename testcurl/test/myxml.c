#include <stdio.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "restful.h"


void trim(char *str)
{
    int i;

    for (i = strlen(str) - 1; 0 <= i; i--) {
        if ((str[i] == '\r') ||
            (str[i] == '\n') ||
            (str[i] == ' ')) {
            str[i] = '\0';
        }
        else {
            break;
        }
    }
}

void print_attr(xmlAttrPtr attr, FILE *output)
{
    xmlAttrPtr cur_attr = NULL;
    for (cur_attr = attr; cur_attr; cur_attr = cur_attr->next) {
        fprintf(output, "attr %s=%s\n", attr->name, attr->children->content);
    }
}

void print_nodes(xmlNodeSetPtr nodes, FILE *output)
{
    xmlNodePtr cur;
    int        size;
    int        i;
    char      *content;
    char       buf[128];

    size = (nodes) ? nodes->nodeNr : 0;
    fprintf(output, "Result (%d nodes):\n", size);
    for(i = 0; i < size; ++i) {
        if(nodes->nodeTab[i]->type == XML_NAMESPACE_DECL) {
            xmlNsPtr ns;

            ns = (xmlNsPtr)nodes->nodeTab[i];
            cur = (xmlNodePtr)ns->next;
            if(cur->ns) {
                fprintf(output,
                        "= namespace \"%s\"=\"%s\" for node %s:%s",
                        ns->prefix,
                        ns->href, cur->ns->href, cur->name);
            }
            else {
                fprintf(output,
                        "= namespace \"%s\"=\"%s\" for node %s",
                        ns->prefix, ns->href, cur->name);
            }
        }
        else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
            cur = nodes->nodeTab[i];
            if(cur->ns) {
                fprintf(output,
                        "= element node \"%s:%s\"",
                        cur->ns->href, cur->name);
            }
            else {
                fprintf(output, "= element node \"%s\"",
                        cur->name);
            }
        }
        else {
            cur = nodes->nodeTab[i];
            fprintf(output, "= node \"%s\": type %d: ", cur->name, cur->type);
        }

        content = cur->children->content;
        if (NULL != content) {
            strcpy(buf, content);
            trim(buf);
            fprintf(output, " content %s\n", buf);
        }
        else {
            fprintf(output, " content (null)\n");
        }
        print_attr(cur->properties, output);
    }
}
int samuel_exec(xmlDocPtr doc, xmlChar* xpath_expr)
{
    xmlXPathContextPtr xpath_context = xmlXPathNewContext(doc);
    xmlXPathObjectPtr xpath_obj = xmlXPathEvalExpression(xpath_expr, xpath_context);
    if (NULL == xpath_obj) {
        printf("fuck parsing fail\n");
        xmlXPathFreeContext(xpath_context);
    }
    xmlXPathFreeObject(xpath_obj);
    xmlXPathFreeContext(xpath_context);
    printf("yes parsing fail\n");
    return 0;
}

int execute_xpath(xmlDocPtr doc, xmlChar *xpath_expr)
{
    printf("samuel\n");
    printf("successful in execute_xpath\n");
    printf("samuel\n");
    xmlXPathContextPtr xpath_context;
    xmlXPathObjectPtr  xpath_obj;

    printf("samuel\n");
    printf("---- XPath %s\n", xpath_expr);
    printf("samuel\n");

    /* Create xpath evaluation context */
    xpath_context = xmlXPathNewContext(doc);
    if(xpath_context == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        return -1;
    }
    printf("samuel\n");
    printf("successful create xpath_context\n");
    printf("samuel\n");

    /* Evaluate xpath expression */
    xpath_obj = xmlXPathEvalExpression(xpath_expr, xpath_context);
    if(xpath_obj == NULL) {
        fprintf(stderr,
                "Error: unable to evaluate xpath expression \"%s\"\n",
                xpath_expr);
        xmlXPathFreeContext(xpath_context);
        xmlFreeDoc(doc);
        return -1;
    }

    printf("samuel\n");
    printf("successful get xpath_obj\n");
    printf("samuel\n");

    /* Print results */
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    print_nodes(xpath_obj->nodesetval, stdout);
    printf("samuel\n");
    printf("successful print xpath_obj\n");
    printf("samuel\n");

    /* Cleanup */
    xmlXPathFreeObject(xpath_obj);
    xmlXPathFreeContext(xpath_context);
}

int main(int argc, char **argv)
{
    const char* url = "http://www.plurk.com/samuelololol";

    CURL *curl;
    RESTFUL_STRING *noise;
    RESTFUL_STRING_INIT(&noise);
    // init curl
    RESTFUL_INIT(&curl);
    // normal processing
    RESTFUL_GET(&curl, url, (void *)noise);
    //printf("response:\n%s", response->ptr);
    //const char* noise = "<node/>";


    xmlDocPtr  doc;
    xmlNodePtr root_element = NULL;
    int        ret;

    LIBXML_TEST_VERSION;

    /* Read document */
    //doc = xmlReadFile(input_file, NULL, 0);
    //doc = xmlReadMemory(noise, strlen(noise), NULL, NULL, 0);
    doc = htmlReadMemory(noise->ptr, noise->len, NULL, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", noise->ptr);
        return -1;
    }
    printf("samuel\n");
    printf("successful get doc\n");
    printf("samuel\n");

    /* Like getElementTagName() */
    execute_xpath(doc, (xmlChar *)"//div[@id='page_title']");
    //execute_xpath(doc, (xmlChar *)"//*");

    /* Free document */
    xmlFreeDoc(doc);
    xmlCleanupParser();
    RESTFUL_STRING_DESTROY(&noise);
    RESTFUL_CLEANUP(&curl);
    return 0;
}
