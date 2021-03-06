/*

Copyright [2010] [Josko Nikolic]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

CODE PORTED FROM THE ORIGINAL GHOST PROJECT: http://ghost.pwner.org/

*/

#include "fileutil.h"

#include <sys/stat.h>
#include <fstream>
#include <algorithm>

#include <dirent.h>
#include <cstring>

using namespace std;

bool FileExists(const string& file)
{
  struct stat fileinfo;
  return (stat(file.c_str(), &fileinfo) == 0);
}

vector<string> FilesMatch(const string& path, const string& pattern)
{
  vector<string> Files;

  DIR* dir = opendir(path.c_str());

  if (dir == nullptr)
    return Files;

  struct dirent* dp = nullptr;

  while ((dp = readdir(dir)) != nullptr)
  {
    string Name = string(dp->d_name);
    transform(begin(Name), end(Name), begin(Name), ::tolower);

    if (Name == pattern)
    {
      Files.emplace_back(dp->d_name);
      break;
    }

    if (Name.find(pattern) != string::npos && Name != "." && Name != "..")
      Files.emplace_back(dp->d_name);
  }

  closedir(dir);

  return Files;
}

string FileRead(const string& file, uint32_t start, uint32_t length)
{
  ifstream IS;
  IS.open(file.c_str(), ios::binary);

  if (IS.fail())
  {
    Print("[UTIL] warning - unable to read file part [" + file + "]");
    return string();
  }

  // get length of file

  IS.seekg(0, ios::end);
  uint32_t FileLength = IS.tellg();

  if (start > FileLength)
  {
    IS.close();
    return string();
  }

  IS.seekg(start, ios::beg);

  // read data

  auto Buffer = new char[length];
  IS.read(Buffer, length);
  string BufferString = string(Buffer, IS.gcount());
  IS.close();
  delete[] Buffer;
  return BufferString;
}

string FileRead(const string& file)
{
  ifstream IS;
  IS.open(file.c_str(), ios::binary);

  if (IS.fail())
  {
    Print("[UTIL] warning - unable to read file [" + file + "]");
    return string();
  }

  // get length of file

  IS.seekg(0, ios::end);
  uint32_t FileLength = IS.tellg();
  IS.seekg(0, ios::beg);

  // read data

  auto Buffer = new char[FileLength];
  IS.read(Buffer, FileLength);
  string BufferString = string(Buffer, IS.gcount());
  IS.close();
  delete[] Buffer;

  if (BufferString.size() == FileLength)
    return BufferString;
  else
    return string();
}

bool FileWrite(const string& file, uint8_t* data, uint32_t length)
{
  ofstream OS;
  OS.open(file.c_str(), ios::binary);

  if (OS.fail())
  {
    Print("[UTIL] warning - unable to write file [" + file + "]");
    return false;
  }

  // write data

  OS.write(reinterpret_cast<const char*>(data), length);
  OS.close();
  return true;
}
