#include <iostream>
#include <fstream>

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
		TEST(Main, isArgument) {
			EXPECT_TRUE(isArgument("-A", 'A', "All"));
			EXPECT_TRUE(isArgument("--All", 'A', "All"));
			EXPECT_FALSE(isArgument("-X", 'A', "All"));
			EXPECT_FALSE(isArgument("--Xll", 'A', "All"));
	} END
			TEST(CSV, CSVLine ctor) {
		CSVLine line = CSVLine(NULL);
		CSVLine line2 = CSVLine("something,somethingelse");
		CSVLine line3 = CSVLine("something,somethingelse,");

		EXPECT_TRUE(NULL == line.getColumns());
		EXPECT_STREQ("something", line2.getColumns()[0]);
		EXPECT_STREQ("somethingelse", line2.getColumns()[1]);
		EXPECT_TRUE(2 == line3.getNumColumns());
		EXPECT_STREQ("somethingelse", line3.getColumns()[1]);
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
		std::ofstream CVS = std::ofstream("test.cvs");
		CVS << "test";
		CVS.close();
		CSVParser csv = CSVParser("test.csv");
		EXPECT_STREQ("test", csv.read().getColumns()[0]);
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