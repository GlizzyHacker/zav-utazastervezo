#include "csvparser.h"
#include "graph.h"

#ifndef CSVGRAPH
#define CSVGRAPH

/*!
 * .csv fajlbol letrehozott Node
 * /see Node
 */
class CSVNode;

/*!
 * .csv fajlbol letrehozott Edge
 * /see Edge
 */
class CSVEdge : public Edge {
public:
	CSVEdge(int weight, Array<Time> startTimes, const char* name);

	/*!
	 * Hozzafuz az elhez egy csucsot
	 */
	void operator+=(CSVNode& node);
};

class CSVNode : public Node {
public:
	CSVNode(const char* name);

	/*!
	 * Hozzafuz a csucshoz egy elt
	 */
	void operator+=(CSVEdge& edge);
};

/*!
 * .csv fajlbol letrehozott Graph
 * /see Graph
 */
class CSVGraph : public Graph {

public:
	/*!
	 * Letrehoz egy grafot egy vagy tobb .csv fajlbol
	 * @param csv a .csv fajl(ok)at kezelo parser
	 */
	CSVGraph(CSVParser& csv);

	/*!
	 * Hozzafuz a grafhoz egy csucsot
	 */
	void operator+=(CSVNode& node);

	~CSVGraph();
};

/*!
 * Letrehoz egy .csv sort egy utvonalbol
 * @param route az utvonal amit a sorba ir
 * @return a sor ami tartalmazza az utvonalat
 */
CSVLine writeRoute(Route& route);

/*!
 * Beolvas Idopontokat
 * a CRON formatum modositott valtozata alapjan
 * @param timeString a c-string amibol beolvassa az idopontokat
 * @return az Idopontokat tartalmazo dinamikus tomb
 * @exception FormatInvalid ha timeString betuket tartalmaz
 * @exception std::out_of_range ha nem ket kulon reszbol all
 */
Array<Time> parseTime(const char* timeString);
#endif
