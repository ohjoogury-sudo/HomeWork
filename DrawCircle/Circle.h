#pragma once
class Circle
{
private:
	double m_x; 
	double m_y; 
	double m_radius; 
	BYTE m_color; 
	int m_width; 
	bool m_fill; 
public:
	Circle(double x = 0, double y = 0, double radius = 0, BYTE color = 0, int width = 1, bool fill = false)
		: m_x(x), m_y(y), m_radius(radius), m_color(color), m_width(width), m_fill(fill) {}
	double getX() const { return m_x; }
	double getY() const { return m_y; }
	double getRadius() const { return m_radius; }
	BYTE getColor() const { return m_color; }
	int getWidth() const { return m_width; }
	bool isFill() const { return m_fill; }
	void setX(double x) { m_x = x; }
	void setY(double y) { m_y = y; }
	void setRadius(double radius) { m_radius = radius; }
	void setColor(BYTE color) { m_color = color; }
	void setWidth(int width) { m_width = width; }
	void setFill(bool fill) { m_fill = fill; }
	bool isValid() const { return m_radius > 0; } 
	static Circle fromPoints(CPoint p1, CPoint p2, CPoint p3, BYTE color = 0, int width = 1, bool fill = false); 
	bool inInCircle(double x, double y);
};

