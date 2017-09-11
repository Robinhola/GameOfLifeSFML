#include "m_gameoflifegrid.h"
#include "m_gameoflifegraphics.h"

using namespace GameOfLife;

int main()
{
    int width  = 160;
    int height = 100;

    Grid grid(width, height);

    Graphics<Grid::World>::WorldAccessor accessor(
        [&grid] (Grid::World * world_p, bool update)
        {
            grid.getWorld(world_p, update);
        });

    Graphics<Grid::World>::Initialisation init(
        [&grid] ()
        {
            grid.init();
        });

    Graphics<Grid::World>::Stoping stop(
        [&grid] ()
        {
            grid.stop();
        });

    Graphics<Grid::World> graphics(accessor, init, stop, grid.getWorld());

    graphics.run();

	return 0;
}
