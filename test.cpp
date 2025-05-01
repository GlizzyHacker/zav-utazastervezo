#include <iostream>
#include <fstream>
#include "array.hpp"
#include "gtest_lite.h"
#include "csvparser.h"
#include "memtrace.h"

#ifdef CPORTA

bool isArgument(const char* input, const char arg, const char* argument) {
	if (input[0] == '-') {
		if (input[1] == '-') {
			return strcmp(input + 2, argument) == 0;
		}
		return input[1] == arg;
	}
	return false;
}
int main() {
	GTINIT(true);

	TEST(Array, operators) {
		int* tmp = new int[1];
		tmp[0] = 2;
		Array<int> array(1,tmp);

		EXPECT_EQ(tmp, array + 0);

		array += 1;
		Array<int> array2(array);
		Array<int> array3;
		array3 = array;

		EXPECT_EQ(2, array[0]);
		EXPECT_EQ(1, array[1]);
		EXPECT_EQ(2, array.getLength());

		EXPECT_NE(array+0, array2+0);
		
		EXPECT_EQ(2, array2[0]);
		EXPECT_EQ(1, array2[1]);
		EXPECT_EQ(2, array2.getLength());

		EXPECT_EQ(2, array3[0]);
		EXPECT_EQ(1, array3[1]);
		EXPECT_EQ(2, array3.getLength());
		
	} END

		TEST(Main, isArgument) {
		EXPECT_TRUE(isArgument("-A", 'A', "All"));
		EXPECT_TRUE(isArgument("--All", 'A', "All"));
		EXPECT_FALSE(isArgument("-X", 'A', "All"));
		EXPECT_FALSE(isArgument("--Xll", 'A', "All"));
	} END

		TEST(CSV, line isEmpty) {
		CSVLine line = CSVLine("");
		CSVLine line2 = CSVLine(NULL);
		CSVLine line3 = CSVLine("something,somethingelse");
		EXPECT_TRUE(line.isEmpty());
		EXPECT_TRUE(line2.isEmpty());
		EXPECT_FALSE(line3.isEmpty());
	} END

		TEST(CSV, read simple) {
		std::ofstream CVS = std::ofstream("test.cvs",3);
		CVS << "test";
		CVS.close();
		CSVParser csv("test.csv");
		CSVLine line = csv.read();
		if (line.isEmpty()) {
			FAIL();
		}
		else {
			EXPECT_STREQ("test", line.getColumns()[0]);
		}
	} END

		TEST(CSV, read multiple lines) {
	} END

		TEST(CSV, read after addition) {
	} END

		TEST(CSV, read incorrect file) {
	} END

		TEST(CSV, write simple) {
	} END

		TEST(CSV, write multiple) {
	} END

		TEST(CSV, write after addition) {
	} END

		TEST(Node from csv, edge addition) {
	} END

		TEST(Edge from csv, node addition) {
	} END

		TEST(Graph from csv, simple graph) {
	} END

		TEST(Graph from csv, complex graph) {
	} END

		TEST(Graph from csv, incorrect format) {
	} END

		//AZ ALLAPOTGEP MINDEN ESETERE TESZT
		TEST(Agent, moved state) {
	} END

		TEST(Agent, terminated state) {
	} END

		TEST(Agent, arrived state) {
	} END

		TEST(Agent pathfinder, create agent) {
	} END

		TEST(Agent pathfinder, delete agent) {
	} END

		TEST(Agent pathfinder, simple) {
	} END

		TEST(Agent pathfinder, number of routes) {
	} END

		//IDE KERULNEK TESZT ESETEK TRUKKOS HELYZETEKKEL
		TEST(Agent pathfinder, complex) {
	} END
}
#endif