#include <iostream>
#include <fstream>

#include <cstdlib>

#include <sstream>

#include <thread>
#include <chrono>

#include <vector>

#include <nlohmann/json.hpp>

#include "time.h"
#include "float.h"
#include "f_read.hpp"
// #define RANGE 30
// #define ROUNDS 100000

using namespace std::chrono_literals;
using json = nlohmann::json;

int range = 0;
int rounds = 0;
int totalcount = 0;
int starttime = time(NULL);
bool _ex = false;

std::vector<std::string> messageque;

void load_config()
{
  json config = "[500, 10000]"_json;
  try
  {
    std::string res;
    f_read("config", res);
    config = json::parse(res);
  }
  catch (nlohmann::detail::type_error)
  {
    config = "[50, 100]"_json;
    exit(1);
  }
  range = config[0];
  rounds = config[1];
}

int randonum()
{
  return (rand() % range) + 1;
}

int m1_count = 0;
int m2_count = 0;

void _status()
{
  while (true)
  {
    totalcount = m1_count + m2_count;
    if (_ex || totalcount > rounds)
    {
      _ex = 1;
      std::cout << std::endl
                << "m1_count " << m1_count << std::endl
                << "m2_count " << m2_count << std::endl;
      exit(0);
    }
    for (int c = 0; c < messageque.size(); c++)
    {
      std::string msg = messageque[c];
      messageque.pop_back();
      // delete messageque[c];
      std::cout << msg << std::endl;
    }
    std::stringstream ss;
    std::string rnds;
    ss << rounds;
    ss >> rnds;
    int _maxspace = (rnds.length() * 2) + 13;
    auto minutes = std::chrono::minutes(int((time(NULL) - starttime) / 60));
    auto seconds = std::chrono::seconds(int((time(NULL) - starttime) - (60 * minutes.count())));

    ss.clear();
    ss << seconds.count();
    std::string _sec;
    ss >> _sec;

    ss.clear();
    ss << minutes.count();
    std::string _min;
    ss >> _min;

    if (_sec.length() < 2)
    {
      _sec = "0" + _sec;
    }
    if (_min.length() < 2)
    {
      _min = "0" + _min;
    }
    std::string perc;
    ss.clear();
    ss << "[" << totalcount << "/" << rounds << "]";
    ss >> perc;

    std::string time = _min + ":" + _sec;
    std::string ftout = "\r\x1b[7m; " + time + " | " + perc + "\x1b[0m";
    std::cout << ftout;
    std::cout.flush();
  }
}

int main()
{
  srand(time(NULL));
  load_config();
  std::stringstream ss;
  std::string rnds;
  ss << rounds;
  ss >> rnds;

  std::thread t(_status);
  t.detach();

  int _maxspace = (rnds.length() * 2) + 13;

  std::string spacing = "";
  for (int a = 0; a < _maxspace; a++)
    spacing.push_back(' ');

  try
  {
    int m1_result;
    int m2_result;
    int m2_int2;
    while (true)
    {
      if (_ex)
      {
        exit(0);
      }
      //method one
      int m1_int1 = randonum();
      int m1_int2 = randonum();

      m1_result = (m1_int1 == m1_int2 ? m1_int1 : 0);

      //method two
      if (!m2_int2 || m2_int2 > range)
        m2_int2 = randonum();

      int m2_int1 = randonum();

      m2_result = 0;
      if (m2_int1 == m2_int2)
      {
        m2_result = m2_int1;
        m2_int2 = randonum();
      }

      // std::cout << "m2_in1, m2_int2 | " << m2_int1 << ", " << m2_int2 << std::endl;

      //end
      if (m1_result != 0)
      {
        ss.clear();
        std::string m1r;
        ss << m1_result;
        ss >> m1r;
        ss.clear();
        messageque.push_back("\r" + spacing + "\rm1_result " + m1r);
        // std::cout << "\r" << spacing << "\rm1_result " << m1_result << std::endl;
        m1_count++;
        std::this_thread::sleep_for(10ms);
      }
      if (m2_result != 0)
      {
        ss.clear();
        std::string m2r;
        ss << m2_result;
        ss >> m2r;
        ss.clear();
        messageque.push_back("\r" + spacing + "\rm2_result " + m2r);
        // std::cout << "\r" << spacing << "\rm2_result " << m2_result << std::endl;
        m2_count++;
        std::this_thread::sleep_for(10ms);
      }
    }
  }
  catch (...)
  {
    _ex = 1;
    exit(0);
  }
}