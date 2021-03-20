#include "test_framework.hpp"

class ClearScreen: public TestFramework {
 public:
    ClearScreen():TestFramework("clear screen") {}
    void Step() {
        auto render = GetRenderer();
        render->SetClearColor(0, 200, 0);
        render->Clear();
    }

 private:
};

RUN_TEST(ClearScreen)
