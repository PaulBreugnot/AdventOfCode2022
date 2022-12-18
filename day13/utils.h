#include <vector>
#include <string>

struct Packet {
	virtual ~Packet() {
	}
};

struct VectorPacket : public Packet {
	std::vector<Packet*> packets;

	VectorPacket() = default;
	VectorPacket(std::initializer_list<Packet*> init) : packets(init) {
	}

	VectorPacket& operator=(const VectorPacket&) = delete;
	VectorPacket& operator=(VectorPacket&&) = delete;
	VectorPacket(const VectorPacket&) = delete;
	VectorPacket(VectorPacket&&) = delete;

	~VectorPacket() {
		for(auto packet : packets)
			delete packet;
	}
};

struct IntPacket : public Packet {
	int value;
	IntPacket(int value) : value(value) {
	}
};

int compare(const Packet& left, const Packet& right);
int compare(const IntPacket& left, const IntPacket& right);
int compare(const VectorPacket& left, const IntPacket& right);
int compare(const IntPacket& left, const VectorPacket& right);
int compare(const VectorPacket& left, const VectorPacket& right);

Packet* parse_packet(std::string str);

struct PacketComparator {

	bool operator()(
			const std::pair<std::string, Packet*>& p1,
			const std::pair<std::string, Packet*>& p2) const {
		return compare(*p1.second, *p2.second) < 0;
	}
};
