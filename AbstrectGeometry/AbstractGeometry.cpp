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

#define Pi 3,14
#define SHAPE_TAKE_PARAMETERS Color color, int start_x, int start_y, int line_width
#define SHAPE_GIVE_PARAMETERS color, start_x, start_y, line_width

class Shape {
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
		virtual void draw()const = 0;
		Shape(SHAPE_TAKE_PARAMETERS) {
			set_color(color);
			set_start_x(start_x);
			set_start_y(start_y);
			set_line_width(line_width);
		}
		~Shape() {}
};

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

	Square(double side, SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS) {
		set_side(side);
	}
	~Square() {}

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

	void info() {
		cout << "Длина стороны: " << get_side() << endl;
		cout << "Периметр:\t" << get_perimeter() << endl;
		cout << "Площадь квадрата:" << get_area() << endl;
	}
};
class Rectangle :public Shape{
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
		if (side_a < 1) side_a = 1;
		this->side_a = side_a;
	}
	void set_side_b(double side_b) {
		if (side_b < 1) side_b = 1;
		this->side_b = side_b;
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
		HBRUSH hBrush= CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		::Rectangle(hdc, start_x, start_y, start_x + side_a, start_y + side_b);

		DeleteObject(hPen);
		DeleteObject(hBrush);

		ReleaseDC(hwnd, hdc);
	}

	Rectangle(double side_a, double side_b, SHAPE_TAKE_PARAMETERS):Shape(SHAPE_GIVE_PARAMETERS) {
		set_color(color);
		set_start_x(start_x);
		set_start_y(start_y);
		set_side_a(side_a);
		set_side_b(side_b);
	}
	~Rectangle(){}

	void info() {
		cout << "Сторона A:\t" << get_side_a() << endl;
		cout << "Сторона B:\t" << get_side_b() << endl;
		cout << "Площадь:\t" << get_area() << endl;
		cout << "Периметр:\t" << get_perimeter() << endl;
	}
};
class Circle:public Shape {
protected:
	double radius;
public:
	double const get_radius() const {
		return radius;
	}
	void set_radius(double radius) {
		if (radius < 1) radius = 1;
		this->radius = radius;
	}
	Circle(double radius, SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS) {
		set_radius(radius);
	}
	~Circle() {}

	double get_area() {
		return 2 * Pi * pow(radius, 2);
	}
	double get_perimeter() {
		return 2 * Pi * radius;
	}

	void draw()const override {
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);
		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		//Rectangle(hdc, start_x, start_y, start_x + side, start_y + side);

		DeleteObject(hBrush);
		DeleteObject(hPen);

		ReleaseDC(hwnd, hdc);
	}

	void info() {
		cout << "Площадь фигуры: " << get_area() << endl;
		cout << "Периметр: " << get_perimeter() << endl;
		cout << "Радиус: " << get_radius() << endl;
	}
};
class Trinagle :public Shape {
protected:
	double side_a;
	double side_b;
	double side_c;
	double semiperimeter;
public:
	double get_side_a()const {
		return side_a;
	}
	double get_side_b()const {
		return side_b;
	}
	double get_side_c()const {
		return side_c;
	}
	
	void set_side_a(double side_a) {
		this->side_a = side_a;
	}
	void set_side_b(double side_b) {
		this->side_b = side_b;
	}
	void set_side_c(double side_c) {
		this->side_c = side_c;
	}

	Trinagle(double side_a, double side_b, double side_c, SHAPE_TAKE_PARAMETERS) :Shape(SHAPE_GIVE_PARAMETERS) {
		set_side_a(side_a);
		set_side_b(side_b);
		set_side_c(side_c);
	}
	~Trinagle() {}

	double get_area() {
		semiperimeter = (get_side_a() + get_side_b() + get_side_c()) / 2;
		return sqrt(semiperimeter * ((semiperimeter * get_side_a()) * (semiperimeter * get_side_b()) * (semiperimeter * get_side_c())));
	}
	double get_perimeter() {
		return get_side_a() + get_side_b() + get_side_b();
	}

	void draw()const override {
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);
		HPEN hPen = CreatePen(PS_SOLID, line_width, color);
		HBRUSH hBrush = CreateSolidBrush(color);

		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		//Rectangle(hdc, start_x, start_y, start_x + side, start_y + side);

		DeleteObject(hBrush);
		DeleteObject(hPen);

		ReleaseDC(hwnd, hdc);
	}

	void info() {
		cout << "Сторона A:" << get_side_a() << endl;
		cout << "Сторона B:" << get_side_b() << endl;
		cout << "Сторона C:" << get_side_c() << endl;
		cout << "Площадь фигуры: " << get_area() << endl;
		cout << "Периметр: " << get_perimeter() << endl;
	}
};

int main() {
	setlocale(LC_ALL, "");
	Square square(150, Color::red,200,200,5);
	square.draw();

	class Rectangle rect(200, 150, Color::blue, 200, 450, 5);
	rect.draw();



	return 0;
}