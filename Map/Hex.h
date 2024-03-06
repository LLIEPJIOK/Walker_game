#pragma once


class GraphMap;

class Coordinates {
public:

	template <class C>
	struct Cube;
	template <class H>
	struct Hex;
	
    enum Direction {
		North,
		NorthEast,
		SouthEast,
		South,
		SouthWest,
		NorthWest
	};

	static Cube<int> cubeDirections[6];
	static Hex<int> hexDirections[6];
	
	template <class C>
	struct Cube {
		C x;
		C y;
		C z;


		Cube operator + (const Cube& cube) const {
			return { x + cube.x, y + cube.y, z + cube.z };
		}

		bool operator == (const Cube& cube) const {
			return x == cube.x && y == cube.y && z == cube.z;
		}

		bool operator != (const Cube& cube) const {
			return !(*this == cube);
		}
	};

	template <class H>
	struct Hex {
		H q;
		H r;

		Hex operator + (const Hex& hex) const {
			return { q + hex.q, r + hex.r };
		}

		bool operator == (const Hex& hex) const {
			return q == hex.q && r == hex.r;
		}

		bool operator != (const Hex& hex) const {
			return !(*this == hex);
		}

		bool operator < (const Hex& hex) const {
			return std::pair<H, H>(q, r) < std::pair<H, H>(hex.q, hex.r);
		}
	};

	template <class CTH>
	static Hex<CTH> cubeToHex(const Cube<CTH>& cubeCoordinate) {
		return { cubeCoordinate.x, cubeCoordinate.z };
	}

	template <class HTC>
	static Cube<HTC> hexToCube(const Hex<HTC>& hexCoordinate) {
		return { hexCoordinate.q,  -hexCoordinate.q - hexCoordinate.r, hexCoordinate.r};
	}

    static Hex<int> neighbour(const Hex<int>& coordinate, Direction direction);

    static Cube<int> neighbour(const Cube<int>& coordinate, Direction direction);


    static int distance(const Cube<int>& first, const Cube<int>& second);

    static int distance(const Hex<int>& first, const Hex<int>& second);

    static std::unordered_set<Hex<int>> range(const Hex<int>& center, int distance, const GraphMap* graph_map);

    static Cube<int> scale(const Cube<int>& cube, int coefficient);

    static Hex<int> scale(const Hex<int>& hex, int coefficient);

    static std::vector<Hex<int>> ring(const Hex<int>& center, int radius, const GraphMap* graph_map);
}; 


class HexMathOnScreen {
public:

    static int size;
	static constexpr auto PI = 3.1415;

    static QPointF hexCorner(const QPointF& center, int i);

    static std::vector<QPointF> corners(const QPointF& center);

    static QPointF hexToPixel(const Coordinates::Hex<int>& hex);

    static Coordinates::Hex<int> pixelToHex(const QPoint& point);

    static Coordinates::Hex<int> hexRound(Coordinates::Hex<double> hex);

    static Coordinates::Cube<int> cubeRound(Coordinates::Cube <double> cube);
};

namespace std {
	template <> struct hash<Coordinates::Hex<int>> {
		size_t operator()(const Coordinates::Hex<int>& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.q);
			size_t hr = int_hash(h.r);
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}
