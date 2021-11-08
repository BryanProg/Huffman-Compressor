#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Quantity of Commands for Possible
#define SIZECOMAND 5

// Function that verifies the valid of the command typed by the user
bool ValidComand(const char*, const char**);

// Function that verifies whether the paths of the files are allowed
bool ValidPath(const char*);

// Function that compels two strings
bool Tocompare(const char*, const char*);

// Help function that helps the user to use the software
void Help();

#endif