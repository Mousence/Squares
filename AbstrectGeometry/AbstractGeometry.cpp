#include <iostream>
#include <Windows.h>
using namespace std;

enum Color {
	red = 0x000000FF,
	green = 0x0000FF00,
	blue = 0x00FF0000,
	yellow = 0x0000FFFF,
	purple = 0x00FF00FF,
	white = 0x00FFFFFF
};

#define delimiter "---------------------------------------------"

#define Pi 3,14
#define SHAPE_TAKE_PARAMETERS Color color, int start_x, int start_y, int line_width
#define SHAPE_GIVE_PARAMETERS color, start_x, start_y, line_width
//#define SQUARE_INDEPENDENT
class Shape {
public:
	static const int MIN_DIMENSION = 25;
	static const int MAX_DIMENSION = 500;

	static const int MIN_START_X = 10;
	static const int MAX_START_X = 800;

	static const int MIN_LINE_WIDTH = 1;
	static const int MAX_LINE_WIDTH = 30;

	static const int MIN_START_Y = 10;
	static const int MAX_START_Y = 500;

protected:
	Color color;
	int start_x;
	int start_y;
	int line_width;

	BOOL(__stdcall* DrawingFunction)(HDC, int, int, int, int);
	int dimensions[2]{};

public:
	Color get_color()const {
		return color;
	}
	int get_start_x()const {
		return start_x;
	}
	int get_start_y()const {
		return start_y;
	}
	int get_line_width()const {
		return line_width;
	}
	void set_color(Color color) {
		this->color = color;
	}
	void set_start_x(double start_x) {
		if (start_x < MIN_START_X) start_x = MIN_START_X;
		if (start_y < MAX_START_X) start_x = MAX_START_X;
		this->start_x = start_x;
	}
	void set_start_y(double start_y) {
		if (start_y < MIN_START_Y) start_y = MIN_START_Y;
		if (start_y < MAX_START_Y) start_y = MAX_START_Y;
		this->start_y = start_y;
	}
	void set_line_width(double line_width) {
		if (line_width < MIN_LINE_WIDTH) line_width = MIN_LINE_WIDTH;
		if (line_width < MAX_LINE_WIDTH) line_width = MAX_LINE_WIDTH;
		this->line_width = line_width;
	}
	virtual double get_area()const = 0;
	virtual double get_perimeter()const = 0;
	//virtual void draw()const = 0;
	virtual void draw()const {
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
	Shape(SHAPE_TAKE_PARAMETERS) {
		set_color(color);
		set_start_x(start_x);
		set_start_y(start_y);
		set_line_width(line_width);
		this->DrawingFunction = DrawingFunction;
	}
	~Shape() {}
	void info()const {
		cout << "Площадь фигуры: " << get_area() << endl;
		cout << "Периметр: " << get_perimeter() << endl;
		draw();
	}

};
#ifdef SQUARE_INDEPENDENT
class Square :public Shape {
	double side;
public:
	double get_side()const {
		return side;
	}
	void set_side(double side) {
		if (side < 1) side = 1;
		this->side = side;
	}

	double get_area()const override {
		return side * side;
	}
	double get_perimeter()const override {
		return side * 4;
	}
	void draw()const override {
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);
		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		Rectangle(hdc, start_x, start_y, start_x + side, start_y + side);

		DeleteObject(hBrush);
		DeleteObject(hPen);

		ReleaseDC(hwnd, hdc);
	}
	Square(double side, SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS) {
		set_side(side);
	}
	~Square() {}
	void info() const {
		cout << typeid(*this).name() << endl;
		cout << "Длина стороны: " << get_side() << endl;
		Shape::info();
	}
};

#endif //SQUARE_INDEPENDENT
class Rectangle :public Shape {
protected:
	double side_a;
	double side_b;
public:
	double get_side_a() {
		return side_a;
	}
	double get_side_b() {
		return side_b;
	}
	void set_side_a(double side_a) {
		if (side_a > MAX_DIMENSION) side_a = MAX_DIMENSION;
		if (side_a < MIN_DIMENSION) side_a = MIN_DIMENSION;
		this->side_a = side_a;
	}
	void set_side_b(double side_b) {
		if (side_b > MAX_DIMENSION) side_b = MAX_DIMENSION;
		if (side_b < MIN_DIMENSION) side_b = MIN_DIMENSION;
		this->side_b = side_b;
	}
	double get_diagonal() {
		return sqrt(side_a * side_a + side_b * side_b);
	}
	double get_area()const override {
		return side_a * side_b;
	}
	double get_perimeter()const override {
		return (side_a + side_b) * 2;
	}

	void draw()const override {
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
	}

	Rectangle(double side_a, double side_b, SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS) {
		set_color(color);
		set_start_x(start_x);
		set_start_y(start_y);
		set_side_a(side_a);
		set_side_b(side_b);
		DrawingFunction = ::Rectangle;
	}
	~Rectangle() {}

	void info() {
		cout << typeid(*this).name() << endl;
		cout << "Сторона A:\t" << get_side_a() << endl;
		cout << "Сторона B:\t" << get_side_b() << endl;
		cout << "Периметр: \t" << get_perimeter() << endl;
		Shape::info();
	}
};
class Square :public Rectangle {
public:
	Square(double side, SHAPE_TAKE_PARAMETERS) :Rectangle(side, side, SHAPE_GIVE_PARAMETERS) {}
	~Square() {}

};

class Circle :public Shape {
protected:
	double radius;
public:
	double get_radius() const {
		return radius;
	}
	void set_radius(double radius) {
		if (radius < MIN_DIMENSION) radius = MIN_DIMENSION;
		if (radius > MAX_DIMENSION) radius = MAX_DIMENSION;
		this->radius = radius;
	}
	Circle(double radius, SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS) {
		set_radius(radius);
		DrawingFunction = ::Ellipse;
	}
	~Circle() {}

	double get_area() const override {
		return 2 * Pi * pow(radius, 2);
	}
	double get_perimeter() const override {
		return 2 * Pi * radius;
	}
	double get_diameter() const {
		return radius * 2;
	}
	void draw()const override {
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);
		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		::Ellipse(hdc, start_x, start_y, start_x + get_diameter(), start_y + get_diameter());

		DeleteObject(hBrush);
		DeleteObject(hPen);

		ReleaseDC(hwnd, hdc);
	}

	void info() const {
		cout << typeid(*this).name() << endl;
		cout << "Периметр: " << get_perimeter() << endl;
		cout << "Радиус: " << get_radius() << endl;
		Shape::info();
	}
};

/// <summary>
/// //////////////////////
/// </summary>
class Triangle :public Shape {
public:
	virtual double get_height()const = 0;
	Triangle(SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS) {}
	~Triangle() {}
	void info()const {
		cout << "Высота треугольника: " << get_height() << endl;
		Shape::info();
	}
};
class EqualateralTrinagle :public Triangle {
	double side;
public:
	double get_side()const {
		return side;
	}
	void set_side(double side) {
		if (side < MIN_DIMENSION) side = MIN_DIMENSION;
		if (side < MAX_DIMENSION) side = MAX_DIMENSION;
		this->side = side;
	}

	double get_height()const override {
		return (side * sqrt(3)) / 2;
	}
	double get_area()const override {
		return pow(get_height(), 2) * sqrt(3);
	}
	double get_perimeter()const override {
		return 3 * side;
	}
	void draw()const override {
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);

		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		POINT vertex[] = {
			{start_x, start_y + side},
			{start_x + side, start_y + side},
			{start_x + side / 2, start_y + side - get_height()}
		};
		::Polygon(hdc, vertex, 3);

		DeleteObject(hPen);
		DeleteObject(hBrush);

		ReleaseDC(hwnd, hdc);
	}

	EqualateralTrinagle(double side, SHAPE_TAKE_PARAMETERS) :Triangle(SHAPE_GIVE_PARAMETERS) {
		set_side(side);
	}
	~EqualateralTrinagle() {}

	void info()const {
		cout << typeid(*this).name() << endl;
		cout << "Длина стороны: " << side << endl;
		Triangle::info();
	}
};

int main() {
	setlocale(LC_ALL, "");
	class Rectangle rect(200, 150, Color::blue, 50, 50, 5);
	rect.info();
	cout << delimiter << endl;
	Square square(200, Color::green, 50,50, 1);
	square.info();
	cout << delimiter << endl;
	Circle circle(200, Color::purple, 100,150, 1);
	circle.info();
	cout << delimiter << endl;
	EqualateralTrinagle triagle(50, Color::red, 150, 100, 1);
	triagle.info();
	return 0;
}