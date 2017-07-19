#pragma once

class Geometry {
	struct Intersection {
		int xpoint;
		int ypoint;
	};
public:
	bool intersects(int x, int y);
	Intersection getIntersection(int x, int y);
};
