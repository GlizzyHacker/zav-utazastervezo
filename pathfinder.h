#include "graph.h"
#include "sorted_list.hpp"

#ifndef PATHFINDER
#define PATHFINDER

/*!
 * Virtualis utvonalkereso egy grafban
 * oroklesevel lehet utvonalkereso algoritmusokat implementalni
 */
class Pathfinder {
protected:
	/*!
	 * A graf amiben az utvonalat keressuk
	 */
	Graph graph;
	/*!
	 * A keresett utak szama
	 */
	size_t numRoutes;
public:
	/*!
	 * Letrehoz egy utvonalkeresot
	 * @param graph a graf amiben az utvonalakat keressuk
	 * @param numRoutes a keresett utak szama
	 */
	Pathfinder(Graph graph, size_t numRoutes = 3);
	/*!
	 * Utvonalakat keres a megadott parameterekkel
	 * @param from a csucs ahonnan indulunk
	 * @param to a csucs ahova erkezunk
	 * @param starTime az indulas ideje
	 * @return Az utvonalakat tartalmazo idohossz szerint novekvo sorrendben rendezett lista
	 */
	virtual SortedList<Route*> getRoutes(const Node& from,const Node& to, Time starTime) = 0;

	virtual ~Pathfinder();
};
#endif