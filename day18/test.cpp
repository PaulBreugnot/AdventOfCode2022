#include <gmock/gmock.h>
#include "utils.h"

TEST(Surface, single_point) {
	World world;
	world.setOrigin({-1, -1, -1});
	world.setExtent({1, 1, 1});

	ASSERT_EQ(world.surface({{0, 0, 0}}), 6);
}

TEST(OuterSurface, full) {
	World world;
	world.setOrigin({-1, -1, -1});
	world.setExtent({1, 1, 1});

	std::vector<Point> points;
	for(int x = -1; x <= 1; x++)
		for(int y = -1; y <= 1; y++)
			for(int z = -1; z <= 1; z++)
				points.push_back({x, y, z});
	ASSERT_EQ(world.surface(points), 0);
}

TEST(Surface, outer_single_hole) {
	World world;
	world.setOrigin({-1, -1, -1});
	world.setExtent({1, 1, 1});

	std::vector<Point> points;
	for(int x = -1; x <= 1; x++)
		for(int y = -1; y <= 1; y++)
			for(int z = -1; z <= 1; z++)
				if(!(x == 0 && y == 0 && z== 0))
					points.push_back({x, y, z});
	ASSERT_EQ(world.surface(points), 6);
}

TEST(OuterSurface, single_hole) {
	World world;
	world.setOrigin({-1, -1, -1});
	world.setExtent({1, 1, 1});

	ASSERT_EQ(world.outerSurface({{0, 0, 0}}), 6);
}

TEST(OuterSurface, unreachable_hole) {
	World world;
	world.setOrigin({-2, -2, -2});
	world.setExtent({2, 2, 2});

	Point p {0, 0, 0};
	std::vector<Point> points;
	for(auto& n : p.neighbors())
		points.push_back(n);

	ASSERT_EQ(world.outerSurface(points), 30);
}
