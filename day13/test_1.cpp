#include "gmock/gmock.h"
#include "utils.h"

using namespace testing;

TEST(Compare, int_int_inf) {
	IntPacket left(4);
	IntPacket right(10);

	ASSERT_LT(compare(left, right), 0);
}

TEST(Compare, int_int_sup) {
	IntPacket left(10);
	IntPacket right(4);

	ASSERT_GT(compare(left, right), 0);
}

TEST(Compare, int_int_eq) {
	IntPacket left(4);
	IntPacket right(4);

	ASSERT_EQ(compare(left, right), 0);
}

TEST(Compare, vec_vec_inf_1) {
	VectorPacket left {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(8),
		new IntPacket(2)
	};
	VectorPacket right {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(10),
		new IntPacket(2)
	};

	ASSERT_LT(compare(left, right), 0);
}

TEST(Compare, vec_vec_inf_2) {
	VectorPacket left {
		new IntPacket(4),
		new IntPacket(12),
	};
	VectorPacket right {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(10),
		new IntPacket(2)
	};

	ASSERT_LT(compare(left, right), 0);
}

TEST(Compare, vec_vec_sup_1) {
	VectorPacket left {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(10),
		new IntPacket(2)
	};
	VectorPacket right {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(8),
		new IntPacket(2)
	};

	ASSERT_GT(compare(left, right), 0);
}

TEST(Compare, vec_vec_sup_2) {
	VectorPacket left {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(10),
		new IntPacket(2)
	};
	VectorPacket right {
		new IntPacket(4),
		new IntPacket(12),
	};

	ASSERT_GT(compare(left, right), 0);
}

TEST(Compare, vec_vec_eq) {
	VectorPacket left {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(8),
		new IntPacket(2)
	};
	VectorPacket right {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(8),
		new IntPacket(2)
	};

	ASSERT_EQ(compare(left, right), 0);
}

TEST(Compare, int_vec_inf) {
	IntPacket left {3};
	VectorPacket right {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(8),
		new IntPacket(2)
	};

	ASSERT_LT(compare(left, right), 0);
}

TEST(Compare, int_vec_sup) {
	IntPacket left {8};
	VectorPacket right {
		new IntPacket(4),
		new IntPacket(12),
		new IntPacket(8),
		new IntPacket(2)
	};

	ASSERT_GT(compare(left, right), 0);
}

TEST(Compare, int_vec_eq) {
	IntPacket left {8};
	VectorPacket right {
		new IntPacket(8)
	};

	ASSERT_EQ(compare(left, right), 0);
}

TEST(Compare, example_1) {
	VectorPacket left {
		new IntPacket(1),
		new IntPacket(1),
		new IntPacket(3),
		new IntPacket(1),
		new IntPacket(1),
	};
	VectorPacket right {
		new IntPacket(1),
		new IntPacket(1),
		new IntPacket(5),
		new IntPacket(1),
		new IntPacket(1),
	};

	ASSERT_LT(compare(left, right), 0);
}

TEST(Compare, example_2) {
	VectorPacket left {
		new VectorPacket {
			new IntPacket(1)
		},
		new VectorPacket {
			new IntPacket(2),
			new IntPacket(3),
			new IntPacket(4)
		}
	};
	VectorPacket right {
		new VectorPacket {
			new IntPacket(1)
		},
		new IntPacket(4)
	};

	ASSERT_LT(compare(left, right), 0);
}

TEST(Compare, example_3) {
	VectorPacket left {
		new IntPacket {9}
	};
	VectorPacket right {
		new VectorPacket {
			new IntPacket(8),
			new IntPacket(7),
			new IntPacket(6)
		}
	};

	ASSERT_GT(compare(left, right), 0);
}

TEST(Compare, example_4) {
	VectorPacket left {
		new VectorPacket {
			new IntPacket {4},
			new IntPacket {4}
		},
		new IntPacket {4},
		new IntPacket {4}
	};
	VectorPacket right {
		new VectorPacket {
			new IntPacket {4},
			new IntPacket {4}
		},
		new IntPacket {4},
		new IntPacket {4},
		new IntPacket {4}
	};

	ASSERT_LT(compare(left, right), 0);
}

TEST(Compare, example_5) {
	VectorPacket left {
		new IntPacket {7},
		new IntPacket {7},
		new IntPacket {7},
		new IntPacket {7}
	};
	VectorPacket right {
		new IntPacket {7},
		new IntPacket {7},
		new IntPacket {7}
	};

	ASSERT_GT(compare(left, right), 0);
}

TEST(Compare, example_6) {
	VectorPacket left;
	VectorPacket right {
		new IntPacket(3)
	};

	ASSERT_LT(compare(left, right), 0);
}

TEST(Compare, example_7) {
	VectorPacket left {
		new VectorPacket {
			new VectorPacket
		}
	};
	VectorPacket right {
		new VectorPacket
	};

	ASSERT_GT(compare(left, right), 0);
}

TEST(Compare, example_8) {
	VectorPacket left {
		new IntPacket {1},
		new VectorPacket {
			new IntPacket {2},
			new VectorPacket {
				new IntPacket {3},
				new VectorPacket {
					new IntPacket {4},
					new VectorPacket {
						new IntPacket {5},
						new IntPacket {6},
						new IntPacket {7}
					}
				}
			}
		},
		new IntPacket {8},
		new IntPacket {9}
	};
	VectorPacket right {
		new IntPacket {1},
		new VectorPacket {
			new IntPacket {2},
			new VectorPacket {
				new IntPacket {3},
				new VectorPacket {
					new IntPacket {4},
					new VectorPacket {
						new IntPacket {5},
						new IntPacket {6},
						new IntPacket {0}
					}
				}
			}
		},
		new IntPacket {8},
		new IntPacket {9}
	};

	ASSERT_GT(compare(left, right), 0);
}

TEST(Parse, int_value) {
	auto packet = parse_packet("4");

	ASSERT_THAT(packet, WhenDynamicCastTo<IntPacket*>(NotNull()));
	ASSERT_THAT(dynamic_cast<IntPacket*>(packet)->value, 4);

	delete packet;
}

TEST(Parse, void_list) {
	auto packet = parse_packet("[]");

	ASSERT_THAT(packet, WhenDynamicCastTo<VectorPacket*>(NotNull()));
	ASSERT_THAT(dynamic_cast<VectorPacket*>(packet)->packets, IsEmpty());
}

TEST(Parse, list) {
	auto packet = parse_packet("[2, 3, 4]");

	ASSERT_THAT(packet, WhenDynamicCastTo<VectorPacket*>(NotNull()));
	ASSERT_THAT(dynamic_cast<VectorPacket*>(packet)->packets, SizeIs(3));
	ASSERT_EQ(
			dynamic_cast<IntPacket*>(dynamic_cast<VectorPacket*>(packet)->packets[0])->value,
			2
			);
	ASSERT_EQ(
			dynamic_cast<IntPacket*>(dynamic_cast<VectorPacket*>(packet)->packets[1])->value,
			3
			);
	ASSERT_EQ(
			dynamic_cast<IntPacket*>(dynamic_cast<VectorPacket*>(packet)->packets[2])->value,
			4
			);
}

TEST(Parse, nested_list) {
	auto packet = parse_packet("[2, [3, 4]]");

	ASSERT_THAT(packet, WhenDynamicCastTo<VectorPacket*>(NotNull()));
	ASSERT_THAT(dynamic_cast<VectorPacket*>(packet)->packets, SizeIs(2));
	ASSERT_EQ(
			dynamic_cast<IntPacket*>(dynamic_cast<VectorPacket*>(packet)->packets[0])->value,
			2
			);
	ASSERT_THAT(dynamic_cast<VectorPacket*>(
				dynamic_cast<VectorPacket*>(packet)->packets[1]
				)->packets, SizeIs(2));
	ASSERT_EQ(
			dynamic_cast<IntPacket*>(
				dynamic_cast<VectorPacket*>(
					dynamic_cast<VectorPacket*>(packet)->packets[1]
					)->packets[0])
			->value,
			3
			);
	ASSERT_EQ(
			dynamic_cast<IntPacket*>(
				dynamic_cast<VectorPacket*>(
					dynamic_cast<VectorPacket*>(packet)->packets[1]
					)->packets[1])
			->value,
			4
			);
}

TEST(Parse, example_2_1) {
	auto* packet = parse_packet("[[1],[2,3,4]]");

	ASSERT_THAT(packet, WhenDynamicCastTo<VectorPacket*>(NotNull()));
	ASSERT_THAT(dynamic_cast<VectorPacket*>(packet)->packets, SizeIs(2));

	ASSERT_THAT(
			dynamic_cast<VectorPacket*>(
				dynamic_cast<VectorPacket*>(packet)->packets[0]
				)->packets, SizeIs(1)
			);
	ASSERT_THAT(
			dynamic_cast<IntPacket*>(
				dynamic_cast<VectorPacket*>(
					dynamic_cast<VectorPacket*>(packet)->packets[0]
					)
				->packets[0])
			->value,
			1
			);
	ASSERT_THAT(
			dynamic_cast<VectorPacket*>(
				dynamic_cast<VectorPacket*>(packet)->packets[1]
				)->packets, SizeIs(3)
			);

	ASSERT_THAT(
			dynamic_cast<IntPacket*>(
				dynamic_cast<VectorPacket*>(
					dynamic_cast<VectorPacket*>(packet)->packets[1]
					)
				->packets[0])
			->value,
			2
			);

	ASSERT_THAT(
			dynamic_cast<IntPacket*>(
				dynamic_cast<VectorPacket*>(
					dynamic_cast<VectorPacket*>(packet)->packets[1]
					)
				->packets[1])
			->value,
			3
			);

	ASSERT_THAT(
			dynamic_cast<IntPacket*>(
				dynamic_cast<VectorPacket*>(
					dynamic_cast<VectorPacket*>(packet)->packets[1]
					)
				->packets[2])
			->value,
			4
			);
}

TEST(Compare, edge_case) {
	VectorPacket left {
		new VectorPacket {
			new VectorPacket,
			new IntPacket(6)
		}
	};
	VectorPacket right {
		new VectorPacket {
			new VectorPacket
		}
	};

	ASSERT_GT(compare(left, right), 0);
}

TEST(Parse, edge_case) {
	auto* packet = parse_packet("[[[],6]]");

	ASSERT_THAT(packet, WhenDynamicCastTo<VectorPacket*>(NotNull()));
	ASSERT_THAT(dynamic_cast<VectorPacket*>(packet)->packets, SizeIs(1));
	ASSERT_THAT(
			dynamic_cast<VectorPacket*>(
				dynamic_cast<VectorPacket*>(packet)->packets[0]
				)->packets,
			SizeIs(2)
			);
	ASSERT_THAT(
			dynamic_cast<VectorPacket*>(
				dynamic_cast<VectorPacket*>(
					dynamic_cast<VectorPacket*>(packet)->packets[0]
					)->packets[0]
				)->packets,
			SizeIs(0)
			);
	ASSERT_THAT(
			dynamic_cast<IntPacket*>(
				dynamic_cast<VectorPacket*>(
					dynamic_cast<VectorPacket*>(packet)->packets[0]
					)->packets[1]
				)->value,
			6	
			);
}

