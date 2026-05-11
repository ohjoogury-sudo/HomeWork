#include "pch.h"
#include "Circle.h"

Circle Circle::fromPoints(CPoint p1, CPoint p2, CPoint p3, BYTE color, int width, bool fill) 
{
	double area = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
	if (fabs(area) < 1e-9) {
		return Circle();
	}
	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;
	double d = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	double h = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / d;
	double k = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / d;
	double r = sqrt(pow(x1 - h, 2) + pow(y1 - k, 2));
	return Circle(h, k, r, color, width, fill);
}

bool Circle::inInCircle(double x, double y)
{
	double dx = x - m_x;
	double dy = y - m_y;
	double d2xy = dx * dx + dy * dy;
	return d2xy <= m_radius * m_radius;
}