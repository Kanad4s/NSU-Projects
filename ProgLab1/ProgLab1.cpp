#include <iostream>
#include <gtest/gtest.h>
#include "BitArray.h"

class BitArrayArguments {
public:
    BitArray firstArg;
    BitArray secondArg;
    std::string result;
    BitArrayArguments(BitArray firstArg, BitArray secondArg, std::string result);
    BitArrayArguments(BitArray firstArg, std::string result);

};

BitArrayArguments::BitArrayArguments(BitArray firstArg, BitArray secondArg, std::string result) :
    firstArg(std::move(firstArg)),
    secondArg(std::move(secondArg)),
    result(std::move(result)) {}

BitArrayArguments::BitArrayArguments(BitArray firstArg, std::string result) :
    firstArg(std::move(firstArg)),
    result(std::move(result)) {}

TEST(BitArrayTest, TestConstructorEmpty) {
    BitArray value(sizeof(long) * 8, 0);
    ASSERT_EQ(BitArray(), value);
};

TEST(BitArrayTest, TestConstructorLong) {
    BitArray value1(1, 98769876);
    BitArray value2(3, 19);
    BitArray value3(sizeof(long) * 8, ULONG_MAX);
    ASSERT_EQ(BitArray(1, 0), value1);
    ASSERT_EQ(BitArray(3, 3), value2);
    ASSERT_EQ(BitArray(sizeof(long) * 8, 4294967295), value3);
}

TEST(BitArrayTest, TestConstructorBitArray) {
    BitArray value1(54, 4893274);
    BitArray value2(value1);
    BitArray value3;
    BitArray value4(value3);
    EXPECT_EQ(value1, value2);
    EXPECT_EQ(value3, value4);
}

TEST(BitArrayTest, TestSwap) {
    BitArray value1(543, 98769876);
    BitArray value2;
    value1.swap(value2);
    ASSERT_EQ(BitArray(), value1);
    ASSERT_EQ(BitArray(543, 98769876), value2);
}

TEST(BitArrayTest, TestResize) {
    BitArray value1(543, 98769876);
    value1.resize(100, false);
    ASSERT_EQ(BitArray(100, 98769876), value1);
    BitArray value2(32, 20);
    value2.resize(4, false);
    ASSERT_EQ(BitArray(4, 4), value2);
    BitArray value3(0, 0);
    value3.resize(32, true);
    ASSERT_EQ(BitArray(32, ULONG_MAX), value3);
    BitArray value4(0, 0);
    value4.resize(32, false);
    ASSERT_EQ(BitArray(32, 0), value4);
}

TEST(BitArrayTest, TestClear) {
    BitArray value(543, 98769876);
    value.clear();
    ASSERT_EQ(BitArray(0, 0), value);
}

TEST(BitArrayTest, TestPushBack) {
    BitArray value1(0, 0);
    value1.pushBack(true);
    ASSERT_EQ(BitArray(1, 1), value1);
    BitArray value2;
    value2.pushBack(true);
    BitArray value3(33, 0);
    value3.set(33, true);
    ASSERT_EQ(value3, value2);
    BitArray value4;
    value4.pushBack(false);
    ASSERT_EQ(BitArray(33, 0), value4);
}

class BitArrayTestCommon : public testing::Test {
public:
    BitArray emptyArray, defaultArray, fullArray, array1, array2, array3, array4, array5;
protected:
    void SetUp() override {
        emptyArray = BitArray(0, 0);
        defaultArray = BitArray();
        fullArray = BitArray(sizeof(long) * 8, ULONG_MAX);
        array1 = BitArray(sizeof(long) * 8, 0);
        array2 = BitArray(sizeof(long) * 8, 1);
        array3 = BitArray(sizeof(long) * 8, 16);
        array4 = BitArray(32, 98769876);
        array5 = BitArray(33, 654);
        array5.set();
    }
    void TearDown() override {};
};

TEST_F(BitArrayTestCommon, TestSet) {
    ASSERT_THROW(emptyArray.set(1, true), std::invalid_argument);
    ASSERT_THROW(emptyArray.set(-1, true), std::invalid_argument);
    array4.set();
    ASSERT_EQ(fullArray, array4);
    array1.set(1, true);
    ASSERT_EQ(array2, array1);
    array3.set(5, false);
    ASSERT_EQ(defaultArray, array3);
}

TEST_F(BitArrayTestCommon, TestReset) {
    ASSERT_THROW(emptyArray.reset(1), std::invalid_argument);
    ASSERT_THROW(emptyArray.reset(-1), std::invalid_argument);
    array4.reset();
    ASSERT_EQ(defaultArray, array4);
    array2.reset(1);
    ASSERT_EQ(defaultArray, array2);
    array3.reset(5);
    ASSERT_EQ(defaultArray, array3);
}

TEST_F(BitArrayTestCommon, TestAny) {
    ASSERT_TRUE(array2.any());
    ASSERT_TRUE(array4.any());
    ASSERT_TRUE(array5.any());
    ASSERT_FALSE(defaultArray.any());
    ASSERT_FALSE(emptyArray.any());
}

TEST_F(BitArrayTestCommon, TestNone) {
    ASSERT_FALSE(array2.none());
    ASSERT_FALSE(array4.none());
    ASSERT_FALSE(array5.none());
    ASSERT_TRUE(emptyArray.none());
    ASSERT_TRUE(defaultArray.none());
}

TEST_F(BitArrayTestCommon, TestEmpty) {
    ASSERT_TRUE(emptyArray.empty());
    ASSERT_FALSE(defaultArray.empty());
    ASSERT_FALSE(array4.empty());
    array4.clear();
    ASSERT_TRUE(array4.empty());
}

TEST_F(BitArrayTestCommon, TestCount) {
    ASSERT_EQ(1, array3.count());
    array3.set(1, true);
    ASSERT_EQ(2, array3.count());
    array3.pushBack(true);
    ASSERT_EQ(3, array3.count());
    ASSERT_EQ(0, defaultArray.count());
}

TEST_F(BitArrayTestCommon, TestSize) {
    ASSERT_EQ(0, emptyArray.size());
    ASSERT_EQ(32, array4.size());
    array4.resize(70, true);
    ASSERT_EQ(70, array4.size());
    array4.pushBack(false);
    ASSERT_EQ(71, array4.size());
}

TEST_F(BitArrayTestCommon, TestOperatorSquareBrackets) {
    ASSERT_THROW(emptyArray[1], std::invalid_argument);
    ASSERT_THROW(emptyArray[-1], std::invalid_argument);
    ASSERT_TRUE(array2[0]);
    ASSERT_FALSE(array2[1]);
    ASSERT_TRUE(array5[32]);
}

TEST(BitArrayTest, TestToString) {
    BitArray value(10, 20);
    ASSERT_EQ("0010100000", value.toString());
    value.resize(35, true);
    ASSERT_EQ("00101000001111111111111111111111111", value.toString());
    value.set();
    ASSERT_EQ("11111111111111111111111111111111111", value.toString());
    value.reset();
    ASSERT_EQ("00000000000000000000000000000000000", value.toString());
}

class BitArrayComparse : public testing::Test {
public:
    BitArray array1, array2, array3, array4, array5, array6;
protected:
    void SetUp() {

    }
    void TearDown() {}
};

TEST_F(BitArrayTestCommon, TestOperatorComparison) {
    ASSERT_TRUE(array1 == defaultArray);
    ASSERT_TRUE(array4 == array4);
    ASSERT_FALSE(array1 == array2);
    ASSERT_FALSE(emptyArray == defaultArray);
}

TEST_F(BitArrayTestCommon, TestOperatorNotComparison) {
    ASSERT_FALSE(array1 != defaultArray);
    ASSERT_FALSE(array4 != array4);
    ASSERT_TRUE(array1 != array2);
    ASSERT_TRUE(emptyArray != defaultArray);
}

TEST_F(BitArrayTestCommon, TestOpearatorEquality) {
    array1 = array2;
    ASSERT_EQ(array1, array2);
    ASSERT_EQ(array2, array2);
}



int main(int argc, char** argv)
{
    //std::cout << value1.toString() << std::endl;
    BitArray bitArray = BitArray(33, 98769876);
    BitArray bitArray2 = BitArray(33, 20);
    bitArray.set(33, true);
    if (bitArray != bitArray2) {
        std::cout << ULONG_MAX << std::endl;
        std::cout << bitArray.toString() << std::endl;
        std::cout << bitArray2.toString() << std::endl;
    }
    /*std::cout << LONG_MAX << " " << 21921929 << std::endl;
    std::cout << 21921929 << std::endl;*/
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
