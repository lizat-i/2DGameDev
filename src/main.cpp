#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <fstream>
#include <sstream>
#include <string>

const int wHeight = 240;
const int wWidth = 320;
typedef std::vector<std::shared_ptr<sf::Shape>> shapeVector;
const sf::FloatRect wRectangle(0.0, 0.0, wHeight, wWidth);

// Assume RGB is a simple struct for colors
struct RGB
{
    int r, g, b;
};

struct inputShape
{
    std::string type;
    std::string name;
    RGB rgb;
};

struct windowData
{
    int width, height;
};

struct rectangleShape
{
    inputShape base;
    int x, y;
    float vx, vy;
    int width, height;
};

struct circleShape
{
    inputShape base;
    int x, y;
    float vx, vy;
    int radius;
};

struct inputContainer
{
    windowData window;
    std::vector<rectangleShape> rectangles;
    std::vector<circleShape> circles;
};

bool isCollisionHappening(const std::shared_ptr<sf::Shape> &input_Shape)
{
    return ~(input_Shape->getGlobalBounds().intersects(wRectangle));
};

void handleCollision(std::shared_ptr<sf::Shape> &input_Shape)
{
    if (
        (input_Shape->getGlobalBounds().top < wRectangle.top) ||
        ((input_Shape->getGlobalBounds().top + input_Shape->getGlobalBounds().height) > (wRectangle.top + wRectangle.height)))
    {
        (*input_Shape).m_velocityPreFactor_y *= -1;
    };
    if (
        (input_Shape->getGlobalBounds().left < wRectangle.left) ||
        ((input_Shape->getGlobalBounds().left + input_Shape->getGlobalBounds().width) > (wRectangle.left + wRectangle.width)))
    {
        (*input_Shape).m_velocityPreFactor_x *= -1;
    };
};

void move_shapes(shapeVector &input_Vector_of_Shapes)
{
    for (auto &shape_iter : input_Vector_of_Shapes)
    {
        const float x_vel = 0.5f;
        const float y_vel = 0.5f;

        if (isCollisionHappening(shape_iter))
        {
            handleCollision(shape_iter);
        };
        shape_iter->setPosition(
            shape_iter->getPosition().x + x_vel * (*shape_iter).m_velocityPreFactor_x,
            shape_iter->getPosition().y + y_vel * (*shape_iter).m_velocityPreFactor_y);
    }
}

void createVectorOfShapesFromContainer(shapeVector &input_Vec, const inputContainer &container)
{
    // Process rectangles from the container
    for (const auto &rect : container.rectangles)
    {
        auto rectShape = std::make_shared<sf::RectangleShape>(sf::Vector2f(rect.width, rect.height));
        rectShape->setPosition(rect.x, rect.y);
        rectShape->setFillColor(sf::Color(rect.base.rgb.r, rect.base.rgb.g, rect.base.rgb.b));
        input_Vec.push_back(rectShape);
    }

    // Process circles from the container
    for (const auto &circle : container.circles)
    {
        auto circleShape = std::make_shared<sf::CircleShape>(circle.radius);
        circleShape->setPosition(circle.x, circle.y);
        circleShape->setFillColor(sf::Color(circle.base.rgb.r, circle.base.rgb.g, circle.base.rgb.b));
        input_Vec.push_back(circleShape);
    }
}
void readInputFile(const std::string &filename, inputContainer &container)
{
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string objectType;
        iss >> objectType;

        if (objectType == "Window")
        {
            iss >> container.window.width >> container.window.height;
        }
        else if (objectType == "Circle")
        {
            circleShape circle;
            iss >> circle.base.name >> circle.x >> circle.y >> circle.vx >> circle.vy >> circle.base.rgb.r >> circle.base.rgb.g >> circle.base.rgb.b >> circle.radius;
            container.circles.push_back(circle);
        }
        else if (objectType == "Rectangle")
        {
            rectangleShape rect;
            iss >> rect.base.name >> rect.x >> rect.y >> rect.vx >> rect.vy >> rect.base.rgb.r >> rect.base.rgb.g >> rect.base.rgb.b >> rect.width >> rect.height;
            container.rectangles.push_back(rect);
        }
        else
        {
            std::cerr << "Unknown object type: " << objectType << std::endl;
        }
    }
    for(auto &circle : container.circles){
        std::cout<<circle.base.name<< " "<<circle.base.type<<" "<<std::endl; 
    }
        for(auto &rect : container.rectangles){
        std::cout<<rect.base.name<< " "<<rect.base.type<<" "<<std::endl; 
    }
    
}

int main()
{

    const int FPS_Limit = 60;
    inputContainer container;
    std::string input_name("input/config.txt"); 
    readInputFile(input_name, container);

    auto window = sf::RenderWindow{sf::VideoMode(container.window.height, container.window.width), " SFML Video"};
    window.setFramerateLimit(FPS_Limit);

    shapeVector my_Shapes;
    createVectorOfShapesFromContainer(my_Shapes, container);

    // for (auto &shape_iter : my_Shapes)
    // {

    //     (*shape_iter).
    // }

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        move_shapes(my_Shapes);

        window.clear();

        for (auto &shape_iter : my_Shapes)
        {

            window.draw(*shape_iter);
        }

        window.display();
    }
}