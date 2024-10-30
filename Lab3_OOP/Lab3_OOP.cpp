#include <iostream>
#include <iomanip>
#define M_PI 3.14159265358979323846

class Point {
public:
    double x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class Shape {
public:
    Point* arc = nullptr;
    char id = 0;
    virtual void info() = 0;
    virtual ~Shape() {};
    virtual double getArea() = 0;
    virtual Point centroid() = 0;
    virtual void rotate(Point, double) = 0;
    virtual void translate(Point) = 0;

    virtual void print() = 0;
    Point rotate_point(Point, double);
    static Shape* createShape(char);
};

Point Shape::rotate_point(Point p, double angle) {
    double rad = angle * (M_PI / 180.0); // Преобразуем угол в радианы
    Point result;

    result.x = p.x * cos(rad) - p.y * sin(rad);
    result.y = p.x * sin(rad) + p.y * cos(rad);

    return result;
}


class Triangle : public Shape {
public:
    Triangle() {
        id = 'T';
        arc = new Point[3];
        do {
            std::cout << "Вершина 1: ";
            std::cin >> arc[0].x >> arc[0].y;
            std::cout << "Вершина 2: ";
            std::cin >> arc[1].x >> arc[1].y;
            std::cout << "Вершина 3: ";
            std::cin >> arc[2].x >> arc[2].y;
            if ((arc[1].x - arc[0].x) * (arc[2].y - arc[0].y) - (arc[1].y - arc[0].y) * (arc[2].x - arc[0].x) == 0) {
                std::cout << "Фигура не является треугоьником\nВведите заково:\n";
            }
        } while ((arc[1].x - arc[0].x) * (arc[2].y - arc[0].y) - (arc[1].y - arc[0].y) * (arc[2].x - arc[0].x) == 0);
    }
    void info() { std::cout << "Треугольник\n"; }
    double getArea();
    ~Triangle() { delete[] arc; }
    Point centroid();
    void rotate(Point, double);
    void translate(Point);
    void print() {
        std::cout << std::fixed << std::setprecision(2) << "------------------\n" << arc[0].x << " " << arc[0].y << "\n" << arc[1].x << " " << arc[1].y << "\n" << arc[2].x << " " << arc[2].y << "\n" << "------------------\n";
    }
};

double Triangle::getArea() {
    return 0.5 * std::abs(arc[0].x * (arc[1].y - arc[2].y) + arc[1].x * (arc[2].y - arc[0].y) + arc[2].x * (arc[0].y - arc[1].y));
}

Point Triangle::centroid() {
    Point G;
    G.x = (arc[0].x + arc[1].x + arc[2].x) / 3;
    G.y = (arc[0].y + arc[1].y + arc[2].y) / 3;
    return G;
}

void Triangle::rotate(Point center, double angle) {
    for (int i = 0; i < 3; i++) {
        Point translated = { arc[i].x - center.x, arc[i].y - center.y };
        Point rotated = rotate_point(translated, angle);
        arc[i].x = rotated.x + center.x;
        arc[i].y = rotated.y + center.y;
    }
}

void Triangle::translate(Point p) {
    for (int i = 0; i < 3; i++) {
        arc[i].x += p.x;
        arc[i].y += p.y;
    }
}




class Paralelogram : public Shape {
private:
    bool is_parl(Point p[4]);
public:
    Paralelogram() {
        id = 'P';
        arc = new Point[4];
        do {
            std::cout << "Вершина 1: ";
            std::cin >> arc[0].x >> arc[0].y;
            std::cout << "Вершина 2: ";
            std::cin >> arc[1].x >> arc[1].y;
            std::cout << "Вершина 3: ";
            std::cin >> arc[2].x >> arc[2].y;
            std::cout << "Вершина 4: ";
            std::cin >> arc[3].x >> arc[3].y;
            if (!is_parl(arc)) {
                std::cout << "Фигура не является параллелограммом\nВведите заково:\n";
            }
        } while (!is_parl(arc)); 
    }
    void info() { std::cout << "Параллелограмм\n"; }
    double getArea();
    ~Paralelogram() { delete[] arc; }
    Point centroid();
    void rotate(Point, double);
    void translate(Point);
    void print() {
        std::cout << std::fixed << std::setprecision(2) << "------------------\n" << arc[0].x << " " << arc[0].y << "\n" << arc[1].x << " " << arc[1].y << "\n" << arc[2].x << " " << arc[2].y << "\n" << arc[3].x << " " << arc[3].y << "\n" << "------------------\n";
    }
};

double Paralelogram::getArea() {
    double ABx = arc[1].x - arc[0].x;
    double ABy = arc[1].y - arc[0].y;
    double ADx = arc[3].x - arc[0].x;
    double ADy = arc[3].y - arc[0].y;
    return std::abs(ABx * ADy - ABy * ADx);
}

bool Paralelogram::is_parl(Point points[4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            for (int k = j + 1; k < 4; k++) {
                int l = 6 - i - j - k;
                if ((((points[j].x - points[i].x) * (points[k].y - points[i].y) - (points[j].y - points[i].y) * (points[k].x - points[i].x)) == 0) ||
                (((points[l].x - points[i].x) * (points[k].y - points[i].y) - (points[l].y - points[i].y) * (points[k].x - points[i].x)) == 0)){
                    return false;
                }
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) { continue; }
            for (int k = 0; k < 4; k++) {
                if (k == j || k == i) { continue; }
                int l = 6 - i - j - k;
                if ((points[i].x - points[j].x) == (points[l].x - points[k].x) && (points[i].y - points[j].y) == (points[l].y - points[k].y)) {
                    Point a = points[i], b = points[j], c = points[k], d = points[l];
                    points[0] = a;
                    points[1] = b;
                    points[2] = c;
                    points[3] = d;
                    return true;
                }
            }
        }
    }
    return false;
}

Point Paralelogram::centroid() {
    Point G;
    G.x = (arc[0].x + arc[1].x + arc[2].x + arc[3].x) / 4;
    G.y = (arc[0].y + arc[1].y + arc[2].y + arc[3].y) / 4;
    return G;
}

void Paralelogram::rotate(Point center, double angle) {
    for (int i = 0; i < 4; i++) {
        Point translated = { arc[i].x - center.x, arc[i].y - center.y };
        Point rotated = rotate_point(translated, angle);
        arc[i].x = rotated.x + center.x;
        arc[i].y = rotated.y + center.y;
    }
}

void Paralelogram::translate(Point p) {
    for (int i = 0; i < 4; i++) {
        arc[i].x += p.x;
        arc[i].y += p.y;

    }
}


class Square : public Paralelogram {
private:
    bool is_square(Point p[4]);
    double leng(Point a, Point b) {
        double c = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
public:
    Square() {
        id = 'S';
        while (!is_square(arc)) {
            std::cout << "Фигура не является квадратом\nВведите заково:\n";
            std::cout << "Вершина 1: ";
            std::cin >> arc[0].x >> arc[0].y;
            std::cout << "Вершина 2: ";
            std::cin >> arc[1].x >> arc[1].y;
            std::cout << "Вершина 3: ";
            std::cin >> arc[2].x >> arc[2].y;
            std::cout << "Вершина 4: ";
            std::cin >> arc[3].x >> arc[3].y;
            
        } ;
    }
    void info() { std::cout << "Квадрат\n"; }
    ~Square() { }
    void print() {
        std::cout << std::fixed << std::setprecision(2) << "------------------\n" << arc[0].x << " " << arc[0].y << "\n" << arc[1].x << " " << arc[1].y << "\n" << arc[2].x << " " << arc[2].y << "\n" << arc[3].x << " " << arc[3].y << "\n" << "------------------\n";
    }
};

bool Square::is_square(Point points[4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            for (int k = j + 1; k < 4; k++) {
                int l = 6 - i - j - k;
                if ((((points[j].x - points[i].x) * (points[k].y - points[i].y) - (points[j].y - points[i].y) * (points[k].x - points[i].x)) == 0) ||
                    (((points[l].x - points[i].x) * (points[k].y - points[i].y) - (points[l].y - points[i].y) * (points[k].x - points[i].x)) == 0)) {
                    return false;
                }
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) { continue; }
            for (int k = 0; k < 4; k++) {
                if (k == j || k == i) { continue; }
                int l = 6 - i - j - k;
                if ((std::fabs(leng(points[i], points[j]) - leng(points[j], points[k])) < 0.0001)
                    && (std::fabs(leng(points[i], points[k]) - leng(points[j], points[l]) < 0.0001))){
                    return true;
                }
            }
        }
    }
    return false;
}


class Operations {
public:
    static int Compare(Shape* a, Shape* b) {
        return a->getArea() > b->getArea() ? 1 : (a->getArea() < b->getArea() ? -1 : 0);
    }

    static bool IsIntersect(Shape* a, Shape* b) {
        for (int i = 0; i < 3; ++i) { 
            for (int j = 0; j < 4; ++j) {
                if (lineIntersection(a->arc[i], a->arc[(i + 1) % 3],
                    b->arc[j], b->arc[(j + 1) % 4])) {
                    return true;
                }
            }
        }
        return false;
    }

    static bool IsInclude(Shape* outer, Shape* inner) {
        for (int i = 0; i < (inner->id == 'T' ? 3 : 4); ++i) {
            if (!pointInShape(inner->arc[i], outer)) {
                return false;
            }
        }
        return true;
    }

private:
    static bool lineIntersection(Point A1, Point A2, Point B1, Point B2) {
        double d1, d2;
        d1 = (A2.x - A1.x) * (B1.y - A1.y) - (A2.y - A1.y) * (B1.x - A1.x);
        d2 = (A2.x - A1.x) * (B2.y - A1.y) - (A2.y - A1.y) * (B2.x - A1.x);
        double r1, r2;
        r1 = (B2.x - B1.x) * (A1.y - B1.y) - (B2.y - B1.y) * (A1.x - B1.x);
        r2 = (B2.x - B1.x) * (A2.y - B1.y) - (B2.y - B1.y) * (A2.x - B1.x);
        return (d1 * d2 < 0) && (r1 * r2 < 0);
    }

    static bool pointInShape(Point p, Shape* s) {
        int n = (s->id == 'T') ? 3 : 4;
        int count = 0;
        for (int i = 0; i < n; ++i) {
            Point A1 = s->arc[i];
            Point A2 = s->arc[(i + 1) % n];
            if (A1.y > A2.y) std::swap(A1, A2);
            if (p.y == A1.y || p.y == A2.y) p.y += 0.0001;
            if (p.y > A1.y && p.y <= A2.y &&
                (p.x <= (A2.x - A1.x) * (p.y - A1.y) / (A2.y - A1.y) + A1.x)) {
                count++;
            }
        }
        return (count % 2 == 1);
    }
};


Shape* Shape::createShape(char cd)
{
    Shape* sh = 0;
    switch (cd) {
    case 'T':
        sh = new Triangle(); break;
    case 'P':
        sh = new Paralelogram(); break;
    case 'S':
        sh = new Square(); break;
    default:
        char h;
        std::cout << "Ошибка ввода\nВведите заново: ";
        std::cin >> h;
        sh = createShape(h);
        break;
    }
    return sh;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Shape* p1 = 0;
    char T;
    std::cout << "Тип? ";
    std::cin >> T;
    p1 = Shape::createShape(T);
    if (!p1) return 0;

    p1->info();
    std::cout << p1->getArea() << "\n";
    std::cout << p1->centroid().x << " " << p1->centroid().y << "\n";

    std::cout << "Введите центр поворота: ";
    double _x, _y;
    std::cin >> _x >> _y;
    std::cout << "Введите угол поворота: ";
    double _f;
    std::cin >> _f;
    p1->rotate({ _x, _y }, _f);
    p1->print();

    std::cout << "Введите вектор сдвига: ";
    double x_, y_;
    std::cin >> x_ >> y_;
    p1->translate({ x_, y_ });
    p1->print();

    std::cout << "\n||     Вторая фигура     ||\n";

    Shape* p2 = 0;
    char T2;
    std::cout << "Тип второй фигуры? ";
    std::cin >> T2;
    p2 = Shape::createShape(T2);
    if (!p2) return 0;

    p2->info();
    std::cout << p2->getArea() << "\n";
    std::cout << p2->centroid().x << " " << p2->centroid().y << "\n";

    int cmpResult = Operations::Compare(p1, p2);
    if (cmpResult == 1) {
        std::cout << "Первая фигура больше второй.\n";
    }
    else if (cmpResult == -1) {
        std::cout << "Вторая фигура больше первой.\n";
    }
    else {
        std::cout << "Фигуры равны по площади.\n";
    }

    if (Operations::IsIntersect(p1, p2)) {
        std::cout << "Фигуры пересекаются.\n";
    }
    else {
        std::cout << "Фигуры не пересекаются.\n";
    }

    if (Operations::IsInclude(p1, p2)) {
        std::cout << "Первая фигура включает вторую.\n";
    }
    else {
        std::cout << "Первая фигура не включает вторую.\n";
    }

    delete p1;
    return 0;
}


