#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/cJSON.h"

void manifest();
void transform(char from[], char to[]);
void transformTemplate(char to[]);

int main(int argc, char **argv)
{
  if (argc == 1)
  {
    fprintf(stderr, "%s", "No arguments found\n");
    return 1;
  }

  char *action = argv[1];

  if (!strcmp(action, "manifest"))
    manifest();
  else if (!strcmp(action, "transform"))
    transform(argv[2], argv[3]);
  else
    fprintf(stderr, "%s%s\n", "Invalid action: ", action);
  return 0;
}

void manifest()
{

  cJSON *output = cJSON_CreateObject();

  cJSON *name = cJSON_CreateString("template");
  cJSON *version = cJSON_CreateString("0.1");
  cJSON *description = cJSON_CreateString("This is a template package.");

  cJSON_AddItemToObject(output, "name", name);
  cJSON_AddItemToObject(output, "version", version);
  cJSON_AddItemToObject(output, "description", description);

  cJSON *transforms = cJSON_CreateArray();
  cJSON *templateTransform = cJSON_CreateObject();

  cJSON *templateFrom = cJSON_CreateString("template");
  cJSON *templateTo = cJSON_CreateArray();
  cJSON_AddItemToArray(templateTo, cJSON_CreateString("html"));
  cJSON *templateArguments = cJSON_CreateArray();

  cJSON_AddItemToObject(templateTransform, "from", templateFrom);
  cJSON_AddItemToObject(templateTransform, "to", templateTo);
  cJSON_AddItemToObject(templateTransform, "arguments", templateArguments);

  cJSON_AddItemToArray(transforms, templateTransform);

  cJSON_AddItemToObject(output, "transforms", transforms);

  printf("%s\n", cJSON_Print(output));
}

void transform(char from[], char to[])
{

  if (!strcmp(from, "template"))
    transformTemplate(to);
  else
    fprintf(stderr, "%s%s\n", "Package does not support: ", from);
}

void transformTemplate(char to[])
{

  char in[10000] = "";
  char buffer[100];

  while (fgets(buffer, sizeof(buffer), stdin))
    strcat(in, buffer);

  cJSON *input = cJSON_Parse(in);
  cJSON *data = cJSON_GetObjectItem(input, "data");

  if (!strcmp(to, "html"))
  {
    cJSON *start = cJSON_CreateObject();
    cJSON *startName = cJSON_CreateString("raw");
    cJSON *startData = cJSON_CreateString("<a>");
    cJSON_AddItemToObject(start, "name", startName);
    cJSON_AddItemToObject(start, "data", startData);

    cJSON *text = cJSON_CreateObject();
    cJSON *contentName = cJSON_CreateString("raw");
    cJSON *contentData = cJSON_CreateString(data->valuestring);
    cJSON_AddItemToObject(text, "name", contentName);
    cJSON_AddItemToObject(text, "data", contentData);

    cJSON *end = cJSON_CreateObject();
    cJSON *endName = cJSON_CreateString("raw");
    cJSON *endData = cJSON_CreateString("</a>");
    cJSON_AddItemToObject(end, "name", endName);
    cJSON_AddItemToObject(end, "data", endData);

    cJSON *output = cJSON_CreateArray();
    cJSON_AddItemToArray(output, start);
    cJSON_AddItemToArray(output, text);
    cJSON_AddItemToArray(output, end);

    printf("%s\n", cJSON_Print(output));
  }
  else
    fprintf(stderr, "%s%s\n", "Cannot convert template to: ", to);
}