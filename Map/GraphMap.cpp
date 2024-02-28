#include "GraphMap.h"

int GraphMap::getWidth() const
{
    return width;
}

void GraphMap::setWidth(int newWidth)
{
    width = newWidth;
}

int GraphMap::getHeight() const
{
    return height;
}

void GraphMap::setHeight(int newHeight)
{
    height = newHeight;
}

void GraphMap::save(QFile &out)
{
    out.write((char*)& width, sizeof(width));
    out.write((char*)& height, sizeof(height));

    size_t size = walls.size();
    out.write((char*)& size, sizeof(size));
    for(auto& i: walls)
    {
        out.write((char*)& i, sizeof(i));
    }

    size = map.size();
    out.write((char*)& size, sizeof(size));
    for(auto& i : map) {
        out.write((char*)& i.first, sizeof(i.first));
        i.second.save(out);
    }
}

void GraphMap::load(QFile &in)
{
    in.read((char*)& width, sizeof(width));
    in.read((char*)& height, sizeof(height));

    size_t size;

    in.read((char*)& size, sizeof(size));
    walls.clear();
    for(int i = 0; i < size; i++) {
        Location location;
        in.read((char*)& location, sizeof(location));
        walls.insert(location);
    }

    in.read((char*)& size, sizeof(size));
    map.clear();
    for(int i = 0; i < size; i++) {
        Location location;
        in.read((char*)& location, sizeof(location));
        MapCell map_cell;
        map_cell.load(in);
        map.insert(std::make_pair(location, map_cell));
    }
}

GraphMap::GraphMap(int width, int height) : width(width), height(height) {
    int left = 0;
    int top = 0;

    for (int q = left; q < width; q++) {
        int q_offset = floor(q / 2.0);
        for (int r = top - q_offset; r < height - q_offset; r++) {
            map.emplace(Location{ q, r }, MapCell());
        }
    }
}

std::unordered_map<Location, MapCell>& GraphMap::getMap() {
    return map;
}

std::unordered_set<Location> GraphMap::getWalls() const {
    return walls;
}

void GraphMap::addWall(const Location &location) {
    if (walls.find(location) != walls.end())
        walls.erase(location);
    else
        walls.insert(location);
}

bool GraphMap::inBounds(const Location &location) const {
    return map.find(location) != map.end();
}

bool GraphMap::isPossible(const Location &location) const {
    return walls.find(location) == walls.end();
}

std::vector<Location> GraphMap::neighbours(const Location &location) {
    std::vector<Location> result;
    for (int i = 0; i < 6; i++) {
        Location neighbour = Coordinates::neighbour(location, (Coordinates::Direction)i);
        if (inBounds(neighbour) && isPossible(location))
            result.push_back(neighbour);
    }

    return result;
}

int GraphMap::cost(const Location &destination) {
    return map[destination].get_type_of_terrain();
}

void GraphMap::aStarSearch(Location start, Location destination) {
    cameFrom.clear();
    costSoFar.clear();

    PriorityQueue<Location, double> frontier;
    frontier.put(start, 0);

    cameFrom[start] = start;
    costSoFar[start] = 0;

    while (!frontier.empty()) {
        Location current = frontier.get();

        if (current == destination) {
            break;
        }

        for (Location next : neighbours(current)) {
            double new_cost = costSoFar[current] + cost(next);
            if (costSoFar.find(next) == costSoFar.end() || new_cost < costSoFar[next]) {
                costSoFar[next] = new_cost;
                double priority = new_cost + heuristic(next, destination);
                frontier.put(next, priority);
                cameFrom[next] = current;
            }
        }
    }
}

std::vector<Location> GraphMap::reconstruct_path(const Location &start, const Location &destination) {
    std::vector<Location> path;
    if (walls.find(destination) != walls.end())
        return path;

    Location current = destination;
    if (cameFrom.find(destination) == cameFrom.end()) {
        return path;
    }
    while (current != start) {
        path.push_back(current);
        current = cameFrom[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
