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

class Circle : Geometry {
public:
	bool intersects(int x, int y);
	Intersection getIntersection(int x, int y);
};

class Rectangle : Geometry {
public:
	bool intersects(int x, int y);
	Intersection getIntersection(int x, int y);
};