#include <gtest/gtest.h>
#include <ModbusCrc16.h>

class ModbusCrc16Suite : public ::testing::Test
{
protected:
};

TEST_F(ModbusCrc16Suite, WriteFileRecord_single1)
{
    EXPECT_EQ(1, 1);
}
