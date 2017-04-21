#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_FORLOOP_FUNC_BUMP6.h"

MBL_FORLOOP_FUNC_BUMP6::MBL_FORLOOP_FUNC_BUMP6(void)
{
	cKeyLiteral = NULL;
	cWKeyLiteral = NULL;

	srand(time(NULL));
	for (int i = 0; i < 8; i++)
		cKey[i] = (unsigned char)(rand() % 225 + 15); //+1 so we don't XOR with 0

	for (int i = 0; i < 8; i++)
		wcKey[i] = (wchar_t)(rand() % 65505 + 15); //+1 so we don't XOR with 0

	//create key literals
	CreateStringLiteralA(cKey, 8, cKeyLiteral);
	CreateStringLiteralW(wcKey, 8, cWKeyLiteral);
}

MBL_FORLOOP_FUNC_BUMP6::~MBL_FORLOOP_FUNC_BUMP6(void)
{
	//free key literals
	if (cKeyLiteral)
		free(cKeyLiteral);

	if (cWKeyLiteral)
		free(cWKeyLiteral);
}

int MBL_FORLOOP_FUNC_BUMP6::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		wcToScramble[i] += wcKey[i % 8];

	return 1;
}

int MBL_FORLOOP_FUNC_BUMP6::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		cToScramble[i] += cKey[i % 8];

	return 1;
}

int MBL_FORLOOP_FUNC_BUMP6::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"char c%sMarbleBumpKey[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_BUMP6D(%s, %d, c%sMarbleBumpKey);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + strlen(cStringLiteral) + (strlen(cVarName) * 4) + strlen(cKeyLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cKeyLiteral, cVarName, iNumOfChars, cVarName);

	return 1;
}

int MBL_FORLOOP_FUNC_BUMP6::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"wchar_t wc%sMarbleBumpKey[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_BUMP6D(%s, %d, wc%sMarbleBumpKey);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + strlen(cStringLiteral) + (strlen(cVarName) * 4) + strlen(cWKeyLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cWKeyLiteral, cVarName, iNumOfChars, cVarName);

	return 1;
}