#ifndef M_GAMEOFLIFEGRID_H
#define M_GAMEOFLIFEGRID_H

#include <vector>

namespace GameOfLife {

class Grid {
  public:
    using Line  = std::vector<int>;
    using World = std::vector<Line>;

  private:
    enum State { permanent, transitional };
    World d_world;
    World d_neighbours;
    bool  d_changed;
    bool  d_started;
    int   d_stateCount;
    State d_simulationStatus;

    void updateTransitionalGrid();
    void updateGrid();
    void updateNeighbours();
    void updateWorld();

  public:
    Grid(int width, int height);
    void init();
    void getWorld(World* world, bool updateWorld = false);
    const World& getWorld();
    bool hasChanged();
    bool hasStarted();
    void run();
    void stop();
};

} // closing project namespace

#endif

