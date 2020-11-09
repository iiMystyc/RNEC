#include <fstream>

template <typename T>
void log(T data)
{
  std::cout << data << std::endl;
}

void f_read(std::string fname, std::string &out)
{
  try
  {
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;

    pFile = fopen(fname.c_str(), "rb");
    if (pFile == NULL)
    {
      log("Error with the file?? " + fname);
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char *)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
      log("Error allocating memory to read file " + fname);
    }

    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize)
    {
      log("Error reading file " + fname);
    }

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose(pFile);
    out = buffer;
  }
  catch (std::exception)
  {
    // std::cout << "Error occured while reading file \"" << fname << "\"" << std::endl;
    log("Error occured while reading file \"" + fname + "\"");
  }
}