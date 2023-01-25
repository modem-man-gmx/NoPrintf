//#include <stdio.h>

#include "no_printf.hpp"

#include "lightest/lightest.h"
//#define __FILE__ "test.cpp"

CONFIG(Configurations) {
  for (; argn > 0; argn--, argc++)
  {
    if (std::string(*argc) == "--no-color") NO_COLOR();
    if (std::string(*argc) == "--no-output") NO_OUTPUT();
  }
  // NO_COLOR();
  // NO_OUTPUT();
}

TEST(TestTimerMacros)
{
  int i = 0;
  REQ(TIMER(i++), >=, 0);
  REQ(AVG_TIMER(i++, 100), >=, 0);
}

TEST(TestAssertionMacros)
{
  REQ(1, ==, 1);
  REQ(1.0, ==, 2.0/2);  // Test fail
  int a = 0, b = 0, c = 1;
  REQ(a, ==, b);
  REQ(b, !=, c);
  REQ(a, >, b-1); // Test fail
  MUST(REQ(a, ==, c-1));  // FAIL & stop this test
}

TEST(TestSub)
{
  int a = 1;
  SUB(SubTest1) { REQ(a, ==, 1); };

  SUB(SubTest2)
  {
    REQ(a, ==, 2/2);  // Test fail
  };

  SUB(SubTest3)
  {
    SUB(SubInSubTest) { REQ(a, ==, 1); };
  };
}

// To test DATA
DATA(GetFailedTests)
{
  std::cout << "-----------------------------" << std::endl;
  std::cout << "Failed tests:" << std::endl;
  data->IterSons( [] (const lightest::Data* item )
    {
    const lightest::DataSet* test = static_cast<const lightest::DataSet*>(item);
    if (test->GetFailed())
    {
      std::cout << " * " << test->GetName() << std::endl;
    }
    }
  ); // lambda-end
  std::cout << "-----------------------------" << std::endl;
}
