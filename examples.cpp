/*  example.cpp
 *
 *  An Example file showing use case for LogSystem
 *  
 *  Created by Langyan
 *  Last edit: 2024-1-7
 *
 */
#include "LogSystem.h"

#define MY_INFO(s) LOG("MY Customized INFO: " s, "1;3;7;", "106;33")

int main(){
  INFO("LogSystem Built by 朗彦");
  TRACE("Example Trace.");
  WARN("Program is Warning!");
  ERROR("Program runs into trouble!");
  DEBUG("debug here.");

  LOG("Cutomize your log with this", "1;3;7;","106;31");

  MY_INFO("你好");


  std::cout << "Nice Literal"_cyan << std::endl;
}
