#include "m_gameoflifegrid.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace GameOfLife {

Grid::Grid(int width, int height)
: d_world(height, std::vector<int>(width, 0))
, d_neighbours(d_world)
, d_changed(false)
, d_started(false)
, d_stateCount(0)
, d_simulationStatus(permanent)
{
    init();
}

void Grid::init()
{
    d_stateCount = 0;
    d_simulationStatus = permanent;
    d_changed = true;
    std::srand(std::time(0));
    for (auto& line : d_world) {
        for (auto& value : line) {
            value = std::rand() % 2 == 0 ? 1 : 0;
        }
    }
}

void Grid::getWorld(World* world, bool update)
{
    if (update) {
        updateWorld();
    }
    if (hasChanged()) {
        std::copy(d_world.begin(), d_world.end(), world->begin());
    }
}

const Grid::World& Grid::getWorld()
{
    return d_world;
}

bool Grid::hasChanged()
{
    if (d_changed) {
        d_changed = false;
        return true;
    }
    return false;
}

bool Grid::hasStarted()
{
    return d_started;
}

void Grid::run()
{
    d_started = true;

}

void Grid::stop()
{
    d_started = false;
}

int stateCount = 0;
void Grid::updateWorld()
{
    if (!d_started) run();
    if (d_simulationStatus == State::permanent) {
        updateNeighbours();
        updateGrid();
        d_simulationStatus = State::transitional;
    } else if (d_simulationStatus == State::transitional) {
        updateTransitionalGrid();
        d_stateCount++;
        if (3 == d_stateCount) {
            d_simulationStatus = State::permanent;
            d_stateCount = 0;
        }
    }
    d_changed = true;
}

bool inRange(int val, int max, int min = 0)
{
    return val >= min && val < max;
}

void updateLine(Grid::Line& line, int center, bool updateCenter)
{
    if (center - 1 >= 0) ++line[center - 1];
    if (center + 1 < line.size()) ++line[center + 1];
    if (updateCenter) ++line[center];
}

void updateNeighbourhood(Grid::World& neighbours, int x, int y)
{
    if (y - 1 >= 0) updateLine(neighbours[y -1], x, true);
    if (y + 1 < neighbours.size()) updateLine(neighbours[y + 1], x, true);
    updateLine(neighbours[y], x, false);
}

void Grid::updateNeighbours()
{
    for (auto& line : d_neighbours) { for (auto& v : line) { v = 0; } }
    for (int y = 0; y < d_world.size(); ++y) {
        const auto& line = d_world[y];
        for (int x = 0; x < line.size(); ++x) {
            if (line[x] == 0) continue;
            updateNeighbourhood(d_neighbours, x, y);
        }
    }
}

void Grid::updateGrid()
{
    for (int y = 0; y < d_neighbours.size(); ++y) {
        const auto& nLine = d_neighbours[y];
        auto& wLine = d_world[y];
        for (int x = 0; x < nLine.size(); ++x) {
            if ((nLine[x] < 2 || nLine[x] > 3)
                && wLine[x] == 1) {
                wLine[x] = -3;
            } else if (nLine[x] == 3) {
                wLine[x] = wLine[x] == 0 ? 4 : 1;
            }
        }
    }
}

void Grid::updateTransitionalGrid()
{
    for (auto& line : d_world) {
        for (auto& v : line) {
            if (v > 1) --v;
            if (v < 0) ++v;
        }
    }
}

} // closing project namespace
