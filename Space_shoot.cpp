#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>
#include <windows.h>

using namespace std;

/*pour executer
g++ Space_shoot.cpp -o Space_shoot.exe -I /c/SFML/include -L /c/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
./Space_shoot.exe
*/
bool areOverlapping(const sf::Sprite& sprite, const sf::Vector2f& circleCenter, float circleRadius) {
    sf::FloatRect rect = sprite.getGlobalBounds();
    float closestX = std::max(rect.left, std::min(circleCenter.x, rect.left + rect.width));
    float closestY = std::max(rect.top, std::min(circleCenter.y, rect.top + rect.height));
    float distanceX = circleCenter.x - closestX;
    float distanceY = circleCenter.y - closestY;
    return (distanceX * distanceX + distanceY * distanceY) <= (circleRadius * circleRadius);
    
}

bool PerduOuPas(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
}


int main() {
    sf::RenderWindow screen(sf::VideoMode(600, 800), "Test n2");
    sf::Clock clock;

    sf::Texture texture;
    texture.loadFromFile("C:/users/Anis Djerrab/Downloads/vaisseau.png");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(300.f, 700.f);
    sprite.setScale(0.5f, 0.5f);



    int score;
    int difficulty(0);
    int nombre(0);
    bool confirmation;
    bool GAMEOVER(false);
    int attente(0);

    sf::Font fond;
    fond.loadFromFile("Arial.ttf");

    sf::Text text1;
    text1.setFont(fond);
    text1.setString("Score ");
    text1.setCharacterSize(30); 
    text1.setFillColor(sf::Color::Blue); 
    text1.setPosition(25, 25);
    sf::Text text3;
    text3.setFont(fond);
    text3.setString("GAME\nOVER");
    text3.setCharacterSize(150); 
    text3.setFillColor(sf::Color::Red); 
    text3.setPosition(90, 250);


    sf::Texture rocksTexture;
    rocksTexture.loadFromFile("C:/users/Anis Djerrab/Downloads/Piskel2.png");
    rocksTexture.setSmooth(false);

    sf::Vector2f velocity(0.f, 0.f);

    vector<sf::CircleShape> CalibresAttaque;
    vector<sf::Sprite> cailloux;
    vector<int> viesCailloux;

    float compteurTemps(0);
    int detecterCompteurTemps(-1);


    while (screen.isOpen())
    {
        sf::Event event;
        sf::Time deltaTime = clock.restart();
        while (screen.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                screen.close();
        
            velocity.x = 0;
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)
                {
                    velocity.x = -3.f;
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    velocity.x = 3.f;
                }
                else if (event.key.code == sf::Keyboard::Space)
                {
                    if (attente > 20)
                    {
                        sf::CircleShape cercle(5.f);
                        cercle.setFillColor(sf::Color::Red);
                        cercle.setPosition(sprite.getPosition().x + 30, sprite.getPosition().y);
                        CalibresAttaque.push_back(cercle);
                        attente = 0;
                    }    
                    
                }
            }
        }
            attente+=1;
            compteurTemps += 0.01;
            nombre=0;
            sprite.move(velocity);
            if (sprite.getPosition().x > 530)
            {
                sprite.setPosition(530, sprite.getPosition().y);
            }
            else if (sprite.getPosition().x < 0)
            {
                sprite.setPosition(0, sprite.getPosition().y);
            }
            sf::Vector2f velocity(0.f, 0.f);
            screen.clear();
            
            if (deltaTime.asSeconds() < 1.f / 60.f)
                    {
                        sf::sleep(sf::seconds(1.f / 60.f - deltaTime.asSeconds()));  
                    }
            screen.draw(sprite);
            for (auto& rond : CalibresAttaque)
            {
                rond.setPosition(rond.getPosition().x, rond.getPosition().y - 5);
                screen.draw(rond);
            }
            for (auto& rond  : CalibresAttaque )
            {
                nombre = 0;
                auto viesIt = viesCailloux.begin();
                for (auto& caillou  : cailloux) 
                {
                    confirmation = areOverlapping(caillou, rond.getPosition(), rond.getRadius());
                    nombre += 1;
                    if (confirmation)
                    {
                        score += 1;
                        cout << score << endl;
                        rond.setPosition(5000.f, rond.getPosition().x);
                        (*viesIt) -= 1;
                        if (*viesIt < 0)
                        {
                            caillou.setPosition(5000.f, 5000.f);
                            if (nombre < viesCailloux.size()) {
                                viesCailloux.erase(viesCailloux.begin() + nombre);
                            }
                        }
                        confirmation = false;
                        break;
                    }
                    else
                    {
                        ++nombre;
                        ++viesIt;
                    }
                }
            }
            if (compteurTemps >= 10 && compteurTemps < 20)
            {
                difficulty = 1;
            }
            if (compteurTemps >= 20 && compteurTemps < 30)
            {
                difficulty = 2;
            }
            if (compteurTemps >= 30 && compteurTemps < 50)
            {
                difficulty = 3;
            }
            if (compteurTemps >= 50 && compteurTemps < 80)
            {
                difficulty = 4;
            }
            if (compteurTemps > 80)
            {
                difficulty = 5;
            }
            if (compteurTemps-detecterCompteurTemps > 3)
            {
                if (difficulty == 0)
                {
                    detecterCompteurTemps = compteurTemps;
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_int_distribution<> dis(0, 400);
                    int random_index = dis(gen);
                    sf::Sprite objet;
                    objet.setTexture(rocksTexture);
                    objet.setPosition(static_cast<float>(random_index), 0.f);
                    objet.setScale(0.02f, 0.02f);

                    cailloux.push_back(objet);
                    viesCailloux.push_back(1);
                }
                else if (difficulty == 2)
                {
                    for (int i(0); i < 2; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -80+i*40.f);
                        objet.setScale(0.02f, 0.02f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(2);
                    }
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_int_distribution<> dis(0, 400);
                    int random_index = dis(gen);
                    sf::Sprite objet;
                    objet.setTexture(rocksTexture);
                    objet.setPosition(static_cast<float>(random_index), 0.f);
                    objet.setScale(0.035f, 0.035f);

                    cailloux.push_back(objet);
                    viesCailloux.push_back(5);
                }
                else if (difficulty == 2)
                {
                    for (int i(0); i < 3; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -200+i*60.f);
                        objet.setScale(0.02f, 0.02f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(2);
                    }
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_int_distribution<> dis(0, 400);
                    int random_index = dis(gen);
                    sf::Sprite objet;
                    objet.setTexture(rocksTexture);
                    objet.setPosition(static_cast<float>(random_index), 0.f);
                    objet.setScale(0.035f, 0.035f);

                    cailloux.push_back(objet);
                    viesCailloux.push_back(5);

                    random_index = dis(gen);
                    objet.setTexture(rocksTexture);
                    objet.setPosition(static_cast<float>(random_index), -400.f);
                    objet.setScale(0.055f, 0.055f);

                    cailloux.push_back(objet);
                    viesCailloux.push_back(8);
                }
                else if (difficulty == 3)
                {
                    for (int i(0); i < 4; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -200+i*40.f);
                        objet.setScale(0.02f, 0.02f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(2);
                    }
                    for (int i(0); i < 2; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -400+i*100.f);
                        objet.setScale(0.035f, 0.035f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(5);
                    }
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_int_distribution<> dis(0, 400);
                    int random_index = dis(gen);
                    sf::Sprite objet;
                    objet.setTexture(rocksTexture);
                    objet.setPosition(static_cast<float>(random_index), -800.f);
                    objet.setScale(0.055f, 0.055f);

                    cailloux.push_back(objet);
                    viesCailloux.push_back(8);   

                }
                else if (difficulty == 4)
                {
                    for (int i(0); i < 6; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -500+i*40.f);
                        objet.setScale(0.02f, 0.02f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(3);
                    }
                    for (int i(0); i < 3; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -600+i*100.f);
                        objet.setScale(0.035f, 0.035f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(6);
                    }
                    for (int i(0); i < 2; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -1000+i*200.f);
                        objet.setScale(0.055f, 0.055f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(9);
                    }
                      

                }
                else if (difficulty == 5)
                {
                    for (int i(0); i < 18; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -500+i*20.f);
                        objet.setScale(0.02f, 0.02f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(3);
                    }
                    for (int i(0); i < 9; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -600+i*40.f);
                        objet.setScale(0.035f, 0.035f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(6);
                    }
                    for (int i(0); i < 6; i++)
                    {
                        detecterCompteurTemps = compteurTemps;
                        random_device rd;
                        mt19937 gen(rd());
                        uniform_int_distribution<> dis(0, 400);
                        int random_index = dis(gen);
                        sf::Sprite objet;
                        objet.setTexture(rocksTexture);
                        objet.setPosition(static_cast<float>(random_index), -1000+i*60.f);
                        objet.setScale(0.055f, 0.055f);

                        cailloux.push_back(objet);
                        viesCailloux.push_back(9);
                    }
                      

                }



            }


            for (auto& caillou : cailloux)
            {
                caillou.setPosition(caillou.getPosition().x, caillou.getPosition().y + 1);
                screen.draw(caillou);
            }
            cailloux.erase(
                remove_if(
                cailloux.begin(), 
                cailloux.end(),
                [](const sf::Sprite& caillou) {
                return caillou.getPosition().y > 800; 
                }),
            cailloux.end());
            
            viesCailloux.erase(
                remove_if(
                viesCailloux.begin(), 
                viesCailloux.end(),
                [](int vie) {
                return vie < 0; 
                }),
            viesCailloux.end());

            CalibresAttaque.erase(
                remove_if(
                CalibresAttaque.begin(), 
                CalibresAttaque.end(),
                [](const sf::CircleShape& rond) {
                return rond.getPosition().y < 0; 
                }),
            CalibresAttaque.end());

            for (auto& caillou : cailloux)
            {
                GAMEOVER = PerduOuPas(caillou, sprite);
                if (GAMEOVER)
                {
                    screen.draw(text3);
                    screen.display();
                    Sleep(3000);
                    score = 0;
                    difficulty = 0;
                    compteurTemps = 0;
                    detecterCompteurTemps = 0;
                    GAMEOVER = false;
                    CalibresAttaque.clear();
                    viesCailloux.clear();
                    cailloux.clear();
                }
            }
            screen.draw(text1);
            sf::Text text2;
            text2.setFont(fond);
            text2.setString(to_string(score));
            text2.setCharacterSize(30); 
            text2.setFillColor(sf::Color::Green); 
            text2.setPosition(140, 25);
            screen.draw(text2);
            screen.display();
    }    
    

    return 0;
}