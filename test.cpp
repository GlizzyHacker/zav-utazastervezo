#include "gtest_lite.h"
#include "csvparser.h"

#ifdef CPORTA
int main() {
	GTINIT(std::cin);

		TEST(Main, isArgument) {
	} END

		TEST(CSV, line isEmpty) {
	} END

		TEST(CSV, read simple) {
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
		TEST(Agent, move state) {
	} END

		TEST(Agent, terminate state) {
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