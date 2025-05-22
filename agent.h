#include "pathfinder.h"
#include "graph.h"
#include "array.hpp"
#include "sorted_list.hpp"

#ifndef AGENT
#define AGENT

/*!
 * Az agent allapotai
 */
enum AgentState
{
	/*!
	 * Az agent halad tovabb egyenesen
	 */
	moved = 0,
	/*!
	 * Az agent megerkezett a celba
	 */
	arrived = 1,
	/*!
	 * Az agent ido elott megallt, valamilyen akadaly miatt
	 */
	terminated = 2
};

/*!
 * Egy grafban elore halado agent
 */
class Agent : public Route {
	/*!
	 * A kezdo nodera mutato pointer
	 */
	const Node* start;

	/*!
	 * A cel nodera mutato pointer
	 */
	const Node* target;
public:
	/*!
	 * Letrehoz egy agentet az adott elen
	 * @param edge az el amin eloszor vegig megy az agent
	 * @param start a kezdo node
	 * @param target a cel node
	 * @param startTime az indulas ideje
	 */
	Agent(Edge& edge, const Node& start, const Node& target, Time startTime);

	/*!
	 * Letrehoz egy agentet egy masik agentbol
	 * @param firstEdge ha megadva lecsereli az utolso elet erre az elre
	 */
	Agent(const Agent&, Edge* firstEdge = NULL);

	/*!
	 * Egyszer lepteti az agentet
	 * @return az agent uj allapota
	 */
	AgentState step();

	/*!
	 * @return A legutobbi el altal mutatott csucs, az agent feje ha egy kigyokent nezzuk
	 */
	const Node* head() const;
};

/*!
 * Agenteket hasznalo utvonaltervezo
 */
class AgentPathfinder : public Pathfinder {
	/*!
	 * Az agenteket tartalmazo "rendezett" lista
	 * Uj agentek mindig a lista eleje lesznek teve, hogy gyoes legyen
	 */
	SortedList<Agent*, alwaysFirst<Agent*>> agents;

	/*!
	 * Torlendo agentek, amik a kovetkezo lepesek elott torolve leszenek
	 * /see ::deleteAgent
	 */
	Array<Agent*> agentsToDelete;

	/*!
	 * Duplikal egy agentet az agent fejen levo osszes elre
	 * @param startTime a fejbe eres ideje
	 * @param startEdge az el amin az eredeti agent tovabb ment
	 */
	void splitAgent(const Agent&, Time startTime, size_t startEdge = 1);

	/*!
	 * Torlesre elokeszit egy agentet
	 * szukseges, mert a listat modositani iteralas kozben veszelyes lenne
	 * @param agent a torlendo agent
	 */
	void deleteAgent(Agent* agent);
	
public:
	/*!
	 * see Pathfinder::Pathfinder()
	 */
	AgentPathfinder(Graph graph, size_t numRoutes = 3);

	/*!
	 * /see Pathfinder::getRoutes()
	 * @return a visszaadott listaban levo utvonalak dinamikusan taroltak es a hivo kezeli
	 * @exception NotFound ha to vagy from nincs a grafban
	 */
	SortedList<Route*> getRoutes(const Node& from, const Node& to, Time starTime);

	~AgentPathfinder();
};
#endif
