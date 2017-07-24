#pragma once
#include "Camera.h"
#include "SDL.h"

/* Basic Line struct for collision management & debug collision geometry rendering. */
struct Line {
    //p1 is the start point, p2 is the endpoint.
    Vector2 p1,p2;
    Line();
    Line(double a,double b,double c,double d);
    void render(const Camera& c) const;
    //Orthogonally projects p onto the line. p is set equal to (p2-p1)*t+p1 (linear interpolation). The 
    //point p is on the line segment if and only if 0<=t<=1. 
    void project(Vector2 &p,double &t) const;
};

struct Rectangle {
    double x0,y0,x1,y1;
    Rectangle();
    Rectangle(double a,double b,double c,double d);
    double getWidth();
    double getHeight();
    void render(const Camera& c);
};

//Pushes a circle so it is no longer intersecting with a point. So (  .)  -> (   ). :)
//Updates the value of circlepos to the new position.
void circlePointCollide(Vector2 &circlepos,double r,Vector2 point);

//Pushes a circle so it is no longer intersecting with a line segment or its endpoints.
//Updates the value of circlepos to the new position.
void circleLineCollide(Vector2 &circlepos,double r,Line line);

//Pushes a circle so it is no longer intersecting with a rectangle
//Updates the value of circlepos to the new position.
void circleRectangleCollide(Vector2 &circlepos,double r,Rectangle rect);

//Pushes a circle so it is no longer intersecting with a circle
//Updates the value of c1 to the new position.
void circleCircleCollide(Vector2 &c1, double r1, Vector2 c2, double r2);

//Pushes two circles to make sure they are no longer intersecting.
//Pushing conserves "center of mass", c1*r1^2+c2*r2^2. This means big circles
//push small circles more easily, and it's difficult for small circles to push big circles.
//Updates the value of c1 and c2 to the new positions.
void circleCirclePush(Vector2 &c1,double r1,Vector2 &c2,double r2);
