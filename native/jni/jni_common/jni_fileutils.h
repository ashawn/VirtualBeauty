/*
 * Copyright (c) 2016 cvte
 * Create by cvte 
 */

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

/*
 *     Author : cvte
 * Created on : 07/22 2016
 *
 * Copyright (c) 2016 cvte. All rights reserved.
 */
namespace jniutils {

bool fileExists(const char* name);

bool dirExists(const char* name);

bool fileExists(const std::string& name);

bool dirExists(const std::string& name);

}  // end jniutils
   /* FILEUTILS_H */
