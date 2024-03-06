#pragma once
#include "Hex.h"
#include "mapcell.h"

template<typename T, typename priority_t>
struct PriorityQueue {
	typedef std::pair<priority_t, T> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>,
		std::greater<PQElement>> elements;

	inline bool empty() const {
		return elements.empty();
	}

	inline void put(T item, priority_t priority) {
		elements.emplace(priority, item);
	}

	T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

typedef Coordinates::Hex<int> Location;

class GraphMap {
private:
	int width;
	int height;

	std::unordered_set<Location> walls;
    std::unordered_map<Location, MapCell> map;

	std::unordered_map<Location, Location> cameFrom;
	std::unordered_map<Location, double> costSoFar;

public:
    GraphMap(int width, int height);
    GraphMap() = default;

    std::unordered_map<Location, MapCell>& getMap();

    std::unordered_set<Location> getWalls() const;

    void addWall(const Location& location);


    bool inBounds(const Location& location) const;

    bool isPossible(const Location& location) const;

    std::vector<Location> neighbours(const Location& location);

	inline double heuristic(Location a, Location b) {
        return Coordinates::distance(a, b);
	}

    int cost(const Location& destination);

    void aStarSearch(Location start, Location destination);

    std::vector<Location> reconstruct_path(const Location& start, const Location& destination);
    int getWidth() const;
    void setWidth(int newWidth);
    int getHeight() const;
    void setHeight(int newHeight);


    void save(QFile& out);
    void load(QFile& in);
};
