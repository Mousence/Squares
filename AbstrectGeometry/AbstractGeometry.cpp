#include<iostream>
#include<Windows.h>
using namespace std;

#define delimiter "\n------------------\n"


enum Color
{
	red = 0x000000FF,
	green = 0x0000FF00,
	blue = 0x00FF0000,
	yellow = 0x0000FFFF,
	purple = 0x00FF00FF,
	white = 0x00FFFFFF
};

#define SHAPE_TAKE_PARAMETERS Color color, int start_x, int start_y, int line_width
#define SHAPE_GIVE_PARAMETERS color, start_x, start_y, line_width
class Shape
{
protected:
	static const int MIN_START_X = 10;
	static const int MAX_START_X = 800;

	static const int MIN_START_Y = 10;
	static const int MAX_START_Y = 500;

	static const int MIN_LINE_WIDTH = 1;
	static const int MAX_LINE_WIDTH = 30;

	static const int MIN_DIMENSION = 25;
	static const int MAX_DIMENSION = 500;
	Color color;
	int start_x;
	int start_y;
	int line_width;

	BOOL(__stdcall* DrawingFunction)(HDC, int, int, int, int);
	int dimensions[2]{};
public:
	Color get_color()const
	{
		return color;
	}
	int get_start_x()const
	{
		return start_x;
	}
	int get_start_y()const
	{
		return start_y;
	}
	int get_line_width()const
	{
		return line_width;
	}
	void set_color(Color color)
	{
		this->color = color;
	}
	void set_start_x(int start_x)
	{
		if (start_x < MIN_START_X)start_x = MIN_START_X;
		if (start_x > MAX_START_X)start_x = MAX_START_X;
		this->start_x = start_x;
	}
	void set_start_y(int start_y)
	{
		if (start_y < MIN_START_Y) start_y = MIN_START_Y;
		if (start_y > MAX_START_Y) start_y = MAX_START_Y;
		this->start_y = start_y;
	}
	void set_line_width(int line_width)
	{
		if (line_width < MIN_LINE_WIDTH)line_width = MIN_LINE_WIDTH;
		if (line_width > MAX_LINE_WIDTH)line_width = MAX_LINE_WIDTH;
		this->line_width = line_width;
	}
	virtual double get_area()const = 0;
	virtual double get_perimeter()const = 0;
	//virtual void draw()const = 0;
	virtual void draw()const
	{
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);

		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		DrawingFunction(hdc, start_x, start_y, start_x + dimensions[0], start_y + dimensions[1]);

		DeleteObject(hPen);
		DeleteObject(hBrush);

		ReleaseDC(hwnd, hdc);
	}
	Shape(SHAPE_TAKE_PARAMETERS)
	{
		set_color(color);
		set_start_x(start_x);
		set_start_y(start_y);
		set_line_width(line_width);
	}
	~Shape() {}
	void info()const
	{
		cout << "Площадь фигуры: " << get_area() << endl;
		cout << "Периметр фигуры: " << get_perimeter() << endl;
		draw();
	}
};

class Rectangle :public Shape
{
	double side_a;
	double side_b;
public:
	double get_side_a()const
	{
		return side_a;
	}
	double get_side_b()const
	{
		return side_b;
	}
	void set_side_a(double side_a)
	{
		if (side_a < MIN_DIMENSION)side_a = MIN_DIMENSION;
		if (side_a > MAX_DIMENSION)side_a = MAX_DIMENSION;
		this->side_a = side_a;
	}
	void set_side_b(double side_b)
	{
		if (side_b < MIN_DIMENSION)side_b = MIN_DIMENSION;
		if (side_b > MAX_DIMENSION)side_b = MAX_DIMENSION;
		this->side_b = side_b;
	}
	double get_diagonal()const
	{
		return sqrt(side_a * side_a + side_b * side_b);
	}
	double get_area()const override
	{
		return side_a * side_b;
	}
	double get_perimeter()const override
	{
		return (side_a + side_b) * 2;
	}
	/*void draw()const override
	{
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);

		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		::Rectangle(hdc, start_x, start_y, start_x + side_a, start_y + side_b);

		DeleteObject(hPen);
		DeleteObject(hBrush);

		ReleaseDC(hwnd, hdc);
	}*/

	Rectangle(double side_a, double side_b, SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS)
	{
		set_color(color);
		set_start_x(start_x);
		set_start_y(start_y);
		set_side_a(side_a);
		set_side_b(side_b);
		DrawingFunction = ::Rectangle;
	}
	~Rectangle() {}

	void info()const
	{
		cout << typeid(*this).name() << endl;
		cout << "Сторона А: " << get_side_a() << endl;
		cout << "Сторона B: " << get_side_b() << endl;
		cout << "Диагональ: " << get_diagonal() << endl;
		Shape::info();
	}
};
class Square :public Rectangle
{
public:
	Square(double side, SHAPE_TAKE_PARAMETERS) :Rectangle(side, side, SHAPE_GIVE_PARAMETERS) {}
	~Square() {}
};

class Circle :public Shape
{
	double radius;
public:
	double get_radius()const
	{
		return radius;
	}
	void set_radius(double radius)
	{
		if (radius < MIN_DIMENSION)radius = MIN_DIMENSION;
		if (radius > MAX_DIMENSION)radius = MAX_DIMENSION;
		this->radius = radius;
	}
	double get_diameter()const
	{
		return 2 * radius;
	}
	double get_area()const override
	{
		return 3,14 * pow(radius, 2);
	}
	double get_perimeter()const override
	{
		return 3,14 * 2 * radius;
	}
	void draw()const override
	{
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);

		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBush);

		::Ellipse(hdc, start_x, start_y, start_x + get_diameter(), start_y + get_diameter());

		DeleteObject(hPen);
		DeleteObject(hBush);

		ReleaseDC(hwnd, hdc);
	}

	Circle(double radius, SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS)
	{
		set_radius(radius);
		DrawingFunction = ::Ellipse;
	}
	~Circle() {}

	void info()const
	{
		cout << typeid(*this).name() << endl;
		cout << "Радиус круга:  " << get_radius() << endl;
		cout << "Диаметр круга: " << get_diameter() << endl;
		Shape::info();
	}
};

class Triangle :public Shape
{
public:
	virtual double get_height()const = 0;
	Triangle(SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS) {}
	~Triangle() {}
	void info()const
	{
		cout << "Высота треугольника: " << get_height() << endl;
		Shape::info();
	}
};
class EquilateralTriangle :public Triangle
{
	double side;
public:
	double get_side()const
	{
		return side;
	}
	void set_side(double side)
	{
		if (side < MIN_DIMENSION)side = MIN_DIMENSION;
		if (side > MAX_DIMENSION)side = MAX_DIMENSION;
		this->side = side;
	}
	double get_height()const override
	{
		return sqrt(pow(side, 2) - pow(side / 2, 2));
	}
	double get_area()const override
	{
		return pow(get_height(), 2) * sqrt(3);
	}
	double get_perimeter()const override
	{
		return side * 3;
	}
	void draw()const override
	{
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);

		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		POINT vertex[] =
		{
			{ start_x, start_y + side },
			{ start_x + side, start_y + side },
			{ start_x + side / 2, start_y + side - get_height() }
		};
		::Polygon(hdc, vertex, 3);

		DeleteObject(hPen);
		DeleteObject(hBrush);

		ReleaseDC(hwnd, hdc);
	}

	EquilateralTriangle(double side, SHAPE_TAKE_PARAMETERS) :Triangle(SHAPE_GIVE_PARAMETERS)
	{
		set_side(side);
	}
	~EquilateralTriangle() {}

	void info()const
	{
		cout << typeid(*this).name() << endl;
		cout << "Длина стороны: " << side << endl;
		Triangle::info();
	}
};
class RightTriangle :public Triangle {
	double side_a;
	double side_b;
	double side_c;
public:
	double get_side_a()const {
		return side_a;
	}
	double get_side_b()const {
		return side_b;
	}
	void set_side_b(double side_b) {
		this->side_b = side_b;
	}
	void set_side_a(double side_a) {
		this->side_a = side_a;
	}

	RightTriangle(double side_a, double side_b, SHAPE_TAKE_PARAMETERS) : Triangle(SHAPE_GIVE_PARAMETERS) {
		set_side_a(side_a);
		set_side_b(side_b);
		this->side_c = sqrt(pow(side_a, 2) * pow(side_b, 2));
	}
	~RightTriangle() {}

	double get_height()const override
	{
		return (side_a + side_b) / side_c;
	}
	double get_area()const override
	{
		return (side_a + side_b) / 2;
	}
	double get_perimeter()const override
	{
		return side_a + side_b + side_c;
	}
	void draw()const override
	{
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);

		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		POINT vertex[] =
		{
			{ start_x, start_y + static_cast<int>(side_a) },
			{ start_x + static_cast<int>(side_b), start_y + static_cast<int>(side_a) },
			{ start_x, start_y }
		};
		Polygon(hdc, vertex, 3);

		DeleteObject(hPen);
		DeleteObject(hBrush);

		ReleaseDC(hwnd, hdc);
	}
	void info() {
		cout << typeid(*this).name() << endl;
		cout << "Гипотенуза равна: " << side_c << endl;
		Triangle::info();
	}
};

void main()
{
	setlocale(LC_ALL, "");
	Square square(150, Color::red, 300, 10, 5);
	square.info();
	cout << delimiter << endl;

	class Rectangle rect(250, 150, Color::blue, 300, 210, 5);
	rect.info();
	cout << delimiter << endl;

	Circle circle(150, Color::yellow, 900, 10, 5);
	circle.info();
	cout << delimiter << endl;

	//EquilateralTriangle ET(150, Color::green, 700, 300, 15);
	//ET.info();

	RightTriangle RT(150, 150, Color::white, 700, 300, 15);
	RT.info();
}