#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>

const int WIDTH = 1200, HEIGHT = 800;
const int CELL_SIZE = 8;
const int COLS = WIDTH / CELL_SIZE;
const int ROWS = HEIGHT / CELL_SIZE;

struct Grid {
    std::vector<uint8_t> cells;
    std::vector<uint8_t> age; // how many gens a cell has been alive, for color fade

    Grid() : cells(COLS*ROWS, 0), age(COLS*ROWS, 0) {}

    int idx(int x, int y) const { return y*COLS + x; }

    bool get(int x, int y) const {
        if(x < 0 || x >= COLS || y < 0 || y >= ROWS) return false;
        return cells[idx(x,y)] != 0;
    }

    void set(int x, int y, bool alive) {
        if(x < 0 || x >= COLS || y < 0 || y >= ROWS) return;
        cells[idx(x,y)] = alive ? 1 : 0;
        if(!alive) age[idx(x,y)] = 0;
    }

    void clear() {
        std::fill(cells.begin(), cells.end(), 0);
        std::fill(age.begin(), age.end(), 0);
    }

    void randomize(float density) {
        clear();
        for(int i=0;i<COLS*ROWS;i++)
            cells[i] = (rand()/(float)RAND_MAX < density) ? 1 : 0;
    }

    int countNeighbors(int x, int y) const {
        int n = 0;
        for(int dy=-1; dy<=1; dy++)
            for(int dx=-1; dx<=1; dx++) {
                if(dx==0 && dy==0) continue;
                if(get(x+dx, y+dy)) n++;
            }
        return n;
    }
};

Grid grid;
Grid nextGrid;
long long generation = 0;

void step() {
    nextGrid.cells = grid.cells;
    nextGrid.age = grid.age;

    for(int y=0; y<ROWS; y++) {
        for(int x=0; x<COLS; x++) {
            int n = grid.countNeighbors(x, y);
            bool alive = grid.get(x, y);
            bool willLive = alive ? (n==2 || n==3) : (n==3);

            nextGrid.set(x, y, willLive);
            if(willLive) {
                int a = grid.age[grid.idx(x,y)];
                nextGrid.age[nextGrid.idx(x,y)] = (uint8_t)std::min(255, a+4);
            }
        }
    }

    std::swap(grid.cells, nextGrid.cells);
    std::swap(grid.age, nextGrid.age);
    generation++;
}

void loadGlider(int ox, int oy) {
    int pts[][2] = {{1,0},{2,1},{0,2},{1,2},{2,2}};
    for(auto& p : pts) grid.set(ox+p[0], oy+p[1], true);
}

void loadPulsar(int ox, int oy) {
    // 13x13 pulsar pattern (classic period-3 oscillator)
    int pts[][2] = {
        {2,0},{3,0},{4,0},{8,0},{9,0},{10,0},
        {0,2},{5,2},{7,2},{12,2},
        {0,3},{5,3},{7,3},{12,3},
        {0,4},{5,4},{7,4},{12,4},
        {2,5},{3,5},{4,5},{8,5},{9,5},{10,5},
        {2,7},{3,7},{4,7},{8,7},{9,7},{10,7},
        {0,8},{5,8},{7,8},{12,8},
        {0,9},{5,9},{7,9},{12,9},
        {0,10},{5,10},{7,10},{12,10},
        {2,12},{3,12},{4,12},{8,12},{9,12},{10,12}
    };
    for(auto& p : pts) grid.set(ox+p[0], oy+p[1], true);
}

void loadGliderGun(int ox, int oy) {
    // Gosper Glider Gun
    int pts[][2] = {
        {24,0},
        {22,1},{24,1},
        {12,2},{13,2},{20,2},{21,2},{34,2},{35,2},
        {11,3},{15,3},{20,3},{21,3},{34,3},{35,3},
        {0,4},{1,4},{10,4},{16,4},{20,4},{21,4},
        {0,5},{1,5},{10,5},{14,5},{16,5},{17,5},{22,5},{24,5},
        {10,6},{16,6},{24,6},
        {11,7},{15,7},
        {12,8},{13,8}
    };
    for(auto& p : pts) grid.set(ox+p[0], oy+p[1], true);
}

sf::Color cellColor(int age) {
    // Newly born = bright cyan/green, aging = shifts to deep blue/purple
    float t = std::min(1.f, age / 60.f);
    uint8_t r = (uint8_t)(40 + t*120);
    uint8_t g = (uint8_t)(220 - t*100);
    uint8_t b = (uint8_t)(180 + t*60);
    return sf::Color(r, g, b);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({(unsigned)WIDTH,(unsigned)HEIGHT}),
        "Game of Life | LMB: Draw | RMB: Erase | Space: Play/Pause | N: Step | C: Clear | R: Random | G/P/U: Patterns");
    window.setFramerateLimit(60);

    srand(42);
    grid.randomize(0.18f);

    bool playing = false;
    bool showGrid = true;
    float stepInterval = 0.08f; // seconds per generation
    float stepTimer = 0.f;

    sf::Font font;
    bool hasFont = font.openFromFile("C:/Windows/Fonts/arial.ttf");
    sf::Text hud(font);
    hud.setCharacterSize(15);
    hud.setFillColor(sf::Color::White);
    hud.setOutlineColor(sf::Color::Black);
    hud.setOutlineThickness(1.5f);
    hud.setPosition({10.f, 10.f});

    sf::Clock clock;
    bool lmbHeld = false, rmbHeld = false;

    while(window.isOpen()) {
        float dt = std::min(clock.restart().asSeconds(), 0.05f);
        auto mpos = sf::Mouse::getPosition(window);
        int gx = mpos.x / CELL_SIZE;
        int gy = mpos.y / CELL_SIZE;

        while(auto ev = window.pollEvent()) {
            if(ev->is<sf::Event::Closed>()) window.close();
            if(auto* k = ev->getIf<sf::Event::KeyPressed>()) {
                if(k->code == sf::Keyboard::Key::Escape) window.close();
                if(k->code == sf::Keyboard::Key::Space) playing = !playing;
                if(k->code == sf::Keyboard::Key::N) step();
                if(k->code == sf::Keyboard::Key::C) { grid.clear(); generation = 0; }
                if(k->code == sf::Keyboard::Key::R) { grid.randomize(0.18f); generation = 0; }
                if(k->code == sf::Keyboard::Key::Grave) showGrid = !showGrid;
                if(k->code == sf::Keyboard::Key::G) { grid.clear(); generation=0; loadGlider(COLS/2-1, ROWS/2-1); }
                if(k->code == sf::Keyboard::Key::P) { grid.clear(); generation=0; loadPulsar(COLS/2-6, ROWS/2-6); }
                if(k->code == sf::Keyboard::Key::U) { grid.clear(); generation=0; loadGliderGun(10, 10); }
                if(k->code == sf::Keyboard::Key::Up) stepInterval = std::max(0.01f, stepInterval - 0.02f);
                if(k->code == sf::Keyboard::Key::Down) stepInterval = std::min(0.5f, stepInterval + 0.02f);
            }
            if(auto* mb = ev->getIf<sf::Event::MouseButtonPressed>()) {
                if(mb->button == sf::Mouse::Button::Left) lmbHeld = true;
                if(mb->button == sf::Mouse::Button::Right) rmbHeld = true;
            }
            if(auto* mb = ev->getIf<sf::Event::MouseButtonReleased>()) {
                if(mb->button == sf::Mouse::Button::Left) lmbHeld = false;
                if(mb->button == sf::Mouse::Button::Right) rmbHeld = false;
            }
        }

        if(lmbHeld) grid.set(gx, gy, true);
        if(rmbHeld) grid.set(gx, gy, false);

        if(playing) {
            stepTimer += dt;
            if(stepTimer >= stepInterval) {
                stepTimer = 0.f;
                step();
            }
        }

        window.clear(sf::Color(8, 10, 16));

        // Draw cells
        sf::RectangleShape cellShape({(float)CELL_SIZE-1.f, (float)CELL_SIZE-1.f});
        long long liveCount = 0;
        for(int y=0; y<ROWS; y++) {
            for(int x=0; x<COLS; x++) {
                if(grid.get(x,y)) {
                    liveCount++;
                    cellShape.setPosition({(float)(x*CELL_SIZE), (float)(y*CELL_SIZE)});
                    cellShape.setFillColor(cellColor(grid.age[grid.idx(x,y)]));
                    window.draw(cellShape);
                }
            }
        }

        // Grid lines
        if(showGrid) {
            sf::VertexArray lines(sf::PrimitiveType::Lines);
            sf::Color gridCol(30,34,42);
            for(int x=0; x<=COLS; x++) {
                lines.append(sf::Vertex{sf::Vector2f((float)(x*CELL_SIZE), 0.f), gridCol});
                lines.append(sf::Vertex{sf::Vector2f((float)(x*CELL_SIZE), (float)HEIGHT), gridCol});
            }
            for(int y=0; y<=ROWS; y++) {
                lines.append(sf::Vertex{sf::Vector2f(0.f, (float)(y*CELL_SIZE)), gridCol});
                lines.append(sf::Vertex{sf::Vector2f((float)WIDTH, (float)(y*CELL_SIZE)), gridCol});
            }
            window.draw(lines);
        }

        // Cursor highlight
        if(gx >= 0 && gx < COLS && gy >= 0 && gy < ROWS) {
            sf::RectangleShape highlight({(float)CELL_SIZE, (float)CELL_SIZE});
            highlight.setPosition({(float)(gx*CELL_SIZE), (float)(gy*CELL_SIZE)});
            highlight.setFillColor(sf::Color(255,255,255,40));
            highlight.setOutlineColor(sf::Color(255,255,255,120));
            highlight.setOutlineThickness(1.f);
            window.draw(highlight);
        }

        if(hasFont) {
            hud.setString(
                "LMB: Draw | RMB: Erase | Space: " + std::string(playing?"Pause":"Play") +
                " | N: Step | C: Clear | R: Random | G/P/U: Glider/Pulsar/Gun | ~: Grid\n"
                "Gen: " + std::to_string(generation) +
                "   Alive: " + std::to_string(liveCount) +
                "   Speed: " + std::to_string((int)(1.f/stepInterval)) + " gen/s (Up/Down to adjust)"
            );
            window.draw(hud);
        }

        window.display();
    }
    return 0;
}

