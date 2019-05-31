#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double result = 0.0;

void removeSpaces(char* str)
{
	char* p = --str;

	do
		while (isspace(*++p));
	while (*++str = *p);
}

char* calcExpression(char* str, char* op)
{
	char* l = op;
	char* r = op;

	while ((isdigit(*--l) || *l == '.') && l >= str);
	if (*++r == '-')
		++r;
	while ((isdigit(*r) || *r == '.') && *r++);

	if (l == str && *l == '-')
		--l;

	char* lstr = malloc(op - l);
	memcpy(lstr, ++l, op - l);
	lstr[(op - l) / sizeof(char)] = '\0';

	char* rstr = malloc(r - op);
	memcpy(rstr, ++op, r - op);
	rstr[(r - op) / sizeof(char)] = '\0';

	char* res = malloc(sizeof(char) * 20);
	int len;

	switch (*--op)
	{
	case '/':
		if (atof(rstr) == 0)
			return NULL;

		len = sprintf(res, "%.3f", atof(lstr) / atof(rstr));
		break;
	case '*':
		len = sprintf(res, "%.3f", atof(lstr) * atof(rstr));
		break;
	case '-':
		len = sprintf(res, "%.3f", atof(lstr) - atof(rstr));
		break;
	case '+':
		len = sprintf(res, "%.3f", atof(lstr) + atof(rstr));
		break;
	}

	free(lstr);
	free(rstr);

	if (len < 0)
	{
		free(res);
		return NULL;
	}

	char* sptr = str;

	int size = sizeof(char) * strlen(sptr) - ((r - l) - len * sizeof(char));

	char* newstr = malloc(size);
	char* nstr = newstr, *rptr = res;

	while (sptr < l)
		*nstr++ = *sptr++;
	while (*res)
		*nstr++ = *res++;
	while (*r)
		*nstr++ = *r++;

	newstr[size] = '\0';

	free(rptr);

	return newstr;
}

char* stringToExpression(char* str)
{
	char* div = strstr(str, "/");
	char* mul = strstr(str, "*");

	char* res;

	if (mul)
	{
		if (div && div < mul)
			res = calcExpression(str, div);
		else
			res = calcExpression(str, mul);
	}
	else if (div)
		res = calcExpression(str, div);
	else
	{
		char* add = strstr(str, "+");
		char* sub = strstr(str, "-");

		if (sub)
		{
			if (sub == str)
				sub = strstr(++sub, "-");

			if (sub)
			{
				if (add && add < sub)
					res = calcExpression(str, add);
				else
					res = calcExpression(str, sub);
			}
			else if (add)
				res = calcExpression(str, add);
			else
				return str;

		}
		else if (add)
			res = calcExpression(str, add);
		else
			return str;
	}

	if (!res)
		return NULL;

	return stringToExpression(res);
}

int findScopes(char* str)
{
	char* r, *l;
	if (r = strstr(str, ")"))
	{
		l = r;

		while (*--l != '(' && l > str);
		if (*l != '(')
			return -1;

		char* substr = malloc(r - l);
		memcpy(substr, ++l, r - l);
		substr[(r - l) / sizeof(char)] = '\0';

		char* res = stringToExpression(substr);
		if (!res)
		{
			free(str);
			free(substr);
			return -1;
		}

		char* sptr = str, *rptr = res;

		int size = sizeof(char) * strlen(sptr) - ((r - --l) - strlen(res) * sizeof(char));
		char* nstr = malloc(size);
		char* n = nstr;

		while (sptr < l)
			*nstr++ = *sptr++;
		while (*res)
			*nstr++ = *res++;
		while (*r)
			*nstr++ = *++r;

		n[size] = '\0';

		free(substr);
		free(rptr);

		return findScopes(n);
	}
	else if (strstr(str, "("))
		return -1;

	char* res = stringToExpression(str);

	if (!res)
		return -1;

	result = atof(res);

	free(res);

	return 0;
}

int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 2)
		return -1;

	char* s = argv[1];

	removeSpaces(s);

	if (findScopes(s) == -1)
		return -1;

	printf("%.3f\n", result);

	return 0;
}
