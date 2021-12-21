#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* IN = "in";
const char* IN2 = "input";

const char* MOD1 = "module";
const char* MOD2 = "module!";
const char* MOD3 = "module*";
const char* MOD4 = "mod";
const char* MOD5 = "mod!";
const char* MOD6 = "mod*";
const char* MOD7 = "make";
const char* MOD8 = "view";
const char* MOD9 = "ipsgopen";
const char* MOD10 = "open";

const char* CMT1 = "--";
const char* CMT2 = "-->";
const char* CMT3 = "**";
const char* CMT4 = "**>";

char* strsub(const char* input, int start, int length) {
    if (strlen(input) <= start)
        return "";
    if (strlen(input) - start < length)
        length = strlen(input) - start;
    char* strRe;
    strRe = (char *) malloc(length + 1);
    memcpy(strRe, &input[start], length);
    strRe[length] = '\0';
    return strRe;
}

char* strsub2(const char* input, int start) {
    int length = strlen(input) - start;
    char* temp = strsub(input, start, length);
    return temp;
}

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s));
}

int startsWith(const char *a, const char *b)
{
    if(strncmp(a, b, strlen(b)) == 0)
        return 1;
    return 0;
}

char* strmycat(const char* str1, const char* str2) {
    char* re;
    int length = strlen(str1) + strlen(str2);
    re = (char *) malloc(length + 1);
    strcpy(re, str1);
    strcat(re, str2);
    re[length] = '\0';
    return re;
}

char* removeComment(const char* line, const char* cmtSign) {
    char* temp2 = strstr(line, cmtSign);
    if (temp2 != NULL) {
        int index = strlen(line) - strlen(temp2);
        return strsub(line, 0, index);
    }
    return line;
}

int strChIndex(const char* string, const char ch) {
    char *e;
    int index;
    e = strchr(string, ch);
    if (e == NULL)
        return -1;
    index = (int)(e - string);
    return index;
}

int strChIndex2(const char* string, const char ch, int startAt) {
    char *subS = strsub2(string, startAt);
    char *e;
    int index;
    e = strchr(subS, ch);
    if (e == NULL)
        return -1;
    index = (int)(e - string);
    return index;
}

char * strrstr(char *string, char *find, ssize_t len)
{
    char *cp;
    for (cp = string + len - strlen(find); cp >= string; cp--) {
        if (strncmp(cp, find, strlen(find)) == 0)
            return cp;
    }
    return NULL;
}

int strStrLastIndex(const char* string, const char* strToFind) {
    char *e;
    int index;
    e = strrstr(string, strToFind, strlen(string));
    if (e == NULL)
        return -1;
    index = (int)(e - string);
    return index;
}

char* addScapeChars(const char* line, char scapeChar) {
    char* res = line;
    int previous = 0;
    int pos = strChIndex(res, scapeChar);
    int last = strStrLastIndex(res," ->");
    while (pos != -1 && pos < last){
        res = strmycat(strmycat(strsub(res, previous, pos), "`"), strsub2(res, pos));
        pos = strChIndex2(res, scapeChar, pos + 2);
    }
    return res;
}

char* handleScapeChars(const char* line){
    char* res = trim(line);
    if (startsWith(res, "op") ||
        startsWith(res, "ops") ||
        startsWith(res, "bop") ||
        startsWith(res, "bops")) {
        res = addScapeChars(res, ',');
        res = addScapeChars(res, '(');
        res = addScapeChars(res, ')');
        res = addScapeChars(res, '{');
        res = addScapeChars(res, '}');
        res = addScapeChars(res, '[');
        res = addScapeChars(res, ']');
    }
    if (startsWith(res, "pred")){
        res = addScapeChars(res, '(');
        res = addScapeChars(res, ')');
        res = addScapeChars(res, '{');
        res = addScapeChars(res, '}');
        res = addScapeChars(res, '[');
        res = addScapeChars(res, ']');
    }
    return res;
}

char* removeAfterEOF(char* text) {
    char *e = strstr(text, "eof");
    if (e == NULL)
        return text;
    int index = (int)(e - text);
    return strsub(text, 0, index);
}

char* processALine(const char* line, int *firstMod) {
    if (trim(line) == "")
        return "";
    char* temp1 = ltrim(line);
    if (startsWith(temp1, MOD1) ||
            startsWith(temp1, MOD2) ||
            startsWith(temp1, MOD3) ||
            startsWith(temp1, MOD4) ||
            startsWith(temp1, MOD5) ||
            startsWith(temp1, MOD6) ||
            startsWith(temp1, MOD7) ||
            startsWith(temp1, MOD8) ||
            startsWith(temp1, MOD9) ||
            startsWith(temp1, MOD10)) {
        int f2 = *firstMod;
        *firstMod = 0;
        if (f2)
            return strmycat("(", temp1);
        return strmycat(")\n\n(", temp1);
    }
    if (startsWith(temp1, CMT1) ||
            startsWith(temp1, CMT2) ||
            startsWith(temp1, CMT3) ||
            startsWith(temp1, CMT4))
        return "";
    return

            handleScapeChars(removeComment(
            removeComment(
                    removeComment(
                            removeComment(line, CMT1),
                    CMT2),
                    CMT3),
                    CMT4));
//    char* temp2 = strstr(line, "--");
//    if (strstr(line, ) != NULL)
    return line;
}

char* process(char* filePath) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char* result = "";
    int firstMod = 1;

    fp = fopen(filePath, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        char* temp = processALine(line, &firstMod);
        char* temp2 = (char *) malloc(strlen(result) + 1);
        strcpy(temp2, result);
        result = strmycat(strmycat(temp2, "\n"), temp);
    }

    fclose(fp);
    if (line)
        free(line);
    return strmycat(trim(removeAfterEOF(result)), ")");
}

int main(int argc, char* args[]) {
    FILE *fp;
    fp = fopen(args[2], "w+");
    fprintf(fp, process(args[1]));
    fclose(fp);
    return 0;
}
