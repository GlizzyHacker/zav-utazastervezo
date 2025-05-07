#include <iostream>
#include <fstream>
#include <sstream>

#include "array.hpp"
#include "gtest_lite.h"
#include "csvparser.h"
#include "memtrace.h"
#include "time.h"
#include "graph.h"

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

	TEST(Array, operators) {
		int* tmp = new int[1];
		tmp[0] = 2;
		Array<int> array(1, tmp);

		EXPECT_EQ(tmp, array + 0);

		array += 1;
		Array<int> array2(array);
		Array<int> array3;
		array3 = array;

		EXPECT_EQ(2, array[0]);
		EXPECT_EQ(1, array[1]);
		EXPECT_EQ(2, array.getLength());

		EXPECT_NE(array + 0, array2 + 0);

		EXPECT_EQ(2, array2[0]);
		EXPECT_EQ(1, array2[1]);
		EXPECT_EQ(2, array2.getLength());

		EXPECT_EQ(2, array3[0]);
		EXPECT_EQ(1, array3[1]);
		EXPECT_EQ(2, array3.getLength());

	} END

		TEST(Time, print) {
		Time t1(-12, -12);
		Time t2(1, 3);
		std::stringstream str1, str2;

		str1 << t1;
		str2 << t2;

		EXPECT_STREQ("11:48", str1.str().c_str());
		EXPECT_STREQ("01:03", str2.str().c_str());
	} END

		TEST(Time, operators) {
		Time t1(12, 12);
		Time t2(10, 13);

		int dif1 = t2 - t1;
		int dif2 = t1 - t2;
		t1 += 70;
		t2 -= 80;

		EXPECT_EQ(3719, dif1);
		EXPECT_EQ(119, dif2);
		std::stringstream str1, str2;
		str1 << t1;
		str2 << t2;
		EXPECT_STREQ("13:22", str1.str().c_str());
		EXPECT_STREQ("08:53", str2.str().c_str());
	} END

		TEST(Main, isArgument) {
		EXPECT_TRUE(isArgument("-A", 'A', "All"));
		EXPECT_TRUE(isArgument("--All", 'A', "All"));
		EXPECT_FALSE(isArgument("-X", 'A', "All"));
		EXPECT_FALSE(isArgument("--Xll", 'A', "All"));
	} END


		TEST(Graph, edge weight) {
		Node n1, n2;
		Edge e(&n1, &n2, 20, Time(10,0), "test");

		EXPECT_EQ(0 + 20, e.getWeight(Time(10, 0)));
		EXPECT_EQ(3601 + 20, e.getWeight(Time(10, 1)));
		EXPECT_EQ(1 + 20, e.getWeight(Time(9, 59)));
	} END

		TEST(Graph, find node) {
		Array<Node> nodes;
		nodes += Node(Array<Edge>(), "name1");
		nodes += Node(Array<Edge>(), "name2");
		nodes += Node(Array<Edge>(), "name3");
		Graph graph(nodes);

		Node* node1 = graph.getNode("name1");

		EXPECT_EQ("name1", node1->getName());
		EXPECT_THROW(graph.getNode("name4"), NotFound);
			
	} END

		TEST(Graph, route weight) {
		Node node1(Array<Edge>(), "name1");
		Node node2(Array<Edge>(), "name2");
		Node node3(Array<Edge>(), "name3");
		Node node4(Array<Edge>(), "name4");
		Array<Edge> edges;
		edges += Edge(&node1,&node2,10, Time(10,0), "name1");
		edges += Edge(&node2, &node3, 20, Time(12, 40), "name2");
		edges += Edge(&node3, &node4, 100, Time(9, 0), "name3");
		Route route(edges);

		int weight = route.getTotalWeight(Time(9, 50));

		EXPECT_EQ(3600+50, weight);

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
		std::ofstream CSV("testr.csv");
		CSV << "test";
		CSV.close();
		CSVParser csv("testr.csv");

		CSVLine line = csv.read();

		if (line.isEmpty()) {
			FAIL();
		}
		else {
			EXPECT_STREQ("test", line.getColumns()[0] + 0);
		}
	} END

		TEST(CSV, read multiple lines) {
		std::ofstream CSV("testrm.csv");
		CSV << "test11,test12,test13" << std::endl;
		CSV << "test21,test22,test23" << std::endl;
		CSV << "test31,test32,test33";
		CSV.close();
		CSVParser csv("testrm.csv");

		CSVLine line1 = csv.read();
		CSVLine line2 = csv.read();
		CSVLine line3 = csv.read();

		if (line1.isEmpty() || line2.isEmpty() || line3.isEmpty()) {
			FAIL();
		}
		else {
			EXPECT_STREQ("test11", line1.getColumns()[0] + 0);
			EXPECT_STREQ("test12", line1.getColumns()[1] + 0);
			EXPECT_STREQ("test13", line1.getColumns()[2] + 0);
			EXPECT_STREQ("test21", line2.getColumns()[0] + 0);
			EXPECT_STREQ("test23", line2.getColumns()[2] + 0);
			EXPECT_STREQ("test31", line3.getColumns()[0] + 0);
			EXPECT_STREQ("test33", line3.getColumns()[2] + 0);
		}
	} END

		TEST(CSV, read after addition) {
		std::ofstream CSV1("testra1.csv");
		std::ofstream CSV2("testra2.csv");
		CSV1 << "test11,test12,test13";
		CSV2 << "test21,test22,test23";
		CSV1.close();
		CSV2.close();
		CSVParser csv1("testra1.csv");
		CSVParser csv2("testra2.csv");

		csv1 += (csv2);
		CSVLine line1 = csv1.read();
		CSVLine line2 = csv1.read();

		EXPECT_STREQ("test11", line1.getColumns()[0] + 0);
		EXPECT_STREQ("test12", line1.getColumns()[1] + 0);
		EXPECT_STREQ("test13", line1.getColumns()[2] + 0);
		EXPECT_STREQ("test21", line2.getColumns()[0] + 0);
		EXPECT_STREQ("test22", line2.getColumns()[1] + 0);
		EXPECT_STREQ("test23", line2.getColumns()[2] + 0);
	} END

		TEST(CSV, read incorrect file) {

	} END

		TEST(CSV, write simple) {
		CSVParser csv1("testw.csv");

		csv1.write(CSVLine("test"));

		std::ifstream CSV("testw.csv");
		char line[100];
		CSV >> line;
		EXPECT_STREQ("test", line);
		CSV.close();

	} END

		TEST(CSV, write multiple) {
		CSVParser csv1("testw.csv");

		csv1.write(CSVLine("test1,test2,test3"));
		csv1.write(CSVLine("test11,test22,test33"));

		std::ifstream CSV("testw.csv");
		char line1[100];
		char line2[100];
		CSV >> line1;
		CSV >> line2;
		EXPECT_STREQ("test1,test2,test3", line1);
		EXPECT_STREQ("test11,test22,test33", line2);
		CSV.close();
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