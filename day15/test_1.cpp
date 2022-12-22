#include "gmock/gmock.h"
#include "utils.h"

using testing::ElementsAre;
using testing::ElementsAreArray;

TEST(Merge, exclude_2) {
	std::set<Range> ranges;
	ranges.insert(Range(10, 12));
	ranges.insert(Range(-4, 7));

	auto copy = ranges;
	copy = merge(ranges);

	ASSERT_THAT(copy, ElementsAreArray(ranges));
}

TEST(Merge, include_2) {
	std::set<Range> ranges;
	ranges.insert(Range(4, 12));
	ranges.insert(Range(7, 10));

	ASSERT_THAT(merge(ranges), ElementsAre(Range(4, 12)));
}

TEST(Merge, include_left_2) {
	std::set<Range> ranges;
	ranges.insert(Range(7, 12));
	ranges.insert(Range(4, 10));

	ASSERT_THAT(merge(ranges), ElementsAre(Range(4, 12)));
}

TEST(Merge, include_right_2) {
	std::set<Range> ranges;
	ranges.insert(Range(4, 10));
	ranges.insert(Range(7, 12));

	ASSERT_THAT(merge(ranges), ElementsAre(Range(4, 12)));
}

TEST(Merge, exclude_3) {
	std::set<Range> ranges;
	ranges.insert(Range(14, 18));
	ranges.insert(Range(-4, 7));
	ranges.insert(Range(10, 11));

	auto copy = ranges;
	copy = merge(ranges);

	ASSERT_THAT(copy, ElementsAreArray(ranges));
}

TEST(Merge, include_3) {
	std::set<Range> ranges;
	ranges.insert(Range(4, 12));
	ranges.insert(Range(6, 10));
	ranges.insert(Range(8, 10));

	ASSERT_THAT(merge(ranges), ElementsAre(Range(4, 12)));
}

TEST(Merge, center_intersection_3) {
	std::set<Range> ranges;
	ranges.insert(Range(4, 12));
	ranges.insert(Range(8, 14));
	ranges.insert(Range(9, 20));

	ASSERT_THAT(merge(ranges), ElementsAre(Range(4, 20)));
}

TEST(Merge, partial_intersection_3) {
	std::set<Range> ranges;
	ranges.insert(Range(4, 12));
	ranges.insert(Range(16, 20));
	ranges.insert(Range(8, 14));

	ASSERT_THAT(merge(ranges), ElementsAre(
				Range(4, 14),
				Range(16, 20)
				));
}
