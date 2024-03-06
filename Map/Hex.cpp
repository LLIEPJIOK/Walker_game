#include "Hex.h"
#include "GraphMap.h"

__declspec(selectany) Coordinates::Cube<int> Coordinates::cubeDirections[6] = {
    { 0, 1,  -1 }, { 1,  0, -1 }, { 1, -1, 0 },
    { 0, -1,  1 }, { -1,  0, +1 }, { -1, 1, 0 }
};

__declspec(selectany) Coordinates::Hex<int> Coordinates::hexDirections[6] = {
    { 0, -1 }, { 1, -1 }, { 1, 0 },
    { 0, 1 }, { -1, 1 }, { -1, 0 }
};

int HexMathOnScreen:: size = 50;

Coordinates::Hex<int> Coordinates::neighbour(const Hex<int> &coordinate, Direction direction) {
    return coordinate + hexDirections[direction];
}

Coordinates::Cube<int> Coordinates::neighbour(const Cube<int> &coordinate, Direction direction) {
    return coordinate + cubeDirections[direction];
}

int Coordinates::distance(const Cube<int> &first, const Cube<int> &second) {
    return (std::abs(first.x - second.x) + std::abs(first.y - second.y) + std::abs(first.z - second.z)) / 2;
}

int Coordinates::distance(const Hex<int> &first, const Hex<int> &second) {
    return distance(hexToCube(first), hexToCube(second));
}

std::unordered_set<Coordinates::Hex<int> > Coordinates::range(const Hex<int> &center, int distance, const GraphMap* graph_map) {
    std::unordered_set<Hex<int>> result;

    for (int x = -distance; x <= distance; x++) {
        for (int y = max(-distance, -x - distance); y <= min(distance, -x + distance); y++) {
            if(graph_map->inBounds(center + Hex<int>{ x, y }) && graph_map->isPossible(center + Hex<int>{ x, y }))
                result.insert(center + Hex<int>{ x, y });
        }
    }
    return result;
}

Coordinates::Cube<int> Coordinates::scale(const Cube<int> &cube, int coefficient) {
    return {cube.x * coefficient, cube.y * coefficient, cube.z * coefficient};
}

Coordinates::Hex<int> Coordinates::scale(const Hex<int> &hex, int coefficient) {
    return { hex.q * coefficient, hex.r * coefficient };
}

std::vector<Coordinates::Hex<int> > Coordinates::ring(const Hex<int> &center, int radius, const GraphMap* graph_map) {
    std::vector<Hex<int>> result;
    Hex hex = center + scale(hexDirections[4], radius);

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < radius; j++) {
            if(graph_map->inBounds(hex) && graph_map->isPossible(hex))
                result.push_back(hex);
            hex = neighbour(hex, (Direction)i);
        }
    }

    return result;
}

QPointF HexMathOnScreen::hexCorner(const QPointF &center, int i) {
    double angleDegree = 60 * i;
    double angleRadiance = M_PI / (double)180 * angleDegree;
    return QPoint(((double)center.x() + ((double)size * cos(angleRadiance))), ((double)center.y() + ((double)size * sin(angleRadiance))));
}

std::vector<QPointF> HexMathOnScreen::corners(const QPointF &center) {
    std::vector<QPointF> result;
    for (int i = 0; i < 6; i++) {
        result.push_back(hexCorner(center, i));
    }

    return result;
}

QPointF HexMathOnScreen::hexToPixel(const Coordinates::Hex<int> &hex) {
    double x = (double)size * 3 * (double)hex.q / 2;
    double y = (double)size * std::sqrt(3) * ((double)hex.r + (double)hex.q / 2);
    return { x, y };
}

Coordinates::Hex<int> HexMathOnScreen::pixelToHex(const QPoint &point) {
    double q = (double)point.x() * 2 / 3 / (double)size;
    double r = ((double)-point.x() / 3 + std::sqrt(3) / 3 * (double)point.y()) / (double)size;
    return hexRound(Coordinates::Hex<double>{q, r});
}

Coordinates::Hex<int> HexMathOnScreen::hexRound(Coordinates::Hex<double> hex) {
    return Coordinates::cubeToHex(cubeRound(Coordinates::hexToCube(hex)));
}

Coordinates::Cube<int> HexMathOnScreen::cubeRound(Coordinates::Cube<double> cube) {
    int rx = std::round(cube.x);
    int ry = std::round(cube.y);
    int rz = std::round(cube.z);

    double xDiff = std::fabs((double)rx - cube.x);
    double yDiff = std::fabs((double)ry - cube.y);
    double zDiff = std::fabs((double)rz - cube.z);

    if (xDiff > yDiff && xDiff > zDiff)
        rx = -ry - rz;
    else if (yDiff > zDiff)
        ry = -rx - rz;
    else
        rz = -rx - ry;

    return (Coordinates::Cube<int>{ rx, ry, rz });
}
