#include <iostream>
#include <fstream>
#include <sstream>

#include "array.hpp"
#include "sorted_list.hpp"
#include "csvparser.h"
#include "csvgraph.h"
#include "memtrace.h"
#include "time.h"
#include "graph.h"
#include "agent.h"
#include "log.hpp"
#ifdef CPORTA
#define TESTS
#endif

#ifdef TESTS
#include "gtest_lite.h"

//HACK: TOO LAZY TO IMPLEMENT VARIADIC FUNCTION
template<typename T>
Array<T> toArray(T item) {
	Array<T> arr;
	arr += item;
	return arr;
}
template<typename T>
Array<T> toArray(T item1, T item2) {
	Array<T> arr;
	arr += item1;
	arr += item2;
	return arr;
}
template<typename T>
Array<T> toArray(T item1, T item2, T item3) {
	Array<T> arr;
	arr += item1;
	arr += item2;
	arr += item3;
	return arr;
}

bool operator==(const Time& time1, const Time& time2) {
	return time1 - time2 == 0;
}
bool operator!=(const Time& time1, const Time& time2) {
	return time1 - time2 != 0;
}

bool operator==(const Edge& edge1, const Edge& edge2) {
	EXPECT_STREQ(edge1.getName(), edge2.getName());

	EXPECT_STREQ(edge1.getToNode()->getName(), edge2.getToNode()->getName());

	EXPECT_EQ(edge1.getFirstStartTimeAfter(Time()), edge2.getFirstStartTimeAfter(Time()));

	EXPECT_EQ(edge1.getWeight(Time(0, 0)), edge2.getWeight(Time(0, 0)));

	return true;
}

bool operator==(const Route& route1, const Route& route2) {
	try
	{
		for (size_t i = 0; i < route1.getEdges().getLength(); i++)
		{
			*route1.getEdges()[i] == *route2.getEdges()[i];
		}
	}
	catch (const std::out_of_range&)
	{
		FAIL() << "Route too short" << std::endl;
	}
	return true;
}

bool operator==(const Graph& graph1, const Graph& graph2) {
	try
	{
		for (size_t i = 0; i < graph1.getNodes().getLength(); i++)
		{
			EXPECT_STREQ(graph1.getNodes()[i]->getName(), graph2.getNodes()[i]->getName()) << i << ". node" << std::endl;
			for (size_t j = 0; j < graph1.getNodes()[i]->getEdges().getLength(); j++)
			{
				std::cout << i << ". node " << j << ". edge" << std::endl;
				*graph1.getNodes()[i]->getEdges()[j] == *graph2.getNodes()[i]->getEdges()[j];
			}
		}
	}
	catch (const std::out_of_range&)
	{
		FAIL() << "Graph too short" << std::endl;
	}
	return true;
}

//ATMASOLVA IDE MAINBOL MERT NEM LEHET MAINT INCLUDOLNI
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
	writeLog = true;

	//SIMPLE TEST OBJECTS
	Node simpleNode1;
	Node simpleNode2;
	Node simpleNode3;
	Edge simpleEdge1 = Edge(&simpleNode1, &simpleNode2, 10, toArray(Time(10, 0)), "edge");
	Edge simpleEdge2 = Edge(&simpleNode2, &simpleNode3, 20, toArray(Time(10, 10)), "edge");
	Array<Edge*> edges1;
	edges1 += &simpleEdge1;
	Array<Edge*> edges2;
	edges2 += &simpleEdge2;
	simpleNode1 = Node(Array<Edge*>(edges1), "name1");
	simpleNode2 = Node(Array<Edge*>(edges2), "name2");
	simpleNode3 = Node(Array<Edge*>(), "name3");
	Graph simpleTestGraph(toArray<Node*>(&simpleNode1, &simpleNode2, &simpleNode3));
	
	Route SimpleTestRoute(toArray<Edge*>(&simpleEdge1, &simpleEdge2), Time(10,0));

	//COMPLEX TEST OBJECTS
	Node complexNodeStart;
	Node complexNodeEnd;
	Node complexNode1;
	Node complexNode2;
	Node complexNode3;
	Edge complexEdge1 = Edge(&complexNodeStart, &complexNode1, 10, toArray(Time(10, 0)), "edge1");
	Edge complexEdge2 = Edge(&complexNodeStart, &complexNode2, 11, toArray(Time(8, 0)), "edge2");
	Edge complexEdge3 = Edge(&complexNodeStart, &complexNode3, 12, toArray(Time(9, 0)), "edge3");
	Edge complexEdge4 = Edge(&complexNode1, &complexNodeEnd, 15, toArray(Time(10, 10)), "edge1");
	Edge complexEdge5 = Edge(&complexNode2, &complexNodeEnd, 16, toArray(Time(8, 11)), "edge2");
	Edge complexEdge6 = Edge(&complexNode3, &complexNodeEnd, 17, toArray(Time(9, 12)), "edge3");
	complexNodeStart = Node(toArray(&complexEdge1, &complexEdge2, &complexEdge3), "nodeStart");
	complexNode1 = Node(toArray(&complexEdge4), "node1");
	complexNode2 = Node(toArray(&complexEdge5), "node2");
	complexNode3 = Node(toArray(&complexEdge6), "node3");
	complexNodeEnd = Node(Array<Edge*>(), "nodeEnd");
	Array<Node*> complexNodes;
	complexNodes += &complexNodeStart;
	complexNodes += &complexNode1;
	complexNodes += &complexNodeEnd;
	complexNodes += &complexNode2;
	complexNodes += &complexNode3;
	Graph complexTestGraph(complexNodes);

	Route ComplexTestRoute1(toArray(&complexEdge1, &complexEdge4), Time(10,0));
	Route ComplexTestRoute2(toArray(&complexEdge2, &complexEdge5), Time(8, 11));
	Route ComplexTestRoute3(toArray(&complexEdge3, &complexEdge6), Time(9, 12));

	GTINIT(std::cin);

	TEST(Array, operators) {
		int tmp[1];
		tmp[0] = 2;
		Array<int> array(1, tmp);
		array += 1;
		Array<int> array2(array);
		Array<int> array3;
		array3 = array;

		EXPECT_EQ(2, array[0]);
		EXPECT_EQ(1, array[1]);
		EXPECT_EQ((size_t)2, array.getLength());

		EXPECT_NE(array + 0, array2 + 0);

		EXPECT_EQ(2, array2[0]);
		EXPECT_EQ(1, array2[1]);
		EXPECT_EQ((size_t)2, array2.getLength());

		EXPECT_EQ(2, array3[0]);
		EXPECT_EQ(1, array3[1]);
		EXPECT_EQ((size_t)2, array3.getLength());

	} ENDM

		TEST(Sorted list, build) {
		SortedList<int> list;

		list += 0;
		list += 1;
		list += 2;
		list += 6;
		list += 4;
		list += 3;
		list += 5;

		int i = 0;
		for (SortedList<int>::Iterator begin = list.begin(); begin != list.end(); begin++)
		{
			EXPECT_EQ(i, *begin);
			i++;
		}
		EXPECT_EQ((size_t)7, list.getLength());
		
	} ENDM

		TEST(Sorted list, deletes) {
		SortedList<int> list;
		list += 0;
		list += 1;
		list += 2;
		list += 3;

		list.removeAt(0);
		
		SortedList<int>::Iterator iter1 = list.begin();
		EXPECT_EQ(1, *iter1);
		EXPECT_EQ(2, *(iter1++));
		EXPECT_EQ(3, *(iter1++));
		EXPECT_EQ((size_t)3, list.getLength());

		list.remove(2);

		SortedList<int>::Iterator iter2 = list.begin();
		EXPECT_EQ(1, *iter2);
		EXPECT_EQ(3, *(iter2++));
		EXPECT_EQ((size_t)2, list.getLength());
	} ENDM

		TEST(Time, print) {
		Time t1(12, 12);
		Time t2(1, 3);
		std::stringstream str1, str2;

		str1 << t1;
		str2 << t2;

		EXPECT_STREQ("12:12", str1.str().c_str());
		EXPECT_STREQ("01:03", str2.str().c_str());
	} ENDM

		TEST(Time, operators) {
		Time t1(12, 12);
		Time t2(10, 13);

		size_t dif1 = t2 - t1;
		size_t dif2 = t1 - t2;
		t1 += 70;
		t2 -= 80;

		EXPECT_EQ((size_t)1440 - 119, dif1);
		EXPECT_EQ((size_t)119, dif2);
		std::stringstream str1, str2;
		str1 << t1;
		str2 << t2;
		t1 == Time(13, 22);
		t2 == Time(8, 53);
	} ENDM

		TEST(Main, isArgument) {
		EXPECT_TRUE(isArgument("-A", 'A', "All"));
		EXPECT_TRUE(isArgument("--All", 'A', "All"));
		EXPECT_FALSE(isArgument("-X", 'A', "All"));
		EXPECT_FALSE(isArgument("--Xll", 'A', "All"));
	} ENDM


		TEST(Graph, edge weight) {
		Node n1, n2;
		Edge e(&n1, &n2, 20, toArray(Time(10, 0)), "test");

		EXPECT_EQ((size_t)0 + 20, e.getWeight(Time(10, 0)));
		EXPECT_EQ((size_t)1439 + 20, e.getWeight(Time(10, 1)));
		EXPECT_EQ((size_t)1 + 20, e.getWeight(Time(9, 59)));
	} ENDM

		TEST(Graph, find node) {
		Node node1(Array<Edge*>(), "name1");
		Node node2(Array<Edge*>(), "name2");
		Node node3(Array<Edge*>(), "name3");
		Node nodeU(Array<Edge*>(), "unique");
		Array<Node*> nodes;
		nodes += &node1;
		nodes += &node2;
		nodes += &node3;
		nodes += &nodeU;
		Graph graph(nodes);

		Node* node = graph.getNode("name1");
		Node* nodeQ = graph.getNode("uni");

		EXPECT_STREQ("name1", node->getName());
		EXPECT_STREQ("unique", nodeQ->getName());
		EXPECT_THROW(graph.getNode("name4"), NotFound&);

	} ENDM

		TEST(Graph, route weight) {
		Node node1(Array<Edge*>(), "name1");
		Node node2(Array<Edge*>(), "name2");
		Node node3(Array<Edge*>(), "name3");
		Node node4(Array<Edge*>(), "name4");
		Edge edge1 = Edge(&node1, &node2, 10, toArray(Time(10, 0)), "name1");
		Edge edge2 = Edge(&node2, &node3, 20, toArray(Time(12, 40)), "name2");
		Edge edge3 = Edge(&node3, &node4, 100, toArray(Time(9, 0)), "name3");
		Array<Edge*> edges;
		edges += &edge1;
		edges += &edge2;
		edges += &edge3;
		Route route(edges, Time(9,50));

		size_t weight = route.getTotalWeight();

		EXPECT_EQ((size_t)1440 + 50, weight);

	} ENDM

		TEST(CSV, line isEmpty) {
		CSVLine line = CSVLine("");
		CSVLine line2 = CSVLine(NULL);
		CSVLine line3 = CSVLine("something,somethingelse");

		EXPECT_TRUE(line.isEmpty());
		EXPECT_TRUE(line2.isEmpty());
		EXPECT_FALSE(line3.isEmpty());
	} ENDM

		TEST(CSV, line quotes) {
		CSVLine line = CSVLine("\"test,test\", test, \",test,\", test2");

		EXPECT_STREQ("test,test", line.getColumns()[0]+0);
		EXPECT_STREQ("test", line.getColumns()[1] + 0);
		EXPECT_STREQ(",test,", line.getColumns()[2] + 0);
		EXPECT_STREQ("test2", line.getColumns()[3] + 0);
	} ENDM

		TEST(CSV, read simple) {
		std::ofstream CSV("testr.csv");
		CSV << "test, test2 ";
		CSV.close();
		CSVParser csv("testr.csv");

		CSVLine line = csv.read();

		if (line.isEmpty()) {
			FAIL();
		}
		else {
			EXPECT_STREQ("test", line.getColumns()[0] + 0);
			EXPECT_STREQ("test2", line.getColumns()[1] + 0);
		}
	} ENDM

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
	} ENDM

		TEST(CSV, read after addition) {
		std::ofstream CSV1("testra1.csv");
		std::ofstream CSV2("testra2.csv");
		CSV1 << "test11,test12,test13";
		CSV2 << "test21,test22,test23";
		CSV1.close();
		CSV2.close();
		CSVParser csv1("testra1.csv");
		CSVParser* csv2 = new CSVParser("testra2.csv");

		csv1 += (*csv2);
		CSVLine line1 = csv1.read();
		CSVLine line2 = csv1.read();

		EXPECT_STREQ("test11", line1.getColumns()[0] + 0);
		EXPECT_STREQ("test12", line1.getColumns()[1] + 0);
		EXPECT_STREQ("test13", line1.getColumns()[2] + 0);
		EXPECT_STREQ("test21", line2.getColumns()[0] + 0);
		EXPECT_STREQ("test22", line2.getColumns()[1] + 0);
		EXPECT_STREQ("test23", line2.getColumns()[2] + 0);
	} ENDM

		TEST(CSV, write simple) {
		CSVParser csv1("testws.csv");

		csv1.write(CSVLine("test"));

		std::ifstream CSV("testws.csv");
		char line[100];
		CSV >> line;
		EXPECT_STREQ("test", line);
		CSV.close();

	} ENDM

		TEST(CSV, write multiple) {
		CSVParser csv1("testwc.csv");

		csv1.write(CSVLine("test1,test2,test3"));
		csv1.write(CSVLine("test11,test22,test33"));

		std::ifstream CSV("testwc.csv");
		char line1[100];
		char line2[100];
		CSV >> line1;
		CSV >> line2;
		EXPECT_STREQ("test1,test2,test3", line1);
		EXPECT_STREQ("test11,test22,test33", line2);
		CSV.close();
	} ENDM

		TEST(Node from csv, edge addition) {
		CSVNode node("node1");
		CSVEdge edge(10, toArray(Time(12, 12)), "edge1");

		node += edge;

		EXPECT_EQ(&edge, node.getEdges()[0]);
	} ENDM

		TEST(Edge from csv, node addition) {
		CSVNode node1("node1");
		CSVEdge edge(10, toArray(Time(12, 12)), "edge1");
		CSVNode node2("node1");

		node1 += edge;
		edge += node2;

		EXPECT_EQ(&node2, edge.getToNode());
	} ENDM

		TEST(Cron from csv, cron simple) {
		Array<Time> times = parseTime("11 12");
		times[0] == Time(11, 12);

	} ENDM

		TEST(Cron from csv, cron asterisk) {
		Array<Time> times = parseTime("* *");
		for (size_t i = 0; i < 24; i++)
		{
			for (size_t j = 0; j < 60; j++)
			{
				Time(i, j) == times[i * 60 + j];
			}
		}
	} ENDM
		TEST(Cron from csv, cron list) {
		Array<Time> times = parseTime("1,2,3 0");
		times[0] == Time(1, 0);
		times[1] == Time(2, 0);
		times[2] == Time(3, 0);
	} ENDM

		TEST(Cron from csv, cron exception) {
		EXPECT_THROW(parseTime("*"), std::out_of_range&);
		EXPECT_THROW(parseTime("a 1"), FormatInvalid&);

	} ENDM

		TEST(Route to csv, simple) {
		CSVLine line = writeRoute(SimpleTestRoute);

		EXPECT_EQ((size_t)9, line.getColumns().getLength());
		EXPECT_STREQ("30", line.getColumns()[0] + 0);
		EXPECT_STREQ("10:00", line.getColumns()[1] + 0);
		EXPECT_STREQ("edge", line.getColumns()[2] + 0);
		EXPECT_STREQ("10", line.getColumns()[3] + 0);
		EXPECT_STREQ("name2", line.getColumns()[4] + 0);
		EXPECT_STREQ("10:10", line.getColumns()[5] + 0);
		EXPECT_STREQ("edge", line.getColumns()[6] + 0);
		EXPECT_STREQ("20", line.getColumns()[7] + 0);
		EXPECT_STREQ("name3", line.getColumns()[8] + 0);
	} ENDM

		TEST(Graph from csv, simple graph) {
		std::ofstream CSV("testgs.csv");
		CSV << "edge,10 0" << std::endl;
		CSV << "0,name1" << std::endl;
		CSV << "10,name2" << std::endl;
		CSV << "30,name3" << std::endl;
		CSV.close();
		CSVParser csv("testgs.csv");

		CSVGraph graph(csv);

		simpleTestGraph == graph;
	} ENDM

		TEST(Graph from csv, complex graph) {
		std::ofstream CSV("testgc.csv");
		CSV << "edge1,10 0" << std::endl;
		CSV << "0,nodeStart" << std::endl;
		CSV << "10,node1" << std::endl;
		CSV << "25,nodeEnd" << std::endl;
		CSV << "edge2,8 0" << std::endl;
		CSV << "0,nodeStart" << std::endl;
		CSV << "11,node2" << std::endl;
		CSV << "27,nodeEnd" << std::endl;
		CSV << "edge3,9 0" << std::endl;
		CSV << "0,nodeStart" << std::endl;
		CSV << "12,node3" << std::endl;
		CSV << "29,nodeEnd" << std::endl;
		CSV.close();
		CSVParser csv("testgc.csv");

		CSVGraph graph(csv);

		complexTestGraph == graph;
	} ENDM

		TEST(Graph from csv, incorrect format) {
		std::ofstream CSV1("testgi1.csv");
		CSV1 << "Test1,12 12" << std::endl;
		CSV1 << "a, a" << std::endl;
		CSV1.close();
		std::ofstream CSV2("testgi2.csv");
		CSV2 << "Test1,12 12" << std::endl;
		CSV2 << "0" << std::endl;
		CSV2.close();
		std::ofstream CSV3("testgi3.csv");
		CSV3 << "Test1,abc" << std::endl;
		CSV3 << "0, a" << std::endl;
		CSV3.close();

		CSVParser csv1("testgi1.csv");
		EXPECT_THROW(CSVGraph graph(csv1), FormatInvalid&);

		CSVParser csv2("testgi2.csv");
		EXPECT_THROW(CSVGraph graph(csv2), FormatInvalid&);

		CSVParser csv3("testgi3.csv");
		EXPECT_THROW(CSVGraph graph(csv3), FormatInvalid&);

	} ENDM

		//AZ ALLAPOTGEP MINDEN ESETERE TESZT
		TEST(Agent, moved state) {
		Node node1;
		Node node2;
		Node node3;
		Node node4;
		Edge edge1 = Edge(&node1, &node2, 10, toArray(Time(10, 0)), "name1");
		Edge edge2 = Edge(&node2, &node3, 20, toArray(Time(12, 40)), "name2");
		node1 = Node(toArray(&edge1), "name1");
		node2 = Node(toArray(&edge2), "name2");
		node3 = Node(Array<Edge*>(), "name3");

		Agent agent(edge1, node1, node3, Time());

		EXPECT_EQ(moved, agent.step());
		EXPECT_EQ(&edge2, agent.getEdges()[1]);

	} ENDM

		TEST(Agent, terminated state) {
		Node node1;
		Node node2;
		Node node3;
		Node node4;
		Edge edge1 = Edge(&node1, &node2, 10, toArray(Time(10, 0)), "name1");
		Edge edge2 = Edge(&node3, &node4, 20, toArray(Time(12, 40)), "name2");
		Edge edge3 = Edge(&node4, &node3, 20, toArray(Time(12, 40)), "name3");
		node1 = Node(toArray(&edge1), "name1");
		node2 = Node(Array<Edge*>(), "name2");
		node3 = Node(toArray(&edge2), "name3");
		node4 = Node(toArray(&edge3), "name4");

		//ZSÁKUTCA
		Agent agent1(edge1, node1, node1, Time());
		EXPECT_EQ(terminated, agent1.step());

		//KÖR
		Agent agent2(edge3, node4, node1, Time());
		agent2.step();

		EXPECT_EQ(terminated, agent2.step());
	} ENDM

		TEST(Agent, arrived state) {
		Node node1;
		Node node2;
		Edge edge1 = Edge(&node1, &node2, 10, toArray(Time(10, 0)), "name1");
		Edge edge2 = Edge(&node2, &node1, 20, toArray(Time(12, 40)), "name2");
		node1 = Node(toArray(&edge1), "name1");
		node2 = Node(Array<Edge*>(), "name2");

		Agent agent(edge1, node1, node2, Time());

		EXPECT_EQ(arrived, agent.step());
	} ENDM

		TEST(Agent pathfinder, simple) {
		AgentPathfinder pathfinder(simpleTestGraph, 1);

		SortedList<Route*> routes = pathfinder.getRoutes(simpleNode1, simpleNode3, Time(0, 0));

		EXPECT_EQ((size_t)1, routes.getLength());
		SimpleTestRoute == **routes.begin();
		delete* routes.begin();
	} ENDM
		GTEND(std::cerr);
		return 0;
}
#endif