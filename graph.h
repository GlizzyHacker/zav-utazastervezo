#include "mytime.h"
#include "array.hpp"

#ifndef GRAPH
#define GRAPH

/*!
 * Kivetel arra, ha valami nem szerepel valamiben
 */
class NotFound : public std::exception {
	/*!
	 * A kivetelt leiro c-string
	 */
	char* whatStr;
public:
	/*!
	 * @param obj annak a neve amit nem talat c-stringkent
	 */
	NotFound(const char* obj);

	virtual const char* what() const throw();

	virtual ~NotFound();
};

/*!
 * Egy graf iranyitott ele
 */
class Edge;

/*!
 * Egy graf csucsa
 */
class Node {
protected:
	/*!
	 * A csucs elei
	 */
	Array<Edge*> edges;

	/*!
	 * A csucs neve
	 */
	char* name;
public:
	/*!
	 * Letrehoz egy csucsot
	 * @param edges a csucs elei
	 * @param name a csucs neve
	 */
	Node(Array<Edge*> edges, const char* name);

	/*!
	 * Letrehoz egy izolalt pontot
	 */
	Node();

	/*!
	 * @return A graf neve c-stringkent
	 */
	const char* getName() const;

	/*!
	 * @return A graf eleit tartalmazo Arra
	 */
	Array<Edge*> getEdges() const;

	/*!
	 * Letrehoz egy csucsot egy masik masolatakent
	 * @param other a jobb ertek
	 * @return ez a csucs
	 */
	Node& operator=(const Node& other);

	virtual ~Node();
};

class Edge {
protected:
	/*!
	 * A csucs amibol indul az el
	 */
	Node* from;

	/*!
	 * A csucs amibe megy az el
	 */
	Node* to;

	/*!
	 * A csucs sulya
	 */
	size_t weight;

	/*!
	 * Az el indulasi idejei
	 * Tobb van mert egy el tobszor is indulhat egy nap
	 */
	Array<Time> startTimes;
	/*!
	 * Az el neve
	 */
	char* name;

public:
	/*!
	 * Letrehoz egy elt
	 * @param from az el ebbol a csucsbol indul
	 * @param to az el ebbe a csucsba megy
	 * @param weight az el sulya
	 * @param startTimes az el indulasi ideje
	 * @param name az el neve
	 */
	Edge(Node* from, Node* to, int weight, Array<Time> startTimes, const char* name);

	Edge();

	/*!
	 * @return az el neve c-stringkent
	 */
	const char* getName() const;

	/*!
	 * Megadja az el sulyat es az indulasaig vart idot
	 * @param currentTime az ido amitol var
	 * @return a suly es vart ido osszege percben
	 */
	size_t getWeight(Time currentTime) const;

	/*!
	 * @return az el sulya
	 */
	size_t getWeight() const;

	/*!
	 * Megadja az elso indulasi idot egy adott ido utan
	 * @param time az ido ami utan keres
	 * @return a legkorabbi indulasi ido
	 */
	Time getFirstStartTimeAfter(Time time) const;

	/*!
	 * @return az csucs ahova az el megy
	 */
	Node* getToNode() const;

	/*!
	 * Letrehoz egy elt egy masik masolatakent
	 * @param other a jobb ertek
	 * @return ez az el
	 */
	Edge& operator=(const Edge& other);

	virtual ~Edge();
};

/*!
 * Utvonaltervezesben hasznalt graf
 */
class Graph {
protected:
	/*!
	 * a graf elei
	 */
	Array<Node*> nodes;
public:
	/*!
	 * letrehoz egy ures grafot
	 */
	Graph();

	/*!
	 * Letrehoz egy grafot
	 * @param nodes a graf csucsai
	 */
	Graph(Array<Node*> nodes);

	/*!
	 *
	 * @return A graf csucsai
	 */
	Array<Node*> getNodes() const;

	/*!
	 * Megkeresi a graf egyik csucsat nev alapjan
	 * alapbol nem teljes egyezes kell, de a megadott stringnek benne kell lennie a nevben
	 * @param name a keresett nev c-stringkent
	 * @param exactMatch ha igaz akkor teljes egyezes kell
	 * @return a csucs aminek ez a neve
	 * @exception NotFound ha nem talalja a csucsot
	 */
	Node* getNode(const char* name, bool exactMatch = false) const;
};

/*!
 * Egy ut egy grafban
 */
class Route {
	/*!
	 * Az ut kezdesi ideje
	 */
	Time startTime;
protected:
	/*!
	 * Az ut altal erintett elek sorrendben
	 */
	Array<Edge*> edges;
public:
	/*!
	 * Letrehoz egy utat
	 * @param edges az ut elei
	 * @param startTime a kezdesi ido
	 */
	Route(Array<Edge*> edges, Time startTime);

	/*!
	 *
	 * @return Az ut elei
	 */
	Array<Edge*> getEdges() const;

	/*!
	 *
	 * @return Az ut kezdesi ideje
	 */
	Time getStartTime() const;

	/*!
	 * Az ut osszes elen vegig menve enny ido lenne varakozasokkal eggyutt
	 * @return az osszsuly percben
	 */
	size_t getTotalWeight() const;
};

#endif
